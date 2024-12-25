#include <iostream>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <cstring>

#define FILE_NAME "memory_mapped_file.dat"  // Имя файла для отображения в память
#define LONG_STRINGS_FILE "long_strings.txt"  // Имя файла для длинных строк
#define SHORT_STRINGS_FILE "short_strings.txt"  // Имя файла для коротких строк

int main() {
    pid_t pid1, pid2;  // Идентификаторы процессов для двух дочерних процессов
    std::string input_str;  // Вводимая строка
    char *mapped_data;  // Указатель на область памяти

    // Открытие файла для отображения в память
    int fd = open(FILE_NAME, O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        perror("Ошибка при открытии файла для отображения в память");
        exit(1);
    }

    // Установка размера файла для отображения в память
    if (ftruncate(fd, 1024) == -1) {
        perror("Ошибка при установке размера файла");
        exit(1);
    }

    // Отображаем файл в память
    mapped_data = (char *)mmap(NULL, 1024, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); //mmap - возвращает указатель на отображенную область памяти
    if (mapped_data == MAP_FAILED) {
        perror("Ошибка при отображении файла в память");
        exit(1);
    }

    while (true) {
        // Запрос строки у пользователя
        std::cout << "Введите строку: ";
        std::getline(std::cin, input_str);

        // Если строка пустая, завершаем выполнение программы
        if (input_str.empty()) {
            break;
        }

        // Записываем строку в общую память
        strncpy(mapped_data, input_str.c_str(), 1024);

        // Создание двух дочерних процессов - В каждом дочернем процессе вызывается программа child с соответствующим аргументом (long или short).
        if (input_str.length() > 10) {
            pid1 = fork();  // Для длинных строк
            if (pid1 == -1) {
                perror("Ошибка при создании первого дочернего процесса");
                exit(1);
            }
            if (pid1 == 0) {
                // Запускаем дочернюю программу для длинных строк
                execl("./child", "child", "long", NULL);
                perror("Ошибка при запуске дочернего процесса для длинных строк");
                exit(1);
            }
        } else {
            pid2 = fork();  // Для коротких строк
            if (pid2 == -1) {
                perror("Ошибка при создании второго дочернего процесса");
                exit(1);
            }
            if (pid2 == 0) {
                // Запускаем дочернюю программу для коротких строк
                execl("./child", "child", "short", NULL);
                perror("Ошибка при запуске дочернего процесса для коротких строк");
                exit(1);
            }
        }

        // Ожидаем завершения обоих дочерних процессов
        if (pid1 != 0) {
            int status;
            waitpid(pid1, &status, 0);  // Ожидаем завершения первого процесса (если был создан)
        }

        if (pid2 != 0) {
            int status;
            waitpid(pid2, &status, 0);  // Ожидаем завершения второго процесса (если был создан)
        }
    }

    // освобождаем память и закрываем файл
    if (munmap(mapped_data, 1024) == -1) {
        perror("Ошибка при освобождении памяти");
        exit(1);
    }
    close(fd);

    return 0;
}

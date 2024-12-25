#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>
#include <cstdio>  // Добавляем для использования FILENAME_MAX

#define BUFFER_SIZE 1024

// Правило фильтрации: строки длины больше 10 символов отправляются в pipe2, иначе в pipe1
bool filterRule(const char* str) {
    return strlen(str) > 10;
}

int main() {
    int pipe1[2], pipe2[2];

    // Создаем два канала
    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {  //если возвращает -1, это означает, что произошла ошибка при создании канала.
        perror("pipe");
        return 1;
    }

    // Ввод имен файлов для дочерних процессов
    std::string filename1, filename2;


    std::cout << "введите имя файла для child1: ";
    std::getline(std::cin, filename1);
    // Проверяем, что имя файла не превышает допустимую длину
    if (filename1.size() >= FILENAME_MAX) {
        std::cerr << "Имя файла для child1 слишком длинное" << std::endl;
        return 1;
    }

    std::cout << "введите имя файла для child2: ";
    std::getline(std::cin, filename2);
    // Проверяем, что имя файла не превышает допустимую длину
    if (filename2.size() >= FILENAME_MAX) {
        std::cerr << "Имя файла для child2 слишком длинное" << std::endl;
        return 1;
    }

    // Создаем первый дочерний процесс
    pid_t pid1 = fork(); //pid_t - хранения id процесса pid1 - id нового
    if (pid1 == -1) {
        perror("fork");
        return 1; //завершает программу с кодом ошибки 1.
    } else if (pid1 == 0) {
        // Дочерний процесс 1
        close(pipe1[1]); // Закрываем запись в pipe1
        dup2(pipe1[0], STDIN_FILENO); // Перенаправляем stdin на чтение из pipe1
        close(pipe1[0]); // Закрываем дескриптор pipe1(для записи)дочерний процесс будет только читать данные из канала
        execl("./child_process", "child_process", filename1.c_str(), NULL); // Заменяем текущий процесс на дочерний процесс
        perror("execl"); // Если execl завершится с ошибкой
        exit(1); // Явное завершение процесса с кодом ошибки
    }

    // Создаем второй дочерний процесс
    pid_t pid2 = fork();
    if (pid2 == -1) {
        perror("fork");
        return 1;
    } else if (pid2 == 0) {
        // Дочерний процесс 2
        close(pipe2[1]); // Закрываем запись в pipe2
        dup2(pipe2[0], STDIN_FILENO); // Перенаправляем stdin на чтение из pipe2
        close(pipe2[0]); // Закрываем дескриптор pipe2
        execl("./child_process", "child_process", filename2.c_str(), NULL); // Заменяем текущий процесс на дочерний процесс
        perror("execl"); // Если execl завершится с ошибкой
        return 1;
    }

    // Родительский процесс
    close(pipe1[0]); // Закрываем чтение из pipe1
    close(pipe2[0]); // Закрываем чтение из pipe2

    std::string input;
    while (true) {
        std::cout << "Введите строку (или 'exit' чтобы завершить): ";
        std::getline(std::cin, input);

        if (input == "exit") {
            char str = static_cast<char>(EOF);
            write(pipe1[1], &str,1);
            write(pipe2[1], &str,1);
            break;
        }

        // Проверка на переполнение буфера
        if (input.size() >= BUFFER_SIZE) {
            std::cerr << "Ошибка: входная строка слишком длинная и превышает размер буфера." << std::endl;
            continue;
        }

       // Проверяем, соответствует ли строка правилу фильтрации
        if (filterRule(input.c_str())) {
            // Если строка длиннее 10 символов, отправляем её в pipe2
            if (write(pipe2[1], input.c_str(), input.size()) == -1) { 
                //write возвращает -1, это означает, что произошла ошибка при записи данных в канал.
                // Если произошла ошибка при записи в pipe2, выводим сообщение об ошибке и выходим из цикла
                perror("write");
                break;
            }
        } else {
            // Если строка короче или равна 10 символам, отправляем её в pipe1
            if (write(pipe1[1], input.c_str(), input.size()) == -1) {
                // Если произошла ошибка при записи в pipe1, выводим сообщение об ошибке и выходим из цикла
                perror("write");
                break;
            }
        }
    }

    // Закрываем каналы записи
    close(pipe1[1]);
    close(pipe2[1]);

    // Ждем завершения дочерних процессов
    std::cout << "waiting child processes ..." << std::endl;
    wait(NULL); 
    wait(NULL);

    return 0;
}
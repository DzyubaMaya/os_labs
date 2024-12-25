#include <iostream>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <cstring>
#include <cctype>

#define FILE_NAME "memory_mapped_file.dat"  // Имя файла для отображения в память
#define LONG_STRINGS_FILE "long_strings.txt"  // Имя файла для длинных строк
#define SHORT_STRINGS_FILE "short_strings.txt"  // Имя файла для коротких строк

// Функция для удаления гласных букв из строки
std::string remove_vowels(const std::string &str) {
    std::string result;
    for (char c : str) { //c - текущий символ из строки
        if (std::strchr("aeiouAEIOU", c) == NULL) {
            result += c;
        }
    }
    return result;
}

int main(int argc, char *argv[]) {
    if (argc != 2 || (std::string(argv[1]) != "long" && std::string(argv[1]) != "short")) {
        std::cerr << "Необходим параметр (long или short) для определения файла.\n";
        exit(1);
    }

    char *mapped_data;  // Указатель на область памяти

    // Открытие файла для отображения в память
    int fd = open(FILE_NAME, O_RDWR, 0666);
    if (fd == -1) {
        perror("Ошибка при открытии файла для отображения в память");
        exit(1);
    }

    // Отображаем файл в память
    mapped_data = (char *)mmap(NULL, 1024, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (mapped_data == MAP_FAILED) {
        perror("Ошибка при отображении файла в память");
        close(fd);
        exit(1);
    }

    std::string input_str(mapped_data);  // Читаем строку из общей памяти

    if (input_str.empty()) {
        std::cerr << "Строка пуста в общей памяти.\n";
        exit(1);
    }

    // Удаляем гласные буквы из строки
    std::string processed_str = remove_vowels(input_str);


    //определяем имя выходного файла
    const char *output_file_name;
    if (std::string(argv[1]) == "long") {
        output_file_name = LONG_STRINGS_FILE;
    } else {
        output_file_name = SHORT_STRINGS_FILE;
    }

    // Открываем файл для записи
    int output_fd = open(output_file_name, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (output_fd == -1) {
        perror("Ошибка при открытии файла для записи");
        exit(1);
    }

    // Записываем обработанную строку в файл
    if (write(output_fd, processed_str.c_str(), processed_str.length()) == -1) { //строка processed_str в файл
        perror("Ошибка при записи в файл");
        close(output_fd);
        exit(1);
    }

    // Закрываем файл
    close(output_fd);

    // Выводим сообщение о том, в какой файл была записана строка
    if (std::string(argv[1]) == "long") {
        std::cout << "Записали в файл длинных строк: " << processed_str << std::endl;
    } else {
        std::cout << "Записали в файл коротких строк: " << processed_str << std::endl;
    }

    // освобождаем и закрываем файл
    if (munmap(mapped_data, 1024) == -1) {
        perror("Ошибка при освобождении памяти");
        close(fd);
        exit(1);
    }
    close(fd);

    return 0;
}
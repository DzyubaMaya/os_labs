#include <iostream>
#include <fstream>
#include <unistd.h>
#include <cstring>

#define BUFFER_SIZE 1024

// Функция для удаления гласных букв из строки
void removeVowels(char* str) {
    // Массив гласных букв
    char vowels[] = "AEIOUYaeiouy";
    int j = 0; //отслеживание новой позиции в str
    // Проходим по всей строке
    for (int i = 0; str[i] != '\0'; ++i) {
        // Если текущий символ не является гласной буквой
        if (strchr(vowels, str[i])) {
            // если сивол не гласная - копируем его в новую позицию
            str[j++] = str[i];
        }
    }
    str[j] = '\0';
}

int main(int argc, char* argv[]) {
    // Проверяем, передан ли аргумент с именем файла
    if (argc < 2) {
        std::cerr << "Использование: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    // Открываем файл для записи
    std::ofstream outFile(argv[1]);
    if (!outFile.is_open()) {
        std::cerr << "Не удалось открыть файл: " << argv[1] << std::endl;
        return 1;
    }

    // Буфер для чтения данных
    char buffer[BUFFER_SIZE];
    while (true) {
        // Читаем данные из стандартного ввода (stdin)
        ssize_t bytesRead = read(STDIN_FILENO, buffer, BUFFER_SIZE); //BUFFER_SIZE — максимальное количество байтов, которые могут быть прочитаны за один раз.
        //bytesRead — количество фактически прочитанных байтов.
        // Если данные не были прочитаны или достигнут конец ввода, завершаем цикл
        if ((bytesRead <= 0) || (bytesRead ==1 && buffer[0] == static_cast<char>(EOF))) {
            break;
        } 

        buffer[bytesRead] = '\0';

        // Удаляем гласные буквы из буфера
        removeVowels(buffer);

        // Записываем обработанные данные в файл
        outFile << buffer << std::endl;
        // Выводим обработанные данные на стандартный вывод (stdout)
        std::cout << "Обработано: " << buffer << std::endl;
    }

    // Закрываем файл
    outFile.close();

    return 0;
}
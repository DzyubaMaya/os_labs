#include <iostream>
#include <vector>
#include <thread>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <algorithm>

// Функция быстрой сортировки
void quick_sort(std::vector<int>* arr, int start, int end) {
    if (start >= end) return; 

    // Выбираем опорный элемент как средний элемент в диапазоне
    int element = (*arr)[start + (end - start) / 2];
    int i = start, j = end;

    while (i <= j) {
        while ((*arr)[i] < element) i++;
        while ((*arr)[j] > element) j--;
        if (i <= j) {
            // Временная переменная для хранения значения элемента
            int temp = (*arr)[i];
            // Меняем местами элементы
            (*arr)[i] = (*arr)[j];
            (*arr)[j] = temp;
            i++;
            j--;
        }
    }
    // Рекурсивно сортируем левую часть массива и правую часть 
    if (start < j) quick_sort(arr, start, j);
    if (i < end) quick_sort(arr, i, end);
}

// Функция, которая будет выполняться в каждом потоке
void sort_chunk(std::vector<int>* arr, int start, int end) {
    quick_sort(arr, start, end - 1);
}

// Функция слияния двух отсортированных частей массива
void merge(std::vector<int>& arr, int start, int mid, int end) {
    std::vector<int> temp(end - start + 1);  // Временный массив для хранения результата слияния
    int i = start, j = mid + 1, k = 0;  // Указатели для обхода частей массива

    // Слияние двух частей в один отсортированный массив
    while (i <= mid && j <= end) {
        if (arr[i] <= arr[j]) {
            temp[k++] = arr[i++];  // Если элемент из левой части меньше, добавляем его во временный массив
        } else {
            temp[k++] = arr[j++];  // Иначе добавляем элемент из правой части
        }
    }

    // Добавляем оставшиеся элементы из левой части
    while (i <= mid) {
        temp[k++] = arr[i++];
    }

    // Добавляем оставшиеся элементы из правой части
    while (j <= end) {
        temp[k++] = arr[j++];
    }

    // Копируем отсортированные элементы из временного массива обратно в исходный массив
    for (i = start; i <= end; ++i) {
        arr[i] = temp[i - start];
    }
}

// Функция объединения всех отсортированных частей массива
void merge_all_parts(std::vector<int>& arr, int max_threads, int chunk_size, int remainder) {
    // Проходим по всем частям массива, начиная со второй части
    for (int i = 1; i < max_threads; ++i) {
        // Вычисляем начало и конец текущей части
        int start = i * chunk_size;
        int mid = start - 1;  // Середина предыдущей части
        int end = (i == max_threads - 1) ? arr.size() - 1 : start + chunk_size - 1;  // Конец текущей части Если текущая часть является последней, то конец будет равен последнему элементу массива, иначе — концу части.

        // Вызываем функцию слияния для объединения текущей части с уже отсортированной частью массива
        merge(arr, 0, mid, end);
    }
}

int main(int argc, char* argv[]) {
    // 1. Инициализация и ввод параметров
    if (argc != 3) {
        std::cerr << "Использование: " << argv[0] << " <размер_массива> <максимальное_количество_потоков>" << std::endl;
        return 1;
    }

    int array_size = std::atoi(argv[1]);
    int max_threads = std::atoi(argv[2]);

    if (array_size <= 0 || max_threads <= 0) {
        std::cerr << "Размер массива и максимальное количество потоков должны быть положительными целыми числами." << std::endl;
        return 1;
    }

    std::cout << "Размер массива: " << array_size << ", Максимальное количество потоков: " << max_threads << std::endl;

    // Проверка, что количество потоков не превышает размер массива
    if (max_threads > array_size) {
        max_threads = array_size;
        std::cout << "Количество потоков уменьшено до размера массива: " << max_threads << std::endl;
    }

    // 2. Генерация массива
    std::vector<int> arr(array_size);
    std::srand(std::time(0));
    for (size_t i = 0; i < arr.size(); ++i) {
        arr[i] = std::rand() % 100;
    }

    std::cout << "Массив сгенерирован" << std::endl;

    // Сохраняем исходный массив перед сортировкой
    std::vector<int> original_arr = arr;

    // 3. Разделение задачи на потоки
    std::vector<std::thread> threads;

    int chunk_size = array_size / max_threads;
    int remainder = array_size % max_threads;

    std::cout << "Размер части: " << chunk_size << ", Остаток: " << remainder << std::endl;

    // 4. Создание и запуск потоков
    clock_t start_time = clock();  // Начало измерения времени

    int start = 0;
    for (int i = 0; i < max_threads; ++i) {
        int end = start + chunk_size;
        if (i == max_threads - 1) {
            end += remainder;
        }

        std::cout << "Создание потока " << i << ": начало=" << start << ", конец=" << end << std::endl;
        threads.emplace_back(sort_chunk, &arr, start, end); //emplace_back создает новый объект std::thread в контейнере threads.
         //Конструктор std::thread вызывается с аргументами sort_chunk, &arr, start, end
        start = end;
    }

    // 5. Ожидание завершения всех потоков
    for (auto& thread : threads) {
        thread.join();
    }

    // 6. Объединение результатов (слияние отсортированных частей)
    merge_all_parts(arr, max_threads, chunk_size, remainder);

    clock_t end_time = clock();  // Конец измерения времени
    double time_taken = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

    // 7. Вывод времени выполнения
    std::cout << "Время выполнения: " << time_taken << " секунд" << std::endl;

    // 8. Запись результатов в файл
    std::ofstream output_file("output.txt");
    if (output_file.is_open()) {
        // Вывод исходного массива в файл
        output_file << "Исходный массив:" << std::endl;
        for (int num : original_arr) {
            output_file << num << " ";
        }
        output_file << std::endl;

        // Вывод отсортированного массива в файл
        output_file << "Отсортированный массив:" << std::endl;
        for (int num : arr) {
            output_file << num << " ";
        }
        output_file << std::endl;

        // Вывод минимального и максимального элементов
        output_file << "Минимальный элемент: " << arr.front() << std::endl;
        output_file << "Максимальный элемент: " << arr.back() << std::endl;
        output_file.close();
        std::cout << "Результаты записаны в файл output.txt" << std::endl;
    } else {
        std::cerr << "Не удалось открыть файл для записи" << std::endl;
    }

    // 9. Завершение программы
    return 0;
}
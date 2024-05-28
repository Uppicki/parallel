#include "reader.h"

void readDataFromFile(const std::string& filename, std::vector<int>& array) {
    std::ifstream inputFile(filename); // Открываем файл для чтения

    if (!inputFile.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        return; // Возвращаемся в случае ошибки
    }

    int n;
    inputFile >> n; // Считываем количество элементов из первой строки файла

    array.resize(n - 1); // Изменяем размер массива в соответствии с количеством элементов

    // Считываем n-1 элементов из следующих n-1 строк файла
    for (int i = 0; i < n - 1; ++i) {
        inputFile >> array[i];
    }

    inputFile.close(); // Закрываем файл
}
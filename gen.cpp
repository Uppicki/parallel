#include <iostream>
#include <fstream>
#include <random>

int main() {
    const int count = 10000000;

    // Открытие файла для записи
    std::ofstream outputFile("res/numbers.txt");

    if (outputFile.is_open()) {
        // Инициализация генератора случайных чисел
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> distrib(1, 1000); // Меняем диапазон, если нужно

        outputFile << count << std::endl;

        // Генерация случайных чисел и запись их в файл
        for (int i = 0; i < count; ++i) {
            int randomNumber = distrib(gen);
            outputFile << randomNumber << std::endl;
            if (i % 1000000 == 0) std::cout << i << std::endl;
        }

        // Закрытие файла
        outputFile.close();
        std::cout << "Successfully generated and saved " << count << " random numbers to 'random_numbers.txt'." << std::endl;
    } else {
        std::cerr << "Failed to open the file." << std::endl;
        return 1; // Возврат ненулевого значения в случае ошибки
    }

    return 0;
}
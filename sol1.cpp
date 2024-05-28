
#include <iostream>
#include <vector>

#include <chrono>

#include "utils\reader\reader.cpp"


int main(int argc, char* argv[]) {
    std::vector<int> original;

    std::vector<int> array;

    int sliceSize = 1000000;
    int checkStep = 10000;

    readDataFromFile("res/numbers.txt", original);

    array.assign(original.begin(), original.begin()+sliceSize);

    std::cout << "Start" << std::endl;

    long long sum = 0;

    auto start = std::chrono::steady_clock::now();

    for (int i = 0; i < array.size()-1; i++)
    {
        if (i % checkStep == 0) std::cout << i << " " << sum << std::endl;
        for (int j = (i+1); j < array.size(); j++)
        {
            if (array[i] > array[j]) sum++;
        }
    }

    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << sum << std::endl;

    std::cout << "Program executed in " << duration.count() << " milliseconds." << std::endl;

    


    return 0;
}

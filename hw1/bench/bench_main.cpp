#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include "mySort.h"

int main() {
    const int SIZE = 1000000; // Миллион элементов
    std::cout << "[BENCHMARK] Generating " << SIZE << " random numbers..." << std::endl;

    std::vector<int> data(SIZE);
    std::mt19937 gen(42); // Генератор случайных чисел (42 - фиксированное зерно для стабильности)
    std::uniform_int_distribution<> dis(1, 1000000);
    
    for (int i = 0; i < SIZE; ++i) {
        data[i] = dis(gen);
    }

    std::cout << "[BENCHMARK] Sorting..." << std::endl;

    // Запускаем секундомер
    auto start = std::chrono::high_resolution_clock::now();
    
    // Вызываем вашу сортировку
    mySort(data.begin(), data.end()); 
    
    // Останавливаем секундомер
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> time_taken = end - start;

    std::cout << "[BENCHMARK] Done! Time taken: " << time_taken.count() << " ms\n";

    return 0;
}
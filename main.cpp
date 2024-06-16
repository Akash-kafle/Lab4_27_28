#include <iostream>
#include <vector>
#include <cstdlib> // For rand() and srand()
#include <ctime>   // For time
#include <chrono>  // Forclock
#include "./insertion.h"


std::vector<int> generateRandomVector(int size) {
    std::vector<int> vec(size);
    for (int i = 0; i < size; ++i) {
        vec[i] = rand() % 1000;
    }
    return vec;
}

int main() {
    std::srand(std::time(0)); 

    std::vector<int> inputSizes = {10, 100, 1000, 10000, 20000};
    std::vector<double> executionTimes;

    for (int size : inputSizes) {
        std::vector<int> data = generateRandomVector(size);
        
        auto start = std::chrono::high_resolution_clock::now();
        insertionSort(data);
        auto end = std::chrono::high_resolution_clock::now();
        
        std::chrono::duration<double> duration = end - start;
        executionTimes.push_back(duration.count());
        
        std::cout << "Sorted " << size << " elements in " << duration.count() << " seconds." << std::endl;
    }

    std::cout << "Input Sizes: ";
    for (int size : inputSizes) {
        std::cout << size << " ";
    }
    std::cout << std::endl;

    std::cout << "Execution Times: ";
    for (double time : executionTimes) {
        std::cout << time << " ";
    }
    std::cout << std::endl;

    return 0;
}

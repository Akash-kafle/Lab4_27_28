#include "QuickSort.h"
void measurePerformance(int sampleSize, std::vector<std::tuple<int, double, double, double, double>> &durations);
void worstCase(std::vector<int> array, int sampleSize, double &duration, std::mutex &mtx);
void randomCase(std::vector<int> array, int sampleSize, double &duration, std::mutex &mtx);
void bestCase(std::vector<int> array, int sampleSize, double &duration, std::mutex &mtx);
void duplicateCase(std::vector<int> array, int sampleSize, double &duration, std::mutex &mtx);

int main()
{
    std::vector<std::tuple<int, double, double, double, double>> durations;
    std::cout << "Quick Sort Performance" << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    for (int sampleSize = 10; sampleSize <= 20000; sampleSize += 10) // Reduce to 1000 for testing
    {
        measurePerformance(sampleSize, durations);
    }

    std::cout << std::setw(10) << "Sample Size"
              << std::setw(20) << "Worst Case"
              << std::setw(20) << "Average Case"
              << std::setw(20) << "Best Case"
              << std::setw(20) << "Duplicates" << std::endl;
    double average_worst = 0;
    double average_average = 0;
    double average_best = 0;
    double average_duplicates = 0;

    for (auto &[size, worst, average, best, duplicates] : durations)
    {
        average_worst += worst;
        average_average += average;
        average_best += best;
        average_duplicates += duplicates;
    }
    std::cout << "Size: ";
    for (const auto &[size, worst, average, best, duplicates] : durations)
    {
        std::cout << size << ", ";
    }
    std::cout << std::endl
              << std::endl;
    std::cout << "worst: ";
    for (const auto &[size, worst, average, best, duplicates] : durations)
    {
        std::cout << worst << ", ";
    }
    std::cout << std::endl
              << std::endl;

    std::cout << "average: ";
    for (const auto &[size, worst, average, best, duplicates] : durations)
    {
        std::cout << average << ", ";
    }
    std::cout << std::endl
              << std::endl;
    std::cout << "best: ";
    for (const auto &[size, worst, average, best, duplicates] : durations)
    {
        std::cout << best << ", ";
    }
    std::cout << std::endl
              << std::endl;

    std::cout << "duplicate: ";
    for (const auto &[size, worst, average, best, duplicates] : durations)
    {
        std::cout << duplicates << ", ";
    }
    std::cout << std::endl
              << std::endl;

    auto end = std::chrono::high_resolution_clock::now();
    std::cout << average_worst / durations.size() << std::endl;
    std::cout << average_average / durations.size() << std::endl;
    std::cout << average_best / durations.size() << std::endl;
    std::cout << average_duplicates / durations.size() << std::endl;
    std::cout << "Total time: " << std::chrono::duration<double>(end - start).count() << " seconds" << std::endl;

    // std::cout << std::fixed << std::setprecision(3);
    // std::cout << std::setw(10) << "Average"
    //           << std::setw(20) << average_worst / durations.size()
    //           << std::setw(20) << average_average / durations.size()
    //           << std::setw(20) << average_best / durations.size()
    //           << std::setw(20) << average_duplicates / durations.size() << std::endl;
    // std::cout << std::fixed << std::setprecision(3);
    // std::cout << std::setw(5) << "total(seconds)"
    //           << std::setw(16) << average_worst / 1000
    //           << std::setw(20) << average_average / 1000
    //           << std::setw(20) << average_best / 1000
    //           << std::setw(20) << average_duplicates / 1000 << std::endl;
    // std::cout << "Total time: " << std::chrono::duration<double>(end - start).count() << " seconds" << std::endl;
    return 0;
}

void measurePerformance(int sampleSize, std::vector<std::tuple<int, double, double, double, double>> &durations)
{
    std::vector<int> array(sampleSize);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 100);

    for (int i = 0; i < sampleSize; i++)
    {
        array[i] = dis(gen);
    }

    double worst, average, best, duplicates;
    std::vector<int> arrayCopy1 = array;
    std::vector<int> arrayCopy2 = array;
    std::vector<int> arrayCopy3 = array;
    std::vector<int> arrayCopy4 = array;
    std::mutex mtx;

    std::thread worstThread(worstCase, arrayCopy1, sampleSize, std::ref(worst), std::ref(mtx));
    std::thread averageThread(randomCase, arrayCopy2, sampleSize, std::ref(average), std::ref(mtx));
    std::thread bestThread(bestCase, arrayCopy3, sampleSize, std::ref(best), std::ref(mtx));
    std::thread duplicatesThread(duplicateCase, arrayCopy4, sampleSize, std::ref(duplicates), std::ref(mtx));

    worstThread.join();
    averageThread.join();
    bestThread.join();
    duplicatesThread.join();

    durations.push_back(std::make_tuple(sampleSize, worst, average, best, duplicates));
}

void worstCase(std::vector<int> array, int sampleSize, double &duration, std::mutex &mtx)
{
    std::sort(array.begin(), array.end(), std::greater<int>());
    auto start = std::chrono::high_resolution_clock::now();
    quickSort(array, 0, sampleSize - 1);
    auto end = std::chrono::high_resolution_clock::now();
    double dur = std::chrono::duration<double>(end - start).count() * 1000;

    std::lock_guard<std::mutex> lock(mtx);
    duration = dur;
}

void randomCase(std::vector<int> array, int sampleSize, double &duration, std::mutex &mtx)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(array.begin(), array.end(), gen);

    auto start = std::chrono::high_resolution_clock::now();
    quickSort(array, 0, sampleSize - 1);
    auto end = std::chrono::high_resolution_clock::now();
    double dur = std::chrono::duration<double>(end - start).count() * 1000;

    std::lock_guard<std::mutex> lock(mtx);
    duration = dur;
}

void bestCase(std::vector<int> array, int sampleSize, double &duration, std::mutex &mtx)
{

    std::random_device rd;
    std::mt19937 gen(rd());
    for (int i = 0; i < sampleSize; i++)
    {
        float data = rand();
        if (i == 0)
        {
            array[i] = sampleSize / 2;
        }
        else if (data <= sampleSize / 2)
        {
            array[i] = sampleSize / 2 - data;
        }
        else
        {
            array[i] = sampleSize / 2 + data;
        }
    }

    auto start = std::chrono::high_resolution_clock::now();
    quickSort(array, 0, sampleSize - 1);
    auto end = std::chrono::high_resolution_clock::now();
    double dur = std::chrono::duration<double>(end - start).count() * 1000;

    std::lock_guard<std::mutex> lock(mtx);
    duration = dur;
}

void duplicateCase(std::vector<int> array, int sampleSize, double &duration, std::mutex &mtx)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 10);

    for (int i = 0; i < sampleSize; i++)
    {
        array[i] = dis(gen);
    }

    auto start = std::chrono::high_resolution_clock::now();
    quickSort(array, 0, sampleSize - 1);
    auto end = std::chrono::high_resolution_clock::now();
    double dur = std::chrono::duration<double>(end - start).count() * 1000;

    std::lock_guard<std::mutex> lock(mtx);
    duration = dur;
}
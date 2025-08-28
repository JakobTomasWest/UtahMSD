#include <iostream>
#include <vector>
#include <numeric>
#include <fstream>
#include <cassert>
#include <chrono>
#include <thread>
#include <atomic>
#include <omp.h>

template<typename T>
class ParallelSum {
public:

  static std::pair<T, std::chrono::duration<double>> parallel_sum_std(T a[], size_t N, size_t num_threads);
  static std::pair<T, std::chrono::duration<double>> parallel_sum_omp1(T a[], size_t N, size_t num_threads);
  static std::pair<T, std::chrono::duration<double>> parallel_sum_omp_builtin(T a[], size_t N, size_t num_threads);
};

template<typename T>
//sum of array and time ot took
std::pair<T, std::chrono::duration<double>> ParallelSum<T>::parallel_sum_std(T a[], size_t arraySize, size_t num_threads) {
    // std::atomic<T> globalSum (0);
    T globalSum = 0;
    std::vector<std::thread> mythreads;

    std::mutex globalSumMutex;
    mythreads.reserve(num_threads);
    //start timing
    auto start = std::chrono::high_resolution_clock::now();
    for(size_t id = 0; id < num_threads; ++id) {
        mythreads.emplace_back([&, id]() {
            //create own local sum variable for each thread
            T localSum = 0;
            //each thread calculates its roughly equal portion of the array  for the sum later
            // (size of array / number of threads)
            size_t start_index = id * (arraySize / num_threads);
            size_t end_index = (id + 1) * (arraySize / num_threads);
            if (id == num_threads - 1) {
                end_index = arraySize;
            }
            for(size_t j = start_index; j < end_index; ++j) {
                localSum += a[j];
            }
            //reduce into a global sum
            // globalSum.fetch_add(localSum, std::memory_order_relaxed);
            {
              std::lock_guard<std::mutex> lock(globalSumMutex);
                globalSum += localSum;
            }
        });
    }
    for(auto &thread : mythreads) {
        thread.join();
    }
    //end the clock
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    //return the sum of the threads work and the time it took to finish
    return {globalSum, duration}; //or .load()
}


template<typename T>
std::pair<T, std::chrono::duration<double>> ParallelSum<T>::parallel_sum_omp1(T a[], size_t arraySize, size_t num_threads) {
    T globalSum = 0;
    omp_set_num_threads(num_threads);
    auto start = std::chrono::high_resolution_clock::now();

#pragma omp parallel
    {
        //create private sum varaible for each thread
        T localSum = 0;
#pragma omp for nowait
        for(size_t i = 0; i < arraySize; ++i) {
            localSum += a[i];
        }
        //add each sum of each thread to the global sum one at a time
#pragma omp atomic
        globalSum += localSum;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    return {globalSum, duration};
}

template<typename T>
std::pair<T, std::chrono::duration<double>> ParallelSum<T>::parallel_sum_omp_builtin(T a[], size_t N, size_t num_threads) {
    T globalSum = 0;
    omp_set_num_threads(num_threads);
    auto start = std::chrono::high_resolution_clock::now();

#pragma omp parallel for reduction(+:globalSum)
    for(size_t i = 0; i < N; ++i) {
        globalSum += a[i];
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    return {globalSum, duration};
}

// Function to test parallel sum functions
template<typename T>
void runTests(const std::string& type, const std::vector<size_t>& N_values, const std::vector<size_t>& thread_counts, std::ofstream& results_file) {
    for (auto N : N_values) {
        std::vector<T> data(N);

        for (size_t i = 0; i < N; ++i) {
           data[i] = i + 1;
       }

        for (auto num_threads : thread_counts) {
            auto result_std = ParallelSum<T>::parallel_sum_std(data.data(), N, num_threads);
            // assert(result_std.first == N * (N + 1) / 2); // Verify the result is correct
            if (result_std.first != N * (N + 1) / 2) {
                        std::cerr << "Failed for N = " << N << " with num_threads = " << num_threads
                                  << ". Expected: " << (N * (N + 1) / 2) << ", Got: " << result_std.first << std::endl;
                                  assert(false);
            }
            results_file << type << "_STD, " << N << ", " << num_threads << ", " << result_std.second.count() << "\n";

            // auto result_omp1 = ParallelSum<T>::parallel_sum_omp1(data.data(), N, num_threads);
            // assert(result_omp1.first == N * (N + 1) / 2); // Verify the result is correct
            // results_file << type << "_OMP1, " << N << ", " << num_threads << ", " << result_omp1.second.count() << "\n";

            // auto result_omp_builtin = ParallelSum<T>::parallel_sum_omp_builtin(data.data(), N, num_threads);
            // assert(result_omp_builtin.first == N * (N + 1) / 2); // Verify the result is correct
            // results_file << type << "_OMP_BUILTIN, " << N << ", " << num_threads << ", " << result_omp_builtin.second.count() << "\n";

            // results_file.flush();
        }
    }
}

template<typename T>
void runTestsWeakScaling(const std::string& type, size_t base_N, size_t base_threads, size_t max_multiplier, std::ofstream& results_file) {
    for (size_t multiplier = 1; multiplier <= max_multiplier; ++multiplier) {
        size_t N = base_N * multiplier;  //increase n proportionally
        size_t num_threads = base_threads * multiplier;  //increase threads proportionally

        std::vector<T> data(N);
        std::iota(data.begin(), data.end(), 1);  // Fill with 1 to N

        auto result_std = ParallelSum<T>::parallel_sum_std(data.data(), N, num_threads);
        // auto result_std = ParallelSum<T>::parallel_sum_omp1(data.data(), N, num_threads);
        // auto result_std = ParallelSum<T>::parallel_sum_omp_builtin(data.data(), N, num_threads);
        if (result_std.first != T(N) * (N + 1) / 2) {
            std::cerr << "Failed for N = " << N << " with num_threads = " << num_threads
                      << ". Expected: " << T(N) * (N + 1) / 2 << ", Got: " << result_std.first << std::endl;
            assert(false);
        }
        results_file << type << ", " << N << ", " << num_threads << ", " << result_std.second.count() << "\n";

        // results_file.flush();
    }
}


int main() {
    std::ofstream results_file("results.csv");
    results_file << "Function, N, Threads, Time\n";

    const std::vector<size_t> N_values_strong = {10000, 100000, 1000000}; // N values for strong scaling
    const std::vector<size_t> N_values_weak = {10000, 20000, 40000, 80000}; // Adjusted for weak scaling
    const std::vector<size_t> thread_counts = {1, 2, 4, 8,16}; // Number of threads

    //Strongscaling test
    runTests<long long>("StrongScaling", N_values_strong, thread_counts, results_file);

    //vase values for N and initial number of threads
    size_t base_N = 10000;
    size_t base_threads = 1;  //1-16
    size_t max_multiplier = 16;

    //WeakScaling test
    // runTestsWeakScaling<long long>("WeakScaling", base_N, base_threads, max_multiplier, results_file);



    results_file.close();
    std::cout << "Tests completed. Results saved to results.csv\n";
    return 0;
}

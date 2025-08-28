
#include "ParallelSum.h"
#include <thread>


template<typename T>
class ParallelSum {
public:

    static std::pair<T, std::chrono::duration<double>> parallel_sum_std(T a[], size_t N, size_t num_threads);
    static std::pair<T, std::chrono::duration<double>> parallel_sum_omp1(T a[], size_t N, size_t num_threads);
    static std::pair<T, std::chrono::duration<double>> parallel_sum_omp_builtin(T a[], size_t N, size_t num_threads);
};

template<typename T>
//sum of array and time ot took
std::pair<T, std::chrono::duration<double>> ParallelSum::parallel_sum_std(T a[], size_t arraySize, size_t num_threads) {
    std::atomic<T> sum = 0;
    std::vector<std::thread> threads;

    //start the clock
    auto start = std::chrono::high_resolution_clock::now();
    for(size_t id = 0; id < num_threads; ++id) {
        threads.emplace_back([&, id]() {
            //create own local sum variable for each thread
            T localSum = 0;
            //each thread calculates its roughly equal portion of the array  for the sum later
            // (size of array / number of threads)
            size_t workPortion = arraySize / num_threads;
            //have id work on workPortion value from the iteration adding it to its local sum
            //until the thread has done it's portion of the work
            for(size_t j = id * (workPortion); j < (id + 1) * (workPortion); ++j) {
                localSum += a[j];
            }
            //reduce into a global sum
            sum.fetch_add(localSum, std::memory_order_relaxed);
        });
    }
    for(auto &thread : threads) {
        thread.join();
    }
    //end the clock
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;
    //return the sum of the threads work and the time it took to finish
    return {sum, diff};
}


template<typename T>
std::pair<T, std::chrono::duration<double>> ParallelSum::parallel_sum_omp1(T a[], size_t arraySize, size_t num_threads) {
    T sum = 0;
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
        sum += localSum;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;
    return {sum, diff};
}

template<typename T>
std::pair<T, std::chrono::duration<double>> ParallelSum::parallel_sum_omp_builtin(T a[], size_t N, size_t num_threads) {
    T sum = 0;
    omp_set_num_threads(num_threads);
    auto start = std::chrono::high_resolution_clock::now();

#pragma omp parallel for reduction(+:sum)
    for(size_t i = 0; i < N; ++i) {
        sum += a[i];
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;
    return {sum, diff};
}

// template class ParallelSum::parallel_sum_std<int>;
// template class ParallelSum::parallel_sum_std<float>;
// template class ParallelSum::parallel_sum_std<double>;
//
// template class ParallelSum::parallel_sum_omp1<int>;
// template class ParallelSum::parallel_sum_omp1<float>;
// template class ParallelSum::parallel_sum_omp1<double>;
//
// template class ParallelSum::parallel_sum_omp_builtin<int>;
// template class ParallelSum::parallel_sum_omp_builtin<float>;
// template class ParallelSum::parallel_sum_omp_builtin<double>;

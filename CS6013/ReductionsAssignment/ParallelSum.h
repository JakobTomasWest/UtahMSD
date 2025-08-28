//
// Created by Jakob West on 4/15/24.
//

#ifndef REDUCTIONSASSIGNMENT_PARALLELSUM_H
#define REDUCTIONSASSIGNMENT_PARALLELSUM_H
#include <chrono>
#include <vector>
#include <atomic>
#include <omp.h>


class ParallelSum {
public:
    template<typename T>
    static std::pair<T, std::chrono::duration<double>> parallel_sum_std(T a[], size_t N, size_t num_threads);

    template<typename T>
    static std::pair<T, std::chrono::duration<double>> parallel_sum_omp1(T a[], size_t N, size_t num_threads);

    template<typename T>
    static std::pair<T, std::chrono::duration<double>> parallel_sum_omp_builtin(T a[], size_t N, size_t num_threads);
};

#endif //REDUCTIONSASSIGNMENT_PARALLELSUM_H

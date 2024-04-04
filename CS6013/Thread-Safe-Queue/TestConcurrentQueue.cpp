//
// Created by Jakob West on 4/3/24.
//
#include "ConcurrentQueue.h"
#include <iostream>
#include <vector>
#include <thread>
template<typename T>
void producer(ConcurrentQueue <T>& queue, int num_ints){
    for( int i =0; i < num_ints; ++i){
        queue.enqueue(i);
    }
}

template<typename T>
void consumer(ConcurrentQueue <T>& queue, int num_ints){
    T item;
    for( int i =0; i< num_ints; ++i){
        //wait for an item to dequeue, keep trying
        while(!queue.dequeue(&item)){
            this_thread::yield();
        }
    }
}
bool testQueue( int num_producers, int num_consumers, int num_ints ) {
    //persists for the scope of the function
    ConcurrentQueue<int> queue;
    std::vector<std::thread> threads;
        threads.reserve(num_producers + num_consumers);

    // Create producer threads
    for (int i = 0; i < num_producers; ++i) {
        threads.emplace_back(producer<int>, std::ref(queue), num_ints);

    }

    //create consumer threads
    for( int i =0; i < num_consumers; ++i){
        threads.emplace_back(consumer<int>, std::ref(queue), num_ints);
    }

    // Wait for all threads to finish
    for (auto& thread : threads) {
        thread.join();
    }
    // Check if the number of elements in the queue matches the expected value
    return queue.size() == (num_producers - num_consumers) * num_ints;
    }

int main(int argc, char **argv) {

    if( argc != 4){
        std::cerr << "Usage: " << argv[0] << " <num_producers> <num_consumers> <num_ints>\n";
        return 1;
    }
    int num_producers = std::stoi(argv[1]);
    int num_consumers = std::stoi(argv[2]);
    int num_ints = std::stoi(argv[3]);

    bool result = testQueue(num_producers, num_consumers, num_ints);
    if (result) {
        std::cout << "Test passed.\n";
    } else {
        std::cout << "Test failed.\n";
    }
    return 0;
}

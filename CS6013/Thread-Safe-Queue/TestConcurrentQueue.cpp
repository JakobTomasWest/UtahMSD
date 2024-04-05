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
    //reserve space in this vector for all producer and consumer threads
    threads.reserve(num_producers + num_consumers);

    //create producer threads
    for (int i = 0; i < num_producers; ++i) {
        threads.emplace_back(producer<int>, std::ref(queue), num_ints);

    }

    //create consumer threads
    for( int i =0; i < num_consumers; ++i){
        threads.emplace_back(consumer<int>, std::ref(queue), num_ints);
    }

    //wait for all threads to join (ie,finish).
    for (auto& thread : threads) {
        thread.join();
    }
    //check if the number of elements in the queue matches the expected value
    return queue.size() == (num_producers - num_consumers) * num_ints;
    }

int main(int argc, char **argv) {
    //   /.Thread_Safe_Queue num_producers num_consumers num ints
    if( argc != 4){
        std::cerr << "Usage: " << argv[0] << " <num_producers> <num_consumers> <num_ints>\n";
        return 1;
    }
    //take string arguments and convert them into integers for teh Queuing Test
    int num_producers = stoi(argv[1]);
    int num_consumers = stoi(argv[2]);
    int num_ints = stoi(argv[3]);

    bool result = testQueue(num_producers, num_consumers, num_ints);
    if (result) {
        cout << "Test passed.\n";
    } else {
        cout << "Test failed.\n";
    }
    return 0;
}

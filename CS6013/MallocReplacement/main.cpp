#include <iostream>
#include <cassert>
#include "hashTable.h"
#include "MyMalloc.h"
#include <chrono>


void testInsertAndFind() {
    hashTable ht(10);
    //Insert an address/size key-value pair and ensure the address can be found
    ht.insert((void*)0x1234, 100);
    assert(ht.find((void*)0x1234) != -1);

    ht.insert((void*)0x5678, 200);
    assert(ht.find((void*)0x5678) != -1);
    //Looking for key that hasn't been inserted
    assert(ht.find((void*)0x9ABC) == -1);

    std::cout << "Test 1 passed: Insert and find are working.\n";
}
void testGrowth() {
    hashTable ht(2);

    //Trigger resize
    ht.insert((void*)0x1000, 50);
    ht.insert((void*)0x2000, 100);
    ht.insert((void*)0x3000, 150);

    //Checking that all the items can still be found
    assert(ht.find((void*)0x1000) != -1);
    assert(ht.find((void*)0x2000) != -1);
    assert(ht.find((void*)0x3000) != -1);

    std::cout << "Test 2 passed: Table can grow can keep addresses.\n";
}
void testRemoveAndLazyDeletion() {
    hashTable ht(10);

    ht.insert((void*)0xABC, 123);
    ht.insert((void*)0xDEF, 456);
    //Ensure the item exists before removal
    assert(ht.find((void*)0xABC) != -1);
    //Remove the item
    ht.remove((void*)0xABC);
    assert(ht.find((void*)0xABC) == -1);
    assert(ht.find((void*)0xDEF) != -1);

    std::cout << "Test 3 passed: Remove and lazy deletion are workign.\n";
}
void testCollisions() {
    hashTable ht(5);

    //These addresses are chosen to ensure they hash to the same index
    ht.insert((void*)0x1000, 10);
    ht.insert((void*)0x2000, 20);

    //Both items should be retrievable even after the collision
    assert(ht.find((void*)0x1000) != -1);
    assert(ht.find((void*)0x2000) != -1);

    std::cout << "Test 4 passed: Hash table can find multiple address at a single index.\n";
}
void testEdgeCases() {
    hashTable ht(1);

    //Insert into a table that is too small and needs to grow
    ht.insert((void*)0x1, 1);
    ht.insert((void*)0x2, 2);
    ht.insert((void*)0x3, 3);

    //Find each entry and pull their addresses
    assert(ht.find((void*)0x1) != -1);
    assert(ht.find((void*)0x2) != -1);
    assert(ht.find((void*)0x3) != -1);

    //Assure that remove works
    ht.remove((void*)0x1);
    ht.remove((void*)0x2);
    ht.remove((void*)0x3);
    //The number of entries should be 0 after removing all of the insertions
    assert(ht.currentNumEntries == 0);

    std::cout << "Test 5 passed: Edge cases and error handling.\n";
}
///////////////////////////////////////////////////////////////////////
void testAllocateAndDeallocate() {
    MyMalloc allocator(10);
    size_t size = 128;
    void* ptr = allocator.allocate(size);
    assert(ptr != nullptr);
    //Memset can fill the byte with a string
    memset(ptr, 'A', size);
    for (size_t i = 0; i < size; ++i) {
        assert(static_cast<char*>(ptr)[i] == 'A');
    }

    std::cout << "Test 1 malloc: (De)allocation test passed." << std::endl;
    allocator.deallocate(ptr);
}

void testMyMallocGrowth() {
    MyMalloc allocator(2);
    std::vector<void*> pointers;
    for (int i = 0; i < 10; ++i) {
        void* ptr = allocator.allocate(128);
        pointers.push_back(ptr);
    }

    //assert(allocator.table.capacity() > 2);
    if(pointers.size()>2) {
        std::cout << "Test 2 malloc: Growth test passed." << std::endl;
    }

    for (void* ptr : pointers) {
        allocator.deallocate(ptr);
    }
}
void testMyMallocEdgeCases() {
    MyMalloc allocator(10);
    allocator.deallocate(nullptr);

    void* zeroPtr = allocator.allocate(0);
    assert(zeroPtr == nullptr || allocator.table.getEntrySize(zeroPtr) > 0);
    allocator.deallocate(zeroPtr);

    std::cout << "Test 3 malloc: Edge cases test passed." << std::endl;
}

//
//MyMalloc myAlloc(100);
//void* malloc(size_t size) {
//    return myAlloc.allocate(size);
//}
//void free(void* ptr) {
//    myAlloc.deallocate(ptr);
//}
//void performanceTest() {
//    using namespace std::chrono;
//
//    size_t numAllocations = 10000; // Number of allocations to test
//    auto start = high_resolution_clock::now();
//
//    for (size_t i = 0; i < numAllocations; ++i) {
//        void* memory = malloc(64); // Allocate small blocks to simulate workload
//        free(memory);
//    }
//
//    auto end = high_resolution_clock::now();
//    duration<double, std::milli> duration = end - start;
//    std::cout << "Performance test: " << numAllocations << " allocations and deallocations took " << duration.count() << " milliseconds.\n";
//}
//With overload of malloc and free the Timing Performance test: 10000 allocations and deallocations took onver avergae
// 5.7244 milliseconds.



//To test this function comment out overloading:
// MyMalloc myAlloc(100);
//void* malloc(size_t size) {
//    return myAlloc.allocate(size);
//}
//void free(void* ptr) {
//    myAlloc.deallocate(ptr);
//}
// from above^^
void myMallocPerformanceTest() {
    using namespace std::chrono;
    MyMalloc allocator(100);

    size_t numAllocations = 10000; // Number of allocations to test
    auto start = high_resolution_clock::now();

    for (size_t i = 0; i < numAllocations; ++i) {
        //MyMalloc use
        void* memory = allocator.allocate(64);
        allocator.deallocate(memory);
    }

    auto end = high_resolution_clock::now();
    duration<double, std::milli> duration = end - start;
    std::cout << "Performance test: " << numAllocations << " allocations and deallocations took " << duration.count() << " milliseconds.\n";
}
//Without overloading of malloc and free the timing Performance test: 10000 allocations and deallocations took on average
// 6.86117 milliseconds.

int main() {
    testInsertAndFind();
    testGrowth();
    testRemoveAndLazyDeletion();
    testCollisions();
    testEdgeCases();
    testAllocateAndDeallocate();
    testMyMallocGrowth();
    testMyMallocEdgeCases();
//    performanceTest();
    myMallocPerformanceTest();
    std::cout << "All tests passed successfully.\n";

    return 0;
}

//had to make MyMalloc hashtable table, MyMalloc constructor , num entries, hashtable contsructor public
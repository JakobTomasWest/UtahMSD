//
// Created by Jakob West on 3/14/24.
//


#ifndef MALLOCREPLACEMENT_MYMALLOC_H
#define MALLOCREPLACEMENT_MYMALLOC_H
#include "hashTable.h"

class MyMalloc {
private:
    hashTable table;

public:
    explicit MyMalloc(size_t initialCapacity);
    void* allocate(size_t size);
    void deallocate(void* addressptr);
};


#endif //MALLOCREPLACEMENT_MYMALLOC_H

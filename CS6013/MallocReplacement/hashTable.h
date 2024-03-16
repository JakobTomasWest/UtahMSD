//
// Created by Jakob West on 3/13/24.
//

#ifndef MALLOCREPLACEMENT_HASHTABLE_H
#define MALLOCREPLACEMENT_HASHTABLE_H


#include <cstdlib>

struct HashEntry {
    //The basis of the hashtable is just a giant array, we are storing the memory address and the size
    void* addressptr;
    size_t size;
    bool used;
    bool deleted;
};

class hashTable {
private:
    //Pointer to the array of hash entries
    HashEntry* table;
    //Total capacity of the hash table
    size_t capacity;

    size_t hashFunction(void* addressptr) const;
    void grow();


public:
    size_t getEntrySize(void* addressptr);
    //Destructor to deallocate the table
    ~hashTable();
    void insert(void* addressptr, size_t size);
    void remove(void* addressptr);
    size_t find(void *addressptr);

    explicit hashTable(size_t initialCapacity);

    //Current number of elements in the hash table
    size_t currentNumEntries;
};

#endif //MALLOCREPLACEMENT_HASHTABLE_H

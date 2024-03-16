//
// Created by Jakob West on 3/13/24.
//Creating hashTable class for custom memory allocation
#include <cstdio>
#include <cassert>
#include <iostream>
#include "hashTable.h"
#include "sys/mman.h"

//hashTable constructor used for setting initial capacity, allocating memory, and initializing hash table entries as unused.
hashTable::hashTable(size_t initialCapacity) {
    capacity = initialCapacity;
    currentNumEntries = 0;
    //Calculate the size needed for the initial capacity
    size_t size = initialCapacity * sizeof(HashEntry);
    //Allocate memory using mmap
    table = (HashEntry*)mmap(
            //Let the OS choose the address
            nullptr,
            //Size of memory to map
            size,
            //Enable read and write
            PROT_READ | PROT_WRITE,
            //Memory is not backed by any file and is private to this process
            MAP_PRIVATE | MAP_ANONYMOUS,
            //No Fd, no offset
            -1,
            0
    );

    //Check for mmap failure
    if (table == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }
    //Initialize all entries as un-used
    for (size_t i = 0; i < initialCapacity; i++) {
    this->table[i].used = false;
    }
}
//The hash function takes whatever the key is and gets the index into our array from the address pointer
size_t hashTable::hashFunction(void *addressptr) const {
    return ((size_t)addressptr >> 12) % capacity;
}
//The purpose of the function is to assign a value of bytes to our key: the address we are pointing to --> within our hashTable
void hashTable::insert(void *addressptr, size_t size) {
    //Check to see if we have to grow the hashtable first
    float loadFactor = static_cast<float>(currentNumEntries + 1) / capacity;
    if (loadFactor > 0.70) {
        grow();
    }
    size_t index = hashFunction(addressptr);

    bool isNewInsertion = false;
    //While the current index with the table is marked as used, linearly probe until we find
    //an open index and wrap to the start of the table when we reach table capacity
    while(table[index].used && !table[index].deleted){
    index = (index + 1) % capacity;
    }
    //Check if this slot was previously unused - if so, it's a new insertion
    isNewInsertion = !table[index].used;

    //Insert new address ptr and size of block
    table[index].addressptr = addressptr;
    table[index].size = size;
    table[index].used = true;
    table[index].deleted = false;

    if (isNewInsertion) {
        currentNumEntries++;
    }
}
//Using the address pointer look for the address in the hashTable and return its index
size_t hashTable::find(void *addressptr){
    //Look for particular address in hashtable
    size_t index = hashFunction(addressptr);
    //Mark this as the beginning to create an end point in our circular-array search
    size_t start = index;
    while (table[index].used || table[index].deleted) {
        if (table[index].used && !table[index].deleted && table[index].addressptr == addressptr) {
            //Found address
            return index;
        }
        index = (index + 1) % capacity;
        if (index == start)
            //Address not found after going through the whole table
            return -1;
    }
    return -1;
}
//Function to find and remove an address using the pointer for the index within our hashtable
void hashTable::remove(void *addressptr) {
    size_t index = find(addressptr); //Can return -1 if not found

    if (index != -1) { //Key found
        table[index].used = false;
        table[index].deleted = true;
        currentNumEntries--; //Decrease count after removing the item
    }
}
//Function to increase the virtual address space available by doubling our hashTable size
void hashTable::grow() {
    size_t newCapacity = capacity *2;
    //Declare the total amount of memory we want to request mmap
    size_t newSize = newCapacity * sizeof(HashEntry);
    //Get pointer to start the newly allocated memory
    HashEntry* newTable = (HashEntry*)mmap(
            //The OS chooses initial address to place memory with nullptr
            nullptr, newSize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (newTable == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    //Rehash the entries from the old table to the new one
    for (size_t i = 0; i < capacity; i++) { //Compute new hash index for the current entry
        if (table[i].used && !table[i].deleted) {
            //Get address at current index of old table
            size_t newIndex = hashFunction(table[i].addressptr) % newCapacity;
            //Find a new spot in the new table if needed
            while (newTable[newIndex].used) {
                newIndex = (newIndex + 1) % newCapacity; //Will the old modulo be am issue?
            }

            //Insert the entry into the new table
            //Directly copy the whole entry
            newTable[newIndex] = table[i];
            //Ensure the entry is marked as used
            newTable[newIndex].used = true;
            //Ensure the entry is not marked as deleted
            newTable[newIndex].deleted = false;
        }
    }
    //Unmap the old table
    if (munmap(table, capacity * sizeof(HashEntry)) == -1) {
        perror("munmap");
        exit(EXIT_FAILURE);
    }
    //Update the table's properties
    table = newTable;
    capacity = newCapacity;

}
//DESTROY
hashTable::~hashTable() {
    if (munmap(table, capacity * sizeof(HashEntry)) == -1) {
        perror("munmap");
    }
}
//Using the address look at the within a particular index of or hashTable
size_t hashTable::getEntrySize(void* addressptr) {
    //First find the index of teh address to retrieve the size
    size_t index = find(addressptr);
    //Make sure the address was found.
    if (index != (size_t)-1) {
        //Return the size associated with the address.
        return table[index].size;
    }
    //Address DNE
    return 0;
}





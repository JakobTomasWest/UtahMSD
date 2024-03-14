//
// Created by Jakob West on 3/14/24.
//
#include <sys/mman.h>
#include <cstdio>

#include "MyMalloc.h"

MyMalloc::MyMalloc(size_t initialCapacity) : table(initialCapacity) {
}

void *MyMalloc::allocate(size_t size) {
    //Use mmap to allocate memory
    void* addr = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (addr == MAP_FAILED) {
        return nullptr;
    }

    //Store the address and size in the hash table
    table.insert(addr, size);
    //Return the allocated memory address
    return addr;
}

void MyMalloc::deallocate(void *addressptr) {
    //Find the size associated with this address
    size_t size = table.getEntrySize(addressptr);
    //if size is 0 then address isn't found or it has no size
    if (size == 0) {
        return;
    }
    //Use munmap to deallocate the memory
    if (munmap(addressptr, size) == -1) {
        perror("munmap"); // Handle error in munmap
    }

    //Remove the entry from the hash table
    table.remove(addressptr);
}

#include <iostream>
#include <cassert>
#include "hashTable.h"


void testInsertAndFind() {
    hashTable ht(10);

    ht.insert((void*)0x1234, 100); // Insert a key-value pair
    assert(ht.find((void*)0x1234) != -1); // Ensure the key can be found

    ht.insert((void*)0x5678, 200); // Insert another key-value pair
    assert(ht.find((void*)0x5678) != -1); // Ensure the second key can also be found

    assert(ht.find((void*)0x9ABC) == -1); // Look for a key that was not inserted

    std::cout << "Test 1 passed: Insert and find functionality works.\n";
}
void testGrowth() {
    hashTable ht(2); // Start with a very small capacity to force growth

    // Insert multiple items to trigger a resize
    ht.insert((void*)0x1000, 50);
    ht.insert((void*)0x2000, 100);
    ht.insert((void*)0x3000, 150);

    // Check that all items can still be found post-resize
    assert(ht.find((void*)0x1000) != -1);
    assert(ht.find((void*)0x2000) != -1);
    assert(ht.find((void*)0x3000) != -1);

    std::cout << "Test 2 passed: Table growth maintains data integrity.\n";
}
void testRemoveAndLazyDeletion() {
    hashTable ht(10);

    ht.insert((void*)0xABC, 123);
    ht.insert((void*)0xDEF, 456);
    assert(ht.find((void*)0xABC) != -1); // Ensure the item exists before removal

    ht.remove((void*)0xABC); // Remove an item
    assert(ht.find((void*)0xABC) == -1); // Ensure the item no longer exists
    assert(ht.find((void*)0xDEF) != -1); // Ensure other items are unaffected

    std::cout << "Test 3 passed: Removal and lazy deletion work as expected.\n";
}
void testCollisions() {
    hashTable ht(5); // Small table to increase likelihood of collisions

    // These addresses are chosen to ensure they hash to the same index
    // This might require adjustment based on your hash function
    ht.insert((void*)0x1000, 10);
    ht.insert((void*)0x2000, 20);

    // Both items should be retrievable despite the collision
    assert(ht.find((void*)0x1000) != -1);
    assert(ht.find((void*)0x2000) != -1);

    std::cout << "Test 4 passed: Hash table handles collisions correctly.\n";
}
void testEdgeCases() {
    hashTable ht(1); // Smallest possible table

    // Insert into a table that will definitely need to grow
    ht.insert((void*)0x1, 1);
    ht.insert((void*)0x2, 2);
    ht.insert((void*)0x3, 3);

    // Ensure growth happened and items are still retrievable
    assert(ht.find((void*)0x1) != -1);
    assert(ht.find((void*)0x2) != -1);
    assert(ht.find((void*)0x3) != -1);

    // Remove all items and check the count
    ht.remove((void*)0x1);
    ht.remove((void*)0x2);
    ht.remove((void*)0x3);
    assert(ht.currentNumEntries == 0); // Ensure the count of entries is accurate after removals

    std::cout << "Test 5 passed: Edge cases and error handling are properly managed.\n";
}


int main() {
    testInsertAndFind();
    testGrowth();
    testRemoveAndLazyDeletion();
    testCollisions();
    testEdgeCases();

    std::cout << "All tests passed successfully.\n";


    return 0;
}

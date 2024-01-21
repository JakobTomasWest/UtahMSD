#include "lab1.c"
#include <stdio.h>
int main() {

   u_int8_t testArray[] = {9,7,5,4,6,2,1,4};
   int n = 7;
   sort(testArray,8);
   printf("Sorted array: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", testArray[i], "\n");
    }
   
   u_int64_t test = 0x0403deadbeef0201;
   u_int64_t sorted_value = byte_sort(test);
   printf("Sorted value: 0x%llx\n", sorted_value);

   u_int64_t nibblesSortedValue = nibble_sort(test);
   printf("Sorted value: 0x%llx\n", nibblesSortedValue);

   return 0;
}

/*********************************************************************
 *
 * File header comments go here... including student name, class, date, 
 * assignment, description, etc.
 *  Jakob Tomas West, u0675702, CS6013, 1/8/2024, lab01
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h> // For strlen()

/*********************************************************************
 *
 * The C functions in this lab use patterns and functionality often found in
 * operating system code. Your job is to implement them.  Additionally, write some test
 * cases for each function (stick them in functions [called xyzTests(), etc or similar])
 * Call your abcTests(), etc functions in main().
 *
 * Write your own tests for each function you are testing, then share/combine
 * tests with a classmate.  Try to come up with tests that will break people's code!
 * Easy tests don't catch many bugs! [Note this is one specific case where you may
 * share code directly with another student.  The test function(s) from other students
 * must be clearly marked (add '_initials' to the function name) and commented with the
 * other student's name.
 *
 * Note: you may NOT use any global variables in your solution.
 * 
 * Errata:
 *   - You can use global vars in your testing functions (if necessary).
 *   - Don't worry about testing the free_list(), or the draw_me() functions.
 *
 * You must compile in C mode (not C++).  If you compile from the commandline
 * run clang, not clang++. Submit your solution files on Canvas.
 *
 *********************************************************************/

/*********************************************************************
 *
 * byte_sort()
 *
 * specification: byte_sort() treats its argument as a sequence of
 * 8 bytes, and returns a new unsigned long integer containing the
 * same bytes, sorted numerically, with the smaller-valued bytes in
 * the lower-order byte positions of the return value
 * 
 * EXAMPLE: byte_sort (0x0403deadbeef0201) returns 0xefdebead04030201
 * Ah, the joys of using bitwise operators!
 *
 * Hint: you may want to write helper functions for these two functions
 *
 *********************************************************************/

//helper method used to sort each of the bytes in ascending order
//it takes an array of 8 byte values and a size n 
void sort(uint8_t arr[], int n){
  int i,j;
  u_int8_t temp;
  //creating bubble sort to compare each bit set with the next 
  for(int i = 0; i < n-1; i++){
    //through each iteration of i, we decrease by 1 comparision 
    for(int j = 0; j <n-i-1;j++){
      //to get ascending order, if current value is greater than next value, swap them
      if( arr[j] > arr[j+1]){
        temp = arr[j];
        arr[j] = arr[j+1];
        arr[j+1] = temp;
      }
    }
  }

}

unsigned long byte_sort( unsigned long arg )
{
  u_int8_t bytes[8];
  //Take out each byte set of the array for sorting
  for(int i=0; i < 8; i++){
    //use the particular set with i position and shift it so we can use 0xFF to take each byte we need
    bytes[i] = (arg >>(8*i)) & 0xFF;
  }
  sort(bytes, 8);
  //recombine each byte set to form 8byte value with largest to smallest values - order
  uint64_t sortedULong = 0;
  for( int i = 0; i < 8; i ++){
    //cast so you can append uint8_t values
    sortedULong |= ((uint64_t)(bytes[i])) << (8 * i);
  }

  return sortedULong;
}

/*********************************************************************
 *
 * nibble_sort()
 *
 * specification: nibble_sort() treats its argument as a sequence of 16 4-bit
 * numbers, and returns a new unsigned long integer containing the same nibbles,
 * sorted numerically, with smaller-valued nibbles towards the "small end" of
 * the unsigned long value that you return
 *
 * the fact that nibbles and hex digits correspond should make it easy to
 * verify that your code is working correctly
 * 
 * EXAMPLE: nibble_sort (0x0403deadbeef0201) returns 0xfeeeddba43210000
 *
 *********************************************************************/

unsigned long nibble_sort (unsigned long arg)
{
  u_int8_t nibbles[16];
  
  for(int i =0; i < 16; i++){
    nibbles[i] = (arg >> 4*i) & 0xF;
  }
  sort(nibbles, 16);

  uint64_t sortedUlong = 0;
  for(int i = 0; i < 16; i++){
    sortedUlong |= ((u_int64_t)nibbles[i]) << (4*i);
  }

  return sortedUlong;
}

/*********************************************************************/

typedef struct elt {
  char val;
  struct elt *link;
} Elt;

/*********************************************************************/

/* Forward declaration of "free_list()"... This allows you to call   */
/* free_list() in name_list() [if you'd like].                       */
void free_list( Elt* head ); // [No code goes here!]

/*********************************************************************
 *
 * name_list()
 *
 * specification: allocate and return a pointer to a linked list of
 * struct elts
 *
 * - the first element in the list should contain in its "val" field the first
 *   letter of your first name; the second element the second letter, etc.;
 *
 * - the last element of the linked list should contain in its "val" field
 *   the last letter of your first name and its "link" field should be a null
 *   pointer
 *
 * - each element must be dynamically allocated using a malloc() call
 * 
 * - you must use the "name" variable (change it to be your name).
 *
 * Note, since we're using C, not C++ you can't use new/delete!
 * The analog to delete is the free() function
 *
 * - if any call to malloc() fails, your function must return NULL and must also
 *   free any heap memory that has been allocated so far; that is, it must not
 *   leak memory when allocation fails
 *
 * Implement print_list and free_list which should do what you expect.
 * Printing or freeing a nullptr should do nothing.
 *
 * Note: free_list() might be useful for error handling for name_list()... 
 *
 *********************************************************************/

Elt *name_list()
{
   char * name = "Davison";
   return NULL;
}

/*********************************************************************/

void print_list( Elt* head )
{
}

/*********************************************************************/

void free_list( Elt* head )
{
}

/*********************************************************************
 *
 * draw_me()
 *
 * This function creates a file called 'me.txt' which contains an ASCII-art
 * picture of you (it does not need to be very big).
 * 
 * Use the C stdlib functions: fopen, fclose, fprintf, etc which live in stdio.h
 * - Don't use C++ iostreams
 *
 *********************************************************************/

void draw_me()
{
}

/*********************************************************************
 *
 * Test Code - Place your test functions in this section:
 */

// bool testByteSort() { ... }
// ...
// ...

/*********************************************************************
 *
 * main()
 *
 * The main driver program.  You can place your main() method in this
 * file to make compilation easier, or have it in a separate file.
 *
 *
 *********************************************************************/

// int main()
// {
//    // Call your test routines here...
// }

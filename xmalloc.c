#include <stdio.h>
#include <stdlib.h>
#include "xmalloc.h"

/*
*** Part 3 (18 points):

Write your own simple malloc-debugging "library" (a .c file you can compile
and link with another program).  This library should export AT LEAST these
two functions: xmalloc() and xfree(), which should have the same
functionality as the usual malloc/free functions.  In turn, xmalloc and
xfree should use malloc and free, respectively.

xmalloc and xfree should track what memory objects are being allocated and
freed, and should warn when these conditions occur: double free, double
malloc (a memory leak), trying to free something in the middle of an
allocated memory chunk, and memory leaks.  By "warn" I mean that they should
"fprintf(stderr)" a descriptive error message saying what went wrong
(optionally, you can also exit the program with an appropriate non-zero exit
status).  Hint: think about the API of your library and how you would detect
leaks or any other unfreed memory (extend the API as needed, but keep it
simple).

Your debugging library should be put into a file called xmalloc.c.  Next,
write one or more programs that link with your xmalloc.o and then exercise
each of the "bad things" that your library catches.

Your code should build and run on both a-centos58 and a-solaris9.
*/

//struct for memory chunks, hold an address, size of chunk, prev and Next
struct memory_chunk {
  void *address;
  size_t size;
  struct memory_chunk* prev;
  struct memory_chunk* next;
};//struct memory_chunk

//track malloc memory chunks using a doubly-linked-list, SLL may be appropriate, will optimize if time permits
static struct memory_chunk *HEAD; //head of our list, aka entry point
static int length = 0; //current size of list
static int exitflag = 0; //flag to ensure atexit is only called once.

/* allocatedMemory
* check the list to make sure there are no unfree'd chunks, if there are report the amount of memory leaked.
*/
void allocatedMemory() {
  int leakCount = 0;
  int leakTotal = 0;
  struct memory_chunk *curr = HEAD;
  while(curr != NULL) {
    ++leakCount;
    leakTotal = curr->size;
    curr = curr->next;
  }//curr exists
  fprintf(stderr, "There was a total of %d memory leaks totaling %d bytes lost\n", leakCount, leakTotal);

}//allocatedMemory

/* xmalloc
 * create a memory chunk struct and add to our doubly-linked-list
 * malloc space and return pointer
 * special case: empty list
*/
void *xmalloc(size_t size) {
  //atexit, force perform memory leak check on program exit
  if(!exitflag) {
    atexit(allocatedMemory);
    exitflag = 1;
  } //!exitflag
  //special case, empty list, create head and memory_chunk, assign
  if(length == 0) {
    HEAD = (struct memory_chunk*) malloc(sizeof(struct memory_chunk));
    if(HEAD == NULL) {
      fprintf(stderr, "Error with struct malloc, returned NULL\n");
      return NULL;
    }
    HEAD->prev = NULL;
    HEAD->next = NULL;
    HEAD->address = malloc(size);
    if(HEAD->address == NULL) {
      fprintf(stderr, "Error with malloc, returned NULL\n");
    }
    HEAD->size = size;
    ++length;
    return HEAD->address;
  }//len = 0, empty list

  //else, create node then standard node insertion
  struct memory_chunk* node = (struct memory_chunk*) malloc(sizeof(struct memory_chunk));
  if(node == NULL) {
    fprintf(stderr, "Error with struct malloc, returned NULL\n");
    return NULL;
  }
  node->prev = NULL;
  node->next = NULL;
  node->address = malloc(size);
  if(node->address == NULL) {
    fprintf(stderr, "Error with malloc, returned NULL\n");
  }
  node->size = size;
  //insertion
  struct memory_chunk *curr = HEAD;
  while(curr->next != NULL) {
    curr = curr->next;
  }//curr-next != NULL
  //at last node, attach new node to last
  curr->next = node;
  node->prev = curr;
  ++length;
  //return address
  return node->address;
}//xmalloc

/* xfree
* free memory_chunk from our DLL
* we expect specific failure report and exit if the memory_chunk doesn't exist in the DLL
* speical case 1: illegal address
* special case 2: DLL is empty (never allocated memory, or double frees)
* otherwise look in list for pointer, remove if it exists, otherwise double free/never alloc
*/
void xfree(void *ptr) {
  if(ptr <= 0) {
    fprintf(stderr, "Failure: Illegal address, unable to free\n");
    return;
  }//ptr is illegal
  if(!length) {
    fprintf(stderr, "Failure: Possible double free or never allocated space\n");
    return;
  }//DLL is empty

  //walk DLL, look for pointer
  struct memory_chunk *curr = HEAD;
  while(curr != NULL) {
    if(curr->address == ptr) { //curr == address to be found, break loop
      break;
    } else if(ptr < curr->address + curr->size && ptr > curr->address) { //ptr is largers than the current address but smaller than address + allocated size
      //in the middle of memory chunks
      fprintf(stderr, "Failure: Requesting freeing of memory in middle of memory chunk\n");
      return;
    } else {
      //curr != ptr, move to next node in DLL
      curr = curr->next;
    }
  }//curr exists

  if(curr == NULL) {
    fprintf(stderr, "Failure: Possible double free or never allocated space\n");
    return;
  }//ptr doesn't exist in DLL

  //else DLL remove node algorithm
  //case1: the address is at the head of DLL
  if(curr->prev == NULL) {
    HEAD = HEAD->next;
    if(HEAD != NULL) {
      HEAD->prev = NULL;
    }//HEAD!=NULL
  } else if(curr->next == NULL) { //case2: the address is the last element of the DLL
    curr->prev->next = NULL;
  } else { //case3: address is in the middle of the list
    struct memory_chunk *next = curr->next;
    struct memory_chunk *prev = curr->prev;
    prev->next = next;
    next->prev = prev;
  }
  //free the struct space for the list and free the requested memory by user
  free(curr);
  free(ptr);
  --length;
}//xfree

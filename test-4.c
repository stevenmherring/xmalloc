#include <stdlib.h>
#include <stdio.h>
#include "xmalloc.h"
//Steven Herring
//intentionally causing a memory leak

int main() {
  printf("-------- Intentional Memory Leak Test Program -------- \n\n\n");
  int size = 256;
  int *p1 = xmalloc(sizeof(int) * size);
  int *p2 = xmalloc(sizeof(int) * size);
  xfree(p2);
  printf("%p\n", p1); //silence warning by using pointer.
  exit(0);
}//main

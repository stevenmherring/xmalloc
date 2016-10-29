#include <stdlib.h>
#include <stdio.h>
#include "xmalloc.h"
//Steven Herring
//intentionally causing a memory leak

int main() {
  printf("-------- Attempt to Free memory between different memory chunks of allocated space Test Program -------- \n\n\n");
  int size = 256;
  int *p1 = xmalloc(sizeof(int) * size * 2);
  xfree(p1 + size);
  xfree(p1);
  exit(0);
}//main

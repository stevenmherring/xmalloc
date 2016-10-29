#include <stdlib.h>
#include <stdio.h>
#include "xmalloc.h"
//Steven Herring
//intentionally causing a memory leak

int main() {
  printf("-------- Attempt to Free memory outside of allocated space Test Program -------- \n\n\n");
  int size = 256;
  int *p1 = xmalloc(sizeof(int) * size);
  xfree(p1 + size + size);
  xfree(p1);
  exit(0);
}//main

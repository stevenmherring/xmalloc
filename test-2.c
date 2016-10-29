#include <stdlib.h>
#include <stdio.h>
#include "xmalloc.h"
//Steven Herring
//testing a double malloc using xmalloc library which will cause a memory leak

int main() {
  printf("-------- Double Malloc to Memory Leak Test Program -------- \n\n\n");
  int size = 256;
  int *p = xmalloc(sizeof(int) * size);
  p = xmalloc(sizeof(int) * size);
  xfree(p);
  //xfree(p-size);
  exit(0);
}//main

#include <stdlib.h>
#include <stdio.h>
#include "xmalloc.h"
//Steven Herring
//testing a double free using xmalloc library

int main() {
  printf("-------- Double Free Test Program -------- \n\n\n");
  int size = 256;
  int *p = xmalloc(sizeof(int) * size);
  xfree(p);
  xfree(p);
  exit(0);
}//main

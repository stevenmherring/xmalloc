#include <stdlib.h>
#include <stdio.h>
#include "xmalloc.h"
//Steven Herring
//tests xmalloc for normalcy

int main() {
  printf("-------- Normal Use Test Program -------- \n\n\n");
  int size = 256;
  int *p = xmalloc(sizeof(int) * size);
  xfree(p);
  exit(0);
}//main

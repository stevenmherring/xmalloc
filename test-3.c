#include <stdlib.h>
#include <stdio.h>
#include "xmalloc.h"
//Steven Herring
//testing an illegal pointer free()

int main() {
  printf("-------- Free Illegal Pointer Test Program -------- \n\n\n");
  int *p = 0;
  xfree(p);
  exit(0);
}//main

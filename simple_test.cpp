#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "my_malloc.h"

int main(int argc, char **argv)
{
  printf("sbrk(0) = %p\n", sbrk(0));
  
  char *dynvar1 = (char *)my_malloc(sizeof(char)*4);
  for(int i=2; i<=10; i++)
  {
    char *dynvar2 = (char *)my_malloc(sizeof(char)*4*i);
    int diff = dynvar2-dynvar1;
    int *p = (int *)&dynvar1[-8];
    printf("dynvar(%2lu bytes) = %p, size = %d, dynvar[-8] = %d\n", sizeof(char)*4*(i-1), dynvar1, diff, *p);
    dynvar1 = dynvar2;
  }
  printf("dynvar(%2lu bytes) = %p\n", sizeof(char)*4*(10), dynvar1);

  printf("sbrk(0) = %p\n", sbrk(0));

  return 0;
}

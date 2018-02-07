#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "my_malloc.h"

void int2str(int d, int base, char s[], int start_pos, int end_pos)
{
  while(start_pos >= end_pos && d != 0)
  {
    int digit = (d % base);
    d = d/base;
    if(digit < 10)
    {
      s[start_pos--] = '0' + digit;
    }
    else
    {
      digit -= 10;
      s[start_pos--] = 'a' + digit;
    }
  }
  while(start_pos >= end_pos)
  {
    s[start_pos--] = ' ';
  }
}

void msg1(int size)
{
  char str[18] = "allocate ";
  int2str(size, 10, str, 16, 9);
  str[17] = ' ';
  write(1, str, 18);
}

void msg2(char *ptr)
{
  char str[19] = "got ptr=0x";
  int addr = (long int)ptr;
  int2str(addr, 16, str, 17, 10);
  str[18] = ' ';
  write(1, str, 19);
  char str2[21] = "malloc size=";
  int *p = (int*)&ptr[-8];
  int2str(*p, 10, str2, 19, 12);
  str2[20] = '\n';
  write(1, str2, 21);
}

void msg3(char *ptr)
{
  char str[20] = "free ptr=0x";
  int addr = (long int)ptr;
  int2str(addr, 16, str, 18, 11);
  str[19] = ' ';
  write(1, str, 20);
  char str2[21] = "malloc size=";
  int *p = (int*)&ptr[-8];
  int2str(*p, 10, str2, 19, 12);
  str2[20] = '\n';
  write(1, str2, 21);
}

int main(int argc, char **argv)
{
  int i, size, max_size, num_iters, iter;
  bool free_ptr;
  char *ptrs[100];

  if(argc != 3)
  {
    printf("usage: random_test MAX_SIZE NUM_ITERATIONS\n");
    exit(1);
  }
  max_size = atoi(argv[1]);
  num_iters = atoi(argv[2]);
  if(max_size <= 0 || num_iters <= 0)
  {
    printf("usage: random_test MAX_SIZE NUM_ITERATIONS\n");
    exit(1);
  }

  srandom(time(0));
  
  for(i=0; i<100; i++)
  {
    size = (random() % max_size) + 1;
    msg1(sizeof(char)*size);
    ptrs[i] = (char *)my_malloc(sizeof(char)*size);
    msg2(ptrs[i]);
  }

  for(iter=0; iter<num_iters; iter++)
  {
    for(i=0; i<100; i++)
    {
      free_ptr = random() % 2;
      if(free_ptr)
      {
        msg3(ptrs[i]);
        my_free(ptrs[i]);
        ptrs[i] = NULL;
      }
    }

    for(i=0; i<100; i++)
    {
      if(ptrs[i] == NULL)
      {
        size = (random() % max_size) + 1;
        msg1(sizeof(char)*size);
        ptrs[i] = (char *)my_malloc(sizeof(char)*size);
        msg2(ptrs[i]);
      }
    }
  }

  return 0;
}

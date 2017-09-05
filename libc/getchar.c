#include <stdio.h>
#include <unistd.h>
#include <sys/defs.h>

int getchar()
{
  int c;
  read(0, &c, (size_t) 1); 
  return c;
}


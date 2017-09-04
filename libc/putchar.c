#include <stdio.h>
#include <unistd.h>
#include <sys/defs.h>

int putchar(int c)
{
  write(1, &c, (size_t) 1);
  return c;
}

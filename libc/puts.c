#include <stdio.h>

int puts(const char *s)
{
  for( ; *s; ++s) if (putchar(*s) != *s) return EOF;

  return 0;
  //return (putchar('\n') == '\n') ? 0 : EOF;
}

#include <stdio.h>

int puts(const char *s)
{
  for( ; *s; ++s) if (putchar(*s) != *s) return EOF;
  return 1;  
 //return (putchar('') == '') ? 0 : EOF;
}

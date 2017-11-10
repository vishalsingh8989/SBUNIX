#ifndef _UTILS_H
#define _UTILS_H

#include <sys/defs.h>

void memset(void* dest, int value, int count);
int strstr(char *a, char *b);
char *strcopy(char *d, const char *s);
uint64_t strlen( const char *s);
uint64_t atoi(char *s);
uint64_t otoi(uint64_t o);

#endif

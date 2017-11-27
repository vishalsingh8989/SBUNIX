#ifndef _STRING_H
#define _STRING_H

#include <sys/defs.h>

//int   strcmp(const char *str1, const char *str2);
char* strcpy(char *dst, const char *src);
uint64_t strlen( const char *s);
int strstr(char *a, char *b);
char *strconcat(char *dst, const char *src);

#endif

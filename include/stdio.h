#ifndef _STDIO_H
#define _STDIO_H

#include <sys/defs.h>
static const int EOF = -1;

int putchar(int c);
int getchar();
int puts(const char *s);
int printf(const char *format, ...);
int strcmp(const char * str1, const char * str2);
char* strtok(char * str1, const char * str2);

char* gets(char *s);
char *strcopy(char *d, const char *s);
char *strconcat(char *dst, const char *src);
uint64_t strlen( const char *s);
#endif

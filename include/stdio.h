#ifndef _STDIO_H
#define _STDIO_H

static const int EOF = -1;

int putchar(int c);
int puts(const char *s);
int printf(const char *format, ...);
int strcmp(const char * str1, const char * str2);

char *gets(char *s);

#endif

#ifndef _STDIO_H
#define _STDIO_H

static const int EOF = -1;

int putchar(int c);
int getchar();
int puts(const char *s);
int printf(const char *format, ...);
int strcmp(const char * str1, const char * str2);
char* strtok(char * str1, const char * str2);
char *strcpy(char *dst, const char *src);
int strlen( const char *s);

char *strconcat(char *dst, const char *src);
char* gets(char *s);
void memset(void* dest, int value, int count);
char *itoa(int value, char *str, int base);

#endif

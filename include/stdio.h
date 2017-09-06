#ifndef _STDIO_H
#define _STDIO_H

static const int EOF = -1;

int putchar(int c);
int getchar();
int puts(const char *s);
int printf(const char *format, ...);
int strcmp(const char * str1, const char * str2);
char* strtok(char * str1, const char * str2);
int strlen(char *string);

int strends(char *string , char *pattern);
char* gets(char *s);

#endif

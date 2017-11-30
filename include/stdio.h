#ifndef _STDIO_H
#define _STDIO_H

static const int EOF = -1;

int putchar(int c);
int getchar();
int puts(const char *s);
int printf(const char *format, ...);
int strcmp(const char * str1, const char * str2);
char* strtok(char * str1, const char * str2);
int strlen( const char *s);
int printf(const char *fmt, ...);
int isdigit (char c);
void memset(void* dest, int value, int count);
int atoi(char *num);
char *itoa(int value, char *str, int base);
char *strcpy(char *dst, const char *src);
char *strconcat(char *dst, const char *src);
void dirname(char* buff, char* par_dir);


char* gets(char *s);

#endif

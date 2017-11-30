#ifndef _STRING_H
#define _STRING_H


#include<sys/defs.h>

void *memcpy(void *dest, const void *src, size_t n);
int memcmp(const void* s1, const void* s2,size_t n);
int strncmp(const char* s1, const char* s2, size_t n);
#endif

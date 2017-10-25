#ifndef _UTILS_H
#define _UTILS_H

#include <sys/defs.h>

void memset(void* dest, int value, int count); 
void clr_term(); 
void print_welcome();
void kpanic(const char *fmt, ...);

#endif

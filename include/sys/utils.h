#ifndef _UTILS_H
#define _UTILS_H

#include <sys/defs.h>

void memset(void* dest, int value, int count); 
void memcpy(void* dest, void* src, int count);
void clr_term(); 
void print_welcome();
void kpanic(const char *fmt, ...);
uint64_t align_up (uint64_t addr);
uint64_t align_down (uint64_t addr);

#endif

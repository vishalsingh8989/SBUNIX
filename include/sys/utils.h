#ifndef _UTILS_H
#define _UTILS_H

#include <sys/defs.h>

//static inline void memset(void* dest, int value, int count);
//static inline void memcpy(void* dest, void* src, int count);
static inline void memcpy(void* dest, void* src, int count)
{
    uint8_t * dest_t = (uint8_t *) dest;
    uint8_t * src_t = (uint8_t *) src;

    for(int i = 0; i < count; i++)
        *dest_t++ = *src_t++;
}

static inline void memset(void* dest, int value, int count)
{
    uint8_t *dest_t = (uint8_t *) dest;
    for(int i = 0; i < count; i++)
        *dest_t++ = value;
}

void clr_term();
void print_welcome();
void kpanic(const char *fmt, ...);
uint64_t align_up (uint64_t addr);
uint64_t align_down (uint64_t addr);
uint64_t atoi(char *num);
uint64_t otod(uint64_t onum);
int dir_match(char *dirname, char *patt);
void sleep(int s);
void itoa(int n, char s[]);

#endif

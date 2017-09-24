#ifndef _ASM_UTILS
#define _ASM_UTILS

#include <sys/defs.h>

//Note: from osdev
static inline void outb(uint16_t port, uint8_t val)
{
    __asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
}

static inline void outl(uint16_t port, uint32_t val)
{
    __asm volatile ( "outl %0, %1" : : "a"(val), "Nd"(port) );
}

static inline uint8_t inb(uint16_t port)
{
    uint8_t ret;
    __asm volatile ( "inb %1, %0" : "=a"(ret) : "Nd"(port) );
    return ret;
}

static inline uint32_t inl(uint16_t port)
{
    uint32_t ret;
    __asm volatile ( "inl %1, %0" : "=a"(ret) : "Nd"(port) );
    return ret;
}

#endif

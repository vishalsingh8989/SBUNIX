#ifndef _ASM_UTILS
#define _ASM_UTILS

#include <sys/defs.h>

//Note: Useful utility functions from  osdev.org
static inline void outb(uint16_t port, uint8_t val)
{
    __asm__ __volatile__ ( "outb %0, %1" : : "a"(val), "Nd"(port) );
}

static inline void outl(uint16_t port, uint32_t val)
{
    __asm__ __volatile__ ( "outl %0, %1" : : "a"(val), "Nd"(port) );
}

static inline uint8_t inb(uint16_t port)
{
    uint8_t ret;
    __asm__ __volatile__ ( "inb %1, %0" : "=a"(ret) : "Nd"(port) );
    return ret;
}

static inline uint32_t inl(uint16_t port)
{
    uint32_t ret;
    __asm__ __volatile__ ( "inl %1, %0" : "=a"(ret) : "Nd"(port) );
    return ret;
}

static inline void io_wait(void)
{
    __asm__ __volatile__ ( "outb %%al, $0x80" : : "a"(0) );
}

inline void wrmsr(uint32_t msr_id, uint64_t msr_value)
{
    __asm__ __volatile__( "wrmsr" : : "c" (msr_id), "A" (msr_value) );
}

inline uint64_t rdmsr(uint32_t msr_id)
{
    uint64_t msr_value;
    __asm__ __volatile__( "rdmsr" : "=A" (msr_value) : "c" (msr_id) );
    return msr_value;
}

static inline void invlpg(void* m)
{
    __asm__ __volatile__( "invlpg (%0)" : : "b"(m) : "memory" );
}

static inline uint64_t read_cr3()
{
    uint64_t read_cr3;
    __asm__ __volatile__("movq %%cr3, %0;" : "=r" (read_cr3));

    return read_cr3;
}

static inline void write_cr3(uint64_t lcr3)
{
    __asm__ __volatile__("movq %0, %%cr3;" : : "r"(lcr3));
}

#endif

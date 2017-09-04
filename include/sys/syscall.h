#ifndef SYSCALL_H
#define SYSCALL_H

#include <sys/defs.h>

#define __NR_write  1
#define __NR_exit   60

static inline uint64_t syscall_0(uint64_t s_no) {

    uint64_t out;   

    __asm__ __volatile__("movq %1, %%rax;"
                         "syscall;"
                         
                         "movq %%rax, %0;"
                         :"=r"(out)
                         :"r"(s_no)
                         :"rax"
                         );
    return out;
} 

static inline uint64_t syscall_1(uint64_t s_no, uint64_t aa) {

    uint64_t out;   

    __asm__ __volatile__("movq %1, %%rax;"
                         "movq %2, %%rdi;" 
                         "syscall;"
                         
                         "movq %%rax, %0;"
                         :"=r"(out)
                         :"r"(s_no), "r"(aa)
                         :"rax", "rdi"
                         );
    return out;
} 

static inline uint64_t syscall_2(uint64_t s_no, uint64_t aa, uint64_t bb) {

    uint64_t out;   

    __asm__ __volatile__("movq %1, %%rax;"
                         "movq %2, %%rdi;" 
                         "movq %3, %%rsi;" 
                         "syscall;"
                         
                         "movq %%rax, %0;"
                         :"=r"(out)
                         :"r"(s_no), "r"(aa), "r"(bb)
                         :"rax", "rdi", "rsi"
                         );
    return out;
} 

static inline uint64_t syscall_3(uint64_t s_no, uint64_t aa, uint64_t bb, uint64_t cc) {

    uint64_t out;   

    __asm__ __volatile__("movq %1, %%rax;"
                         "movq %2, %%rdi;" 
                         "movq %3, %%rsi;" 
                         "movq %4, %%rdx;" 
                         "syscall;"
                         
                         "movq %%rax, %0;"
                         :"=r"(out)
                         :"r"(s_no), "r"(aa), "r"(bb), "r"(cc)
                         :"rax", "rdi", "rsi", "rdx"
                         );
    return out;
} 

#endif

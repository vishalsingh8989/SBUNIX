#ifndef SYSCALL_H
#define SYSCALL_H

#include <sys/defs.h>

#define __NR_read        0
#define __NR_write       1
#define __NR_open        2
#define __NR_close       3
#define __NR_pipe       22
#define __NR_dup2       33
#define __NR_fork       57
#define __NR_execve     59
#define __NR_exit       60
#define __NR_wait4      61
#define __NR_getdents   78
#define __NR_getcwd     79
#define __NR_chdir      80

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

static inline uint64_t syscall_4(uint64_t s_no, uint64_t aa, uint64_t bb, uint64_t cc, uint64_t dd) {

    uint64_t out;   

    __asm__ __volatile__("movq %1, %%rax;"
                         "movq %2, %%rdi;" 
                         "movq %3, %%rsi;" 
                         "movq %4, %%rdx;" 
                         "movq %4, %%r10;" 
                         "syscall;"
                         
                         "movq %%rax, %0;"
                         :"=r"(out)
                         :"r"(s_no), "r"(aa), "r"(bb), "r"(cc), "r"(dd)
                         :"rax", "rdi", "rsi", "rdx", "r10"
                         );
    return out;
}



//vj
static inline uint64_t syscall_6(uint64_t arg0,uint64_t arg1, uint64_t arg2, uint64_t arg3 ,uint64_t arg4 , uint64_t arg5, uint64_t arg6){
    uint64_t result;
        __asm volatile("movq %1, %%rax;"
                       "movq %2, %%rdi;"
                       "movq %3, %%rsi;"
                       "movq %4, %%rdx;"
                       "movq %5, %%r10;"
                       "movq %6, %%r8;"
                       "movq %7, %%r9;"
                       "syscall;"
                       "movq %%rax,  %0;"
                       :"=r"(result)
                       :"r"(arg0), "r"(arg1), "r"(arg2), "r"(arg3), "r"(arg4), "r"(arg5), "r"(arg6)
                       :"rax" , "rdi", "rsi" , "rdx" , "r10", "r8", "r9"
                   );

    return result;
    
}
 

#endif

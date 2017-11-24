#ifndef SYSCALL_H
#define SYSCALL_H

#include <sys/defs.h>
#include <dirent.h>

#define __NR_read         0
#define __NR_write        1
#define __NR_open         2
#define __NR_close        3
#define __NR_access      21
#define __NR_pipe        22
#define __NR_dup2        33
#define __NR_fork        57
#define __NR_execve      59
#define __NR_exit        60
#define __NR_wait4       61
#define __NR_getdents    78
#define __NR_getcwd      79
#define __NR_chdir       80
#define __NR_sched_yield 24
#define __NR_shutdown    48

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
                         "movq %5, %%r10;"
                         "syscall;"

                         "movq %%rax, %0;"
                         :"=r"(out)
                         :"r"(s_no), "r"(aa), "r"(bb), "r"(cc), "r"(dd)
                         :"rax", "rdi", "rsi", "rdx", "r10"
                         );
    return out;
}

//Kernal Syscall Handlers.
void sys_exit();
void sys_sched_yield();
void sys_shutdown(uint64_t code);
uint64_t sys_fork();
uint64_t sys_pipe(uint64_t* fds);
uint64_t sys_execve(char *fname, char **argv, char **envp);
uint64_t sys_write(uint64_t fd, uint64_t addr, uint64_t size);
uint64_t sys_read(uint64_t fd, uint64_t addr, uint64_t size);
uint64_t sys_waitpid(uint64_t pid, uint64_t status, uint64_t options);
uint64_t sys_dup2(uint64_t old_fd, uint64_t new_fd);
uint64_t sys_getdents(uint64_t fd, struct dirent *dir, uint64_t size);
uint64_t sys_getcwd(char *buf, uint64_t size);
uint64_t sys_access(char * pathname, uint64_t mode);
uint64_t sys_chdir(char * pathname);
uint64_t sys_open(char * pathname, uint64_t flags);
uint64_t sys_close(uint64_t fd);

#endif

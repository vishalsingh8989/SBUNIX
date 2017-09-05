#include <stdio.h>
#include <sys/defs.h>
#include <sys/syscall.h>

int waitpid(int pid, int *status) {
   uint64_t out;
   out = syscall_4(__NR_wait4, (uint64_t) pid, (uint64_t) status, (uint64_t) 0, (uint64_t) NULL);
   return (int) out;
}

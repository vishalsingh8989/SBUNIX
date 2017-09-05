#include <stdio.h>
#include <sys/defs.h>
#include <sys/syscall.h>

int dup2(int old_fd, int new_fd){
   uint64_t out;
   out = syscall_2(__NR_dup2, (uint64_t) old_fd, (uint64_t) new_fd);
   return (int) out;
}

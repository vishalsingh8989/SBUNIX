#include <stdio.h>
#include <sys/defs.h>
#include <sys/syscall.h>

int pipe(int *fds) {
   uint64_t out;
   out = syscall_1(__NR_pipe, (uint64_t) fds);
   return (int) out;
}

#include <stdio.h>
#include <sys/defs.h>
#include <sys/syscall.h>

int close(int fd) {
   uint64_t out;
   out = syscall_1(__NR_close, (uint64_t) fd);
   return (int) out;
}


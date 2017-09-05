#include <stdio.h>
#include <sys/defs.h>
#include <sys/syscall.h>

int open(const char* pathname, int flags) {
   uint64_t out;
   out = syscall_2(__NR_open, (uint64_t) pathname, (uint64_t) flags);
   return (int) out;
}

#include <stdio.h>
#include <sys/defs.h>
#include <sys/syscall.h>

int access(const char * pathname, int mode) {
   uint64_t out;
   out = syscall_2(__NR_access, (uint64_t) pathname, (uint64_t) mode);
   return (int) out;
}

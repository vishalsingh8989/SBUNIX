#include <stdio.h>
#include <sys/defs.h>
#include <sys/syscall.h>

int fork() {
   uint64_t out;
   out = syscall_0(__NR_fork);
   return (int) out;
}

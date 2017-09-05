#include <stdio.h>
#include <sys/defs.h>
#include <sys/syscall.h>

int chdir(const char *path) {
   uint64_t out;
   out = syscall_1(__NR_chdir, (uint64_t) path);
   return (int) out;
}

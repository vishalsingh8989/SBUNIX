#include <stdio.h>
#include <sys/defs.h>
#include <sys/syscall.h>

char* getcwd(char *buf, size_t size) {
   uint64_t out;
   out = syscall_2(__NR_getcwd, (uint64_t) buf, (uint64_t) size);
   int out_i = (int) out;

   if(out_i == -1)
      return NULL;
   else
      return buf;
}

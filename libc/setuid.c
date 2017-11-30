#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/defs.h>
#include <sys/syscall.h>



int setuid(int uid) {
   uint64_t out;

   out = syscall_1(__NR_setuid, (uint64_t) uid);
   return (int) out;
}

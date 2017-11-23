#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/defs.h>
#include <sys/syscall.h>

int getdents(int fd, struct dirent *dir, int size) {
   uint64_t out;
   out = syscall_3(__NR_getdents, (uint64_t) fd, (uint64_t) dir, (uint64_t) size);
   return out;
}

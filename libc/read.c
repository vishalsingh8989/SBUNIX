#include <stdio.h>
#include <sys/defs.h>
#include <sys/syscall.h>

ssize_t read(int fd, void *buf, size_t count) {
   uint64_t out;
   out = syscall_3(__NR_read, (uint64_t) fd, (uint64_t) buf, (uint64_t) count);
   return (ssize_t) out;
}

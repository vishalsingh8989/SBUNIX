#include <stdio.h>
#include <sys/defs.h>
#include <sys/syscall.h>
#include <unistd.h>

pid_t fork() {
   uint64_t out;
   out = syscall_0(__NR_fork);
   return (pid_t) out;
}

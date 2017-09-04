#include <stdio.h>
#include <sys/defs.h>
#include <sys/syscall.h>

void exit(int exit_code) {
   syscall_1(__NR_exit, (uint64_t) exit_code);
}

#include <stdio.h>
#include <sys/defs.h>
#include <sys/syscall.h>

void shutdown(int code) {
   syscall_1(__NR_shutdown, (uint64_t) code);
}

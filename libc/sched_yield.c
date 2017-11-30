#include <stdio.h>
#include <sys/defs.h>
#include <sys/syscall.h>
#include <unistd.h>

void yeild() {
   syscall_0(__NR_sched_yield);
}

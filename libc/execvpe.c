#include <stdio.h>
#include <sys/defs.h>
#include <sys/syscall.h>

int execvpe(char *fname, char **args, char *envp[]) {
   uint64_t out;
   out = syscall_3(__NR_execve, (uint64_t) fname, (uint64_t) args, (uint64_t) envp);
   return (int) out;
}

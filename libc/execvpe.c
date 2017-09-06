#include <stdio.h>
#include <sys/defs.h>
#include <sys/syscall.h>

int execvpe(char *bin_name, char **args, char *envp) {
   uint64_t out;
   out = syscall_3(__NR_execve, (uint64_t) bin_name, (uint64_t) args, (uint64_t) envp);
   return (int) out;
}

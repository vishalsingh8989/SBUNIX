#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/syscall.h>

string users[5];
string home[5];
extern char* senvp[];

int main(int argc, char* argv[], char* envp[]) {
    users[0] = "jvishal";
    users[1] = "aahangir";
    users[2] = "admin";
    home[0]="/usr/jvishal/";
    home[1]="/usr/aahangar/";
    home[2]="/usr/admin/";
    char cwd[64];
    uint64_t out = out = syscall_1(__NR_setuid, (uint64_t) -1);
    getcwd(cwd,64);
    printf("PWD=%s\n", cwd);
    //printf("PATH=%s\n", senvp[0]);
    printf("USER=%s\n", users[out]);
    printf("HOME=%s\n", home[out]);
    printf("PATH=/home/jvishal/bin:/home/jvishal/.local/bin:/shared/bin:/bin\n");
    printf("SHELL=/bin/bash\n");
  return 0;
}

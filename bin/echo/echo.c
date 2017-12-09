#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/syscall.h>


extern char* senvp[];

string users[5];
string home[5];
int main(int argc, char* argv[], char* envp[]) {

    users[0] = "jvishal";
    users[1] = "aahangir";
    users[2] = "admin";
    home[0]="/usr/jvishal/";
    home[1]="/usr/aahangar/";
    home[2]="/usr/admin/";

  //puts("-----IN ECHO-----");
    if(argc == 1) {
        return 0;
    }
    else {
         if(!strcmp(argv[1], "$PWD")){
            char cwd[64];
            getcwd(cwd,64);
            printf("%s\n", cwd);
        }else if(!strcmp(argv[1], "$PATH")){
            printf("/home/jvishal/bin:/home/jvishal/.local/bin:/shared/bin:/bin\n");
        }else if(!strcmp(argv[1], "$HOME")){
            uint64_t out = out = syscall_1(__NR_setuid, (uint64_t) -1);
            printf("%s\n", home[out]);
        }else if(!strcmp(argv[1], "$USER")){
            uint64_t out = out = syscall_1(__NR_setuid, (uint64_t) -1);
            printf("USER=%s\n", users[out]);
        }else if(!strcmp(argv[1], "$TERM")){
            printf("TERM=/bin/sbush\n");
        }else{
            int idx = 1;
            while(argv[idx] != NULL)
            printf("%s \n", argv[idx++]); //Replace with printf with no newline.
        }
    }


  return 0;
}

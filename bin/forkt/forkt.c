#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

char* const sargv[] = {"bin/ls", NULL};
char* const senvp[] = {"PATH=/bin:", NULL};

int main(int argc, char **argv, char **envp)
{
    puts(">>Begin Fork Test..");

    int status;

    for(int i = 0; i < 40; i++) {
      pid_t pid = fork();

      if(pid == 0) {

        int ret = execvpe(sargv[0], sargv, senvp);
        if(ret < 0) {
            puts("Command not found!!\n");
				    exit(1);
        }
      }
      else {
        puts("Executing waitpid() in Forkt");
        waitpid(pid, &status);
      }
    }

    puts(">>End Fork Test..");

    return 0;
}

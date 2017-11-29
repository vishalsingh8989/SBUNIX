#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv, char **envp)
{
    puts(">>In init process..");
    puts(">>Executing /etc/rc..(TODO: Not Implemented Yet)");
    puts(">>Executing Sbush..");

    char* const sargv[] = {"bin/sbush", NULL};
    char* const senvp[] = {"PATH=/bin:", NULL};

    int status;

    //puts("Executing fork()");
    pid_t pid = fork();

    if(pid == 0) {
        //Include the environment facility or change to execve.
        //puts("Executing execvpe()");
        execvpe(sargv[0], sargv, senvp);
    }
    else {
        puts("Executing waitpid()");
        waitpid(pid, &status);
    }

    while(1) {
        yeild();
    }

    return 0;
}

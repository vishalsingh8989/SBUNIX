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

    pid_t pid = fork();

    if(pid == 0) {
        //Include the environment facility or change to execve.
        execvpe(sargv[0], sargv, senvp);
    }
    else {
        waitpid(pid, &status);
    }

    //while(1) {
    //    __asm__ __volatile__("cli;");
    //    __asm__ __volatile__("hlt;");
    //    __asm__ __volatile__("sti;");
    //    //yeild();
    //}

    return 0;
}

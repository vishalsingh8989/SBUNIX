#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv, char **envp)
{
    /*
    char* video_p = (char *) 0xffffffff800b8000;
    char msg[10] = {'I', 'n', ' ', 'I', 'n', 'i', 't'};

    for(int i = 0; i < 1000; i+=2)
        video_p[i] = msg[(i/2) % 10];
    */

    puts("----Welcome to SBUnix Os-----");
    puts(">>In init process..");
    puts(">>Executing /etc/rc..(TODO: Not Implemented Yet)");
    puts(">>Executing Sbush..");

    char* const sargv[] = {"bin/sbush", NULL};
    char* const senvp[] = {"PATH=/bin:", NULL};

    uint64_t pid;
    int status;

    puts("Executing fork()");
    pid = fork();

    if(pid == 0) {
        //Include to environment facility or change to execve.
        puts("Executing execvpe()");
        //execvpe("bin/sbush", sargv, senvp);
        execvpe(sargv[0], sargv, senvp);
    }
    else {
        puts("Executing waitpid()");
        waitpid(pid, &status);
    }

    while(1);

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/vmm.h>

//char   dir_path[500];
int main(int argc, char **argv, char **envp)
{
    char* video_p = (char *) 0xffffffff800b8000;
    char msg[10] = {'I', 'n', ' ', 'I', 'n', 'i', 't'};

    for(int i = 0; i < 14; i+=2)
       video_p[i] = msg[i/2];


    char   dir_path[500];

    //fork();
    getcwd(dir_path, (size_t) 500);
    chdir("/bin");
    //getcwd(dir_path, (size_t) 500);
    // __asm__ __volatile__("int $0x80" :: "a"(1), "b"(0));

    /*
    while(1) {
        char* video_p = (char *) 0xffffffff800b8000;
        char msg[10] = {'I', 'n', ' ', 'I', 'n', 'i', 't'};

        for(int i = 0; i < 14; i+=2)
            video_p[i] = msg[i/2];
        __asm__ __volatile__("hlt:");
    }
    */

    return 0;
}

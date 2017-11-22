#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/utils.h>



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
    char buff[500];
    getcwd(buff, 500);


    int fidx = open("/lib", 0);


    struct dirent dir_buff ;
    struct dirent* ptr_dir = &dir_buff;
    ptr_dir->offset=0;

    while(getdents(fidx, ptr_dir, 100) !=-1){

    		puts("Dir is : ");
    		puts(ptr_dir->d_name);
    		puts("\n");
    }



    //char fidxbuff[10];
    //itoa(fidx, fidxbuff);
    //puts(fidxbuff);
    //puts("cwd from sys_call");
    //puts(buff);

//    char* const sargv[] = {"/bin/sbush", NULL};
//    char* const senvp[] = {"PATH=/bin:", NULL};
//
//    uint64_t pid;
//    int status;
//
//    puts("Executing fork()");
//    pid = fork();
//
//    if(pid == 0) {
//        //Include to environment facility or change to execve.
//        puts("Executing execvpe()");
//        execvpe("/bin/sbush", sargv, senvp);
//    }
//    else {
//        puts("Executing waitpid()");
//        waitpid(pid, &status);
//    }

    //while(1);

    return 0;
}

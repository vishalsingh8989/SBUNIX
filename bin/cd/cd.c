#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>


int main(int argc, char **argv, char **envp)
{

	puts("*************In cd*********\n");
	char buff[NAME_MAX+1];
	memset(buff, '\0',  NAME_MAX+1);
	char *dir = "/lib";// TODO replace dir with argv[1]

	if(dir[0] == '/'){
    		strcpy(buff, dir);
    		//strconcat(buff, (const char *)dir);

    }else{

    		if(dir[0] == '.' && dir[1] == '/'){

    			getcwd(buff, 500);
    			int len = strlen(buff);
    			//int dirlen = strlen(dir);
    			int idx = 2;
    			while(dir[idx] !='\0'){
    				buff[len + idx - 2] = dir[idx];
    				idx++;
    			}
    		}else{
    			getcwd(buff, 500);
    			strconcat(buff, (const char *)dir);

    		}
    		//strconcat(buff, (const char *)dir);


    }


    strconcat(buff, "/");

    puts("chdir to:");
    puts(buff);
    puts("\n");
    chdir(buff);
    	getcwd(buff, 500);



//    	struct dirent dir_buff ;
//    struct dirent* ptr_dir = &dir_buff;
//    ptr_dir->offset=0;
//
//
//    while(getdents(fidx, ptr_dir, 100) !=-1){
//    		if(strcmp(ptr_dir->d_name,argv[1]))
//    		{
//    			//TODO chdir();
//    			return 0;
//    		}
//    		puts("Dir is : ");
//    		puts(ptr_dir->d_name);
//    		puts("\n");
//    }





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

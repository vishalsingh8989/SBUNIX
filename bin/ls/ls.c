#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/defs.h>
#include <sys/stat.h>
#include <sys/user.h>

char *file_type = "-lscbdfc";
int main(int argc, char* argv[], char* envp[]) {

	char buff[NAME_MAX+1];
	memset(buff, '\0', NAME_MAX+1);
	if(argc == 1){
		getcwd(buff, NAME_MAX+1);
	}else if(argc == 2){
		strcpy(buff, argv[1]);
	}else{
		printf("Multiple directories not supported\n");
	}
	DIR *dir = opendir(buff);
	struct dirent *dirent;
	uint32_t size = 0;

	while ((dirent = readdir(dir))) {
			 char *fname = strtok(dirent->d_name, "/");
		     char *prev = NULL;
		     while (fname != NULL) {
		         prev = fname;
		         fname = strtok(NULL, "/");
		     }
		     if(dirent->type == 5){// DIRTYPE
		         strconcat(prev , "/");
		     }
			printf("%crwx--x--x    %4dKB     %s     %s\n", file_type[dirent->type], (dirent->size)/1024,dirent->fowner, prev);

			size = size + dirent->size;
	        //printf("%s  \n", dirent->d_name);
	    }
	if(dir != NULL){
		printf("drwx--x--x    %4dKB     %s  %s\n", size/1024,"admin", ".");
		printf("drwx--x--x    %4dKB     %s  %s\n", 0,"admin", "..");
	}else{
		printf("ls: %s: No such file or directory,\n", argv[1]);
	}
	return 0;

//	char* m = (char*)malloc(100);
//	char* k = (char*)malloc(100);
//	printf("allocated :  %p\n", m);
//	printf("allocated :  %p\n", k);



}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[], char* envp[]) {

	if(argc != 2){
	printf("What manual page do you want?.  Usage :  man  <cmd>.\n");
	}else{
		if(!strcmp(argv[1], "ls")){
			printf("Usage : ls. list content of current directory\n");
		}else if(!strcmp(argv[1], "ps")){
			printf("Usage : ps. list current running processes. \n");
		}else if (!strcmp(argv[1], "cat")){
			printf("Usage : cat <file_name>. file name can be fullpath or files in current directory\n");
		}else if (!strcmp(argv[1], "clear")){
			printf("Usage : clear \n");

		}else if(!strcmp(argv[1], "date")){
			printf("Usage: date . Print UTC TIME.\n");
		}else if(!strcmp(argv[1], "cd")){
			printf("Usage: cd <path>. path can be absolute path/path in current directory/..(parent). \n");
		}else if(!strcmp(argv[1], "pwd")){
			printf("Usage : pwd. print current directory\n");
		}else if(!strcmp(argv[1], "logout")){
			printf("Usage : logout. logout current user.\n");
		}else if(!strcmp(argv[1], "kill")){
			printf("Usage : kill <process id>. kill running process.\n");

		}else{
			printf("No manual entry for  %s\n", argv[1]);
			printf("Commands available: cat, cd, ls, ps , pwd, clear, date\n");
		}
	}

	return 0;
}

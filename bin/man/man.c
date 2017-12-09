#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[], char* envp[]) {

	if(argc != 2){
	printf("What manual page do you want?.  Usage :  man  <cmd>.\n");
	}else{
		if(!strcmp(argv[1], "ls")){
			printf("Detail about ls\n");
			
		}else if(!strcmp(argv[1], "ps")){
			printf("Detail about ps\n");
		}else if (!strcmp(argv[1], "cat")){
			printf("Detail about cat\n");
		}else if (!strcmp(argv[1], "clear")){
			printf("Detail about clear\n");
		}else if(!strcmp(argv[1], "date")){
			printf("Detail about date\n");
		}else if(!strcmp(argv[1], "cd")){
			printf("Detail about cd\n");
		}else if(!strcmp(argv[1], "pwd")){
			printf("Detail about pwd\n");
		}else if(!strcmp(argv[1], "logout")){
			printf("Detail about logout\n");
		}else{
			printf("No manual entry for  %s\n", argv[1]);
			printf("Commands available: cat, cd, ls, ps , pwd, clear, date\n");
		}
	}

	return 0;
}

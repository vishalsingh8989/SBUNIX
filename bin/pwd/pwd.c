#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/defs.h>

int main(int argc, char **argv, char **envp){
	//puts("*************In pwd*********\n");

//	printf("************In pwd argv*************\n");
//	for(int i=0 ;i < argc ; i++){
//		printf("%s at %p\n", argv[i], &argv[i]);
//	}
//	printf("************In pwd argv************* %d \n", argc);
	char buff[NAME_MAX+1];
	memset(buff, '\0', NAME_MAX+1);
	getcwd(buff, NAME_MAX+1);
	//puts("pwd is :");
	printf("%s\n",buff);
	return 0;
}

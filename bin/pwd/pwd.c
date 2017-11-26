#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/defs.h>

int main(int argc, char **argv, char **envp){
	//puts("*************In pwd*********\n");
	char buff[NAME_MAX+1];
	memset(buff, '\0', NAME_MAX+1);
	getcwd(buff, NAME_MAX+1);
	//puts("pwd is :");
	puts(buff);
	puts("\n");
	return 0;
}

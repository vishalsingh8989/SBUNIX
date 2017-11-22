#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/utils.h>

int main(int argc, char **argv, char **envp){

	char buff[500];
    	getcwd(buff, 500);
    	puts(buff);
    return 0;
}

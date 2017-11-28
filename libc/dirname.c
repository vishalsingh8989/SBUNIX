#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/defs.h>

void dirname(char* buff, char* par_dir)
{
	memset(par_dir, '\0', sizeof(par_dir));
	char *tokens[12];
	int idx = 0;
	tokens[idx] = strtok(buff, "/");
	strcpy(par_dir, "/");
	while (tokens[idx] != NULL) {
		++idx;
		tokens[idx] = strtok(NULL, "/");
		//prev = tokens[idx];
		if(tokens[idx] !=NULL){
			strconcat(par_dir, tokens[idx-1]);
			strconcat(par_dir, "/");
		}
	}

}

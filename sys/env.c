#include <sys/env.h>
#include <stdio.h>

void init_env(){
	strcpy(PS1, "#sbush>");
}

char* getenv(int env){

	switch(env){
	case ENV_PWD:
		return PWD;
	case ENV_PS1:
		return PS1;
	default:
		return "";

	}
}









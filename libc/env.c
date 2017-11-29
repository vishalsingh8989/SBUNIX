
#include <sys/env.h>
#include <stdio.h>

char* getenv(int env){

	switch(env){
	case ENV_PWD:
		return PWD;
	case ENV_PS1:
		return PS1;

	}
	return  "";

}


void setenv(int env, char* env_val){
	switch(env){
	case ENV_PS1:
		strcpy(PS1,env_val);
		break;
	}
}

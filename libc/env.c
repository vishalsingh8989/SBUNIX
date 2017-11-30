
#include <sys/env.h>
#include <stdio.h>

char* getenv(int env){

	switch(env){
	case ENV_PWD:
		return PWD;
	case ENV_PS1:
		//printf("get env :  %s.\n", PS1);
		return PS1;
	case ENV_HOME:
		return HOME;
	case ENV_USER:
		return USER;
	}
	return  "";

}


void setenv(int env, char* env_val){
	//printf("set env : %s\n", env_val);
	switch(env){
	case ENV_PS1:
		strcpy(PS1,env_val);
		break;
	case ENV_HOME:
		strcpy(HOME,env_val);
		break;
	case ENV_USER:
		strcpy(USER,env_val);
		break;
	}
}

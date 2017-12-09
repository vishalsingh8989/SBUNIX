#include <sys/env.h>
#include <sys/kprintf.h>
#include <stdio.h>

char PWD[MAX_NAME+1];
char PS1[MAX_NAME+1];
char HOME[MAX_NAME+1];
char USER[MAX_NAME+1];
char PATH[MAX_NAME+1];

void init_env(){
	klog(BOOTLOG, "Set environment variable.\n");
	strcpy(PS1, "#sbush>");
	strcpy(PATH, "/bin:/shared/bin:/usr/local/sbin");
}

char* getenv(int env){

	switch(env){
	case ENV_PWD:
		return PWD;
	case ENV_PS1:
		return PS1;
	case ENV_HOME:
		return HOME;
	case ENV_USER:
		//kprintf("get env : %s\n", USER);
		return USER;
	}
	return  "";

}


void setenv(int env, char* env_val){
	kprintf("set env : %s\n", env_val);
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

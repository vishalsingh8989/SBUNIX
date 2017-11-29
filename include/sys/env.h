#ifndef _ENV_H
#define _ENV_H

//char *LOGNAME = "admin";
#include <sys/process.h>

#define ENV_PWD	0
#define ENV_PS1  1
char PWD[MAX_NAME+1];
char PS1[MAX_NAME+1];

void init_env();
char* getenv(int env);
void setenv(int env, char* env_val);

#endif

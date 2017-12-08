#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/env.h>


extern char PWD[MAX_NAME+1];
extern char PS1[MAX_NAME+1];
extern char HOME[MAX_NAME+1];
extern char USER[MAX_NAME+1];
extern char PATH[MAX_NAME+1];

int main(int argc, char* argv[], char* envp[]) {


  //print_env();

    //char* paths = (char*)malloc(sizeof(char)*200);
    //char* user = (char*)malloc(sizeof(char)*100);
    //char* cmd  = (char*)malloc(sizeof(char)*100);


    //strcpy(paths, PATH);
    //strcpy(user, USER);
    printf("PATH=%s\n", PATH);
    printf("USER=%s\n", USER);

  return 0;
}

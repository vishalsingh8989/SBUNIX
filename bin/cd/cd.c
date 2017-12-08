#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/env.h>



int main(int argc, char **argv, char **envp)
{

	char buff[NAME_MAX+1];

	//printf("allocated :  %p\n", m);
	if(argc == 1){
		return 0;
	}else if(argc == 2){
		//remove_extra(argv[1], strlen(argv[1]));
		//getcwd(buff, NAME_MAX+1);
		if(!strcmp(argv[1], ".")){
			//printf("Move dir\n");
			return 0;
		}else if( strlen(argv[1]) == 1 && argv[1][0] == '~'){
			chdir(getenv(ENV_HOME));
			return 0;
		}else if( argv[1][0]=='.' && strlen(argv[1]) !=1 && strcmp(argv[1] , "..")){

			//char dir[NAME_MAX+1];
			getcwd(buff, NAME_MAX+1);
			char *tokens[12];
			int idx = 0;
			tokens[idx] = strtok(argv[1], "/");
			//printf("token :  %s.\n", tokens[idx]);
			while (tokens[idx] != NULL) {
				++idx;
				tokens[idx] = strtok(NULL, "/");
				strconcat(buff, tokens[idx]);
				if(tokens[idx] !=NULL){
					strconcat(buff, "/");
				}
			}

			chdir(buff);
			return 0;

		}else if(!strcmp(argv[1] , "..")){
			//printf("Current dir\n");
			char *par_dir= (char *)malloc(sizeof(char)*(NAME_MAX+1));
			getcwd(buff, NAME_MAX+1);
			dirname(buff, par_dir);
			//printf("Move to par dir :  %s \n", buff);
			chdir(par_dir);
			return 0;
		}else if(argv[1][0] == '/'){
			strcpy(buff,argv[1]);
			if(strlen(argv[1]) == 1){
				//TODO change to root.
			}else{
				//change to
				strconcat(buff, "/");
			}
			//printf("chdir : %s\n",buff);
			chdir(buff);
			//printf("Move from root.\n");
			return 0;
		}else{
			getcwd(buff, NAME_MAX+1);
			strconcat(buff, argv[1]);
			strconcat(buff, "/");
			chdir(buff);
			return 0;
		}

		return 0;
	}




    return 0;
}

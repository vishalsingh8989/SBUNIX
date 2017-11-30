#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/defs.h>
#include <sys/env.h>


#define MAX_INPUT 512
#define TRUE 1
#define FALSE 0

char  str_buf[MAX_INPUT];
char  path_buf[MAX_INPUT];
char* pipes[64];
char* tokens[8];
char* mod_tokens[64];
char  path_var[MAX_INPUT] = "/home/aahangar/workdir/rootfs/bin/";
char  ps1_var[MAX_INPUT] = "sbush>";
char* sargv[] = {"bin/ls", NULL};
char* senvp[] = {"PATH=/bin:", NULL};

int   err;
char* perr;

extern char PWD[MAX_NAME+1];
extern char PS1[MAX_NAME+1];
char weekdayn[7][4] = {
    "Mon",
    "Tue",
	"Wed",
	"Thu",
	"Fri",
	"Sat",
	"Sun"
};

char monthn[13][5] = {
    "Jan",
    "Feb",
	"Mar",
	"Apr",
	"May",
	"Jun",
	"July",
	"Aug",
	"Oct",
	"Sept",
	"Nov",
	"Dec"
};


void setprompt() {
    char *temp;
    temp = getenv(ENV_PS1);
    char buff[50];
    if(temp == NULL) {
       printf("%s#sbush>", getcwd(buff, 50));
    }
    else {
       //bprintf("cwd :  %s. %s.\n", getcwd(buff, 50), temp);
       printf("%s#%s",getcwd(buff, 50), temp);
    }
}


int main(int argc, char* argv[], char* envp[]) {

    setenv(ENV_PS1, "sbush>");
	printf("Users : jvishal, aahangir, user1. Password is root.\n");

	char username[30];
	char password[30];
	printf("\n");
	LOGOUT:while(1){
		printf("username:");
		perr = gets(username);
		printf("password:");
		perr = gets(password);


        //int user_id = 0;
		if(!strcmp("jvishal", username) && !strcmp("root", password))
		{
			printf("Welcome Vishal.\n");
			setenv(ENV_HOME, "/usr/jvishal/");
            //setuid(user_id);
			chdir("/usr/jvishal/");
			break;

		}

        if(!strcmp("aahangir", username) && !strcmp("root", password)){
			puts("Welcome Atif ahangir.\n");
			setenv(ENV_HOME, "/usr/aahangir/");
            //user_id = 1;
            //setuid(user_id);
			chdir("/usr/aahangir/");
			break;
		}

        // With 3 users prompt is messed up. #weird.
        // if(!strcmp("user1", username) && !strcmp("root", password)){
        //     printf("Welcome user1 .\n");
        //     setenv(ENV_HOME, "/usr/user1/");
        //     chdir("/usr/user1/");
        //     break;
        // }

		puts("Incorrect username or password.\n");


	}


	struct tm tm_time;
	gettime(&tm_time);
	//printf("%d\n", tm_time.tm_wday);
	printf("%s, %s %d  %d :%d :%d,  UTC %d   \n",weekdayn[tm_time.tm_wday],monthn[tm_time.tm_mon-1],tm_time.tm_mday,tm_time.tm_hour,tm_time.tm_min,tm_time.tm_sec, tm_time.tm_year);


    puts("---Welcome to SBUSH shell---\n");

    if(argc == 1) {
        while (TRUE) {

            setprompt();

            perr = gets(str_buf);
            if(strlen(str_buf)==0){
            		continue;
            }else if(!strcmp("logout", str_buf)){
				printf("Logged out\n");
				goto LOGOUT;
			}
            int idx = 0;
			tokens[idx] = strtok(str_buf, " ");
			while (tokens[idx] != NULL) {
				////printf("%s\n",tokens[idx]);
				++idx;
				tokens[idx] = strtok(NULL, " ");
			}

            pid_t pid = fork();
            int status;
            char *const senvp[] = {"PATH=/bin:", NULL};

            if(pid == 0) {

            		int ret = 0;
            		ret = execvpe(tokens[0], tokens, senvp);

                if(ret < 0) {
                       puts("Command not found!!\n");
                }
            }
            else {
                puts("Executing waitpid()");
                waitpid(pid, &status);
            }

            //while(1); //Put execve here for now, until pipe is implemented.

            //execute_line(str_buf, envp);
        }
    }
//    else {
//        int fd;
//
//        fd = open(argv[1], O_RDONLY);
//        err = read(fd, str_buf, 18*sizeof(char));
//
//        if (!strcmp(str_buf, "#!rootfs/bin/sbush")) { //check if file is executable.
//
//            int temp;
//            err = read(fd, &temp, 1*sizeof(char));
//
//            err = read(fd, &temp, 1*sizeof(char));
//            while (err != 0) {
//                int cidx = 0;
//
//                while (temp != '\n' && err != 0) {
//                    str_buf[cidx++] = (char) temp;
//                    err = read(fd, &temp, 1*sizeof(char));
//                }
//                str_buf[cidx] = '\0';
//
//                execute_line(str_buf, envp);
//                err = read(fd, &temp, 1*sizeof(char));
//            }
//        }
//        else {
//            puts("File is not a valid sbush shell script!");
//        }
//        close(fd);
//    }

    return 0;
}

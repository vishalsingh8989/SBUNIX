#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_INPUT 512
#define TRUE 1
#define FALSE 0

char  scmd[MAX_INPUT];
char  fcmd[MAX_INPUT];
char  prompt[MAX_INPUT] = "sbush> ";
char* tokens[64];

int   err;
char* perr;

int main(int argc, char* argv[]) {

    int idx;
    int bp;
    int pid;
    int status;

    for(;;) {

        idx = 0;

        if (argc == 1) {
            puts(prompt);
            perr = gets(scmd);

            tokens[idx] = strtok(scmd, " =");
            while (tokens[idx] != NULL) {
                tokens[++idx] = strtok(NULL, " =");
            }
        }
        else {
            for(int i = 1; i < argc; i++)
                tokens[idx++] = argv[i];
        }

        if (!strcmp(tokens[0], "exit")) {
            return 0;
        }
        else if (!strcmp(tokens[0], "pwd")) {
            perr = getcwd(scmd, (size_t) MAX_INPUT);
            puts(scmd);
        }
        else if (!strcmp(tokens[0], "cd")) {
            err = chdir(tokens[1]);
        }
        //TODO: check for PATH.
        //TODO: check for PS1.
        else if (!strcmp(tokens[0], "PS1")) {
            printf("Token 1 %s, Token 2 %s\n",tokens[0], tokens[1]);
            int cidx = 0;
            while (tokens[1][cidx] != '\0') {
                prompt[cidx] = tokens[1][cidx];
                cidx++;
            }
        }
        else if (!strcmp(tokens[0], "sbush")) {
            int fd;
            fd = open(tokens[1], O_RDONLY);
            err = read(fd, fcmd, 7*sizeof(char));

            if (!strcmp(fcmd, "#!sbush")) {
                int temp;

                err = read(fd, &temp, 1*sizeof(char));

                while (err != 0) {
                    int cidx = 0;
                    err = read(fd, &temp, 1*sizeof(char));

                    while (temp != '\n' && err != 0) {
                        scmd[cidx++] = (char) temp;
                        err = read(fd, &temp, 1*sizeof(char));
                    }
                    scmd[cidx] = '\0';

                    idx = 0;
                    tokens[idx] = strtok(scmd, " =");
                    while (tokens[idx] != NULL) {
                        tokens[++idx] = strtok(NULL, " =");
                    }

                    pid = fork();
                    if (pid == 0) {
                        err = execvp(tokens[0], tokens);
                        if (err == -1) {
                            puts("Invalid command!");
                            return 1;
                        }
                    }
                    else if (pid < 0) {
                        puts("Failed to fork!");
                    }
                    else {
                        waitpid(pid, &status, 0);
                    }
                }
            }
            else {
                puts("File is not a valid sbush shell script!");
            }
            close(fd);
        }
        else {
            if(tokens[idx-1][0] == '&') {
                bp = TRUE;
            }
            else {
                bp = FALSE;
            }

            pid = fork();

            if (bp == TRUE) {
                if (pid == 0) {
                    err = execvp(tokens[0], tokens);
                    if (err == -1) {
                        puts("Invalid command!");
                        return 1;
                    }
                }
                else if (pid < 0) {
                    puts("Failed to fork!");
                }
            }
            else {
                if (pid == 0) {
                    err = execvp(tokens[0], tokens);
                    if (err == -1) {
                        puts("Invalid command!");
                        return 1;
                    }
                }
                else if (pid < 0) {
                    puts("Failed to fork!");
                }
                else {
                    waitpid(pid, &status, 0);
                }
            }
        }
    }
}

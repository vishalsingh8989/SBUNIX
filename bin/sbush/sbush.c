#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_INPUT 512
#define TRUE 1
#define FALSE 0

char  str_buf[MAX_INPUT];
char  prompt[MAX_INPUT];
char  path[MAX_INPUT];
char* pipes[64];
char* tokens[64];

int   err;
char* perr;

//TODO: do some more processing here.
void setprompt() {
    char * temp;
    int idx = 0;
    temp = getenv("PS1");
    while(temp[idx] != '\0') {
        prompt[idx] = temp[idx];
        idx++;
    }
    puts(prompt);
}

int execute(char* cmd, int pos) {

    int pipe_ids[2];
    static int pipe_prev;

    pipe(pipe_ids);

    int idx = 0;
    tokens[idx] = strtok(cmd, " ");
    while (tokens[idx] != NULL) {
        tokens[++idx] = strtok(NULL, " ");
    }

    if (!strcmp(tokens[0], "cd")) {
        int err;
        err = chdir(tokens[1]);
        return 0;
    }
    else if (!strcmp(tokens[0], "export")) {
        setenv(tokens[1], tokens[2], 1);
        return 0;
    }

    int bp, status;
    if(tokens[idx-1][0] == '&') {
        bp = TRUE;
    }
    else {
        bp = FALSE;
    }

    int pid = fork();
    
    if (pid == 0) {

        if (pos == 0) {
            dup2(pipe_ids[1], 1);
        }
        else if (pos == 1) {
            dup2(pipe_prev, 0);
            dup2(pipe_ids[1], 1);
        }
        else {
            dup2(pipe_prev, 0);
        }

        err = execvp(tokens[0], tokens);
        if (err == -1) {
            puts("Invalid command!");
            exit(1);
        }

    }
    else if (pid < 0) {
        puts("Failed to fork!");
    }
    else {
        if (bp == FALSE) waitpid(pid, &status, 0);
    }

    if (pos == 0) {
        close(pipe_ids[1]);
        pipe_prev = pipe_ids[0];
    }
    else if (pos == 1) {
        if (pipe_prev != 0) close(pipe_prev);
        close(pipe_ids[1]);
        pipe_prev = pipe_ids[0];
    }
    else {
        if (pipe_prev != 0) close(pipe_prev);
        close(pipe_ids[1]);
        close(pipe_ids[0]);
    }

    return 0;
}

int main(int argc, char* argv[]) {

    if(argc == 1) {
        while (TRUE) {

            setprompt();
            perr = gets(str_buf);

            int idx = 0;
            pipes[idx] = strtok(str_buf, "|");
            while (pipes[idx] != NULL) {
                pipes[++idx] = strtok(NULL, "|");
            }

            if(!strcmp(pipes[0], "exit")) return 0;
            else {
                for(int i = 0; i < idx; i++) {
                    int pos;

                    if (i == idx-1) 
                        pos = 2;
                    else if (i == 0)
                        pos = 0;
                    else 
                        pos = 1;

                    execute(pipes[i], pos);
                }
            }
        }
    }
    else {
        int fd;

        fd = open(argv[1], O_RDONLY);
        err = read(fd, str_buf, 18*sizeof(char));

        if (!strcmp(str_buf, "#!rootfs/bin/sbush")) { //check if file is executable.

            int temp;
            err = read(fd, &temp, 1*sizeof(char));

            while (err != 0) {
                int cidx = 0;
                err = read(fd, &temp, 1*sizeof(char));

                while (temp != '\n' && err != 0) {
                    str_buf[cidx++] = (char) temp;
                    err = read(fd, &temp, 1*sizeof(char));
                }
                str_buf[cidx] = '\0';

                int idx = 0;
                tokens[idx] = strtok(str_buf, " =");
                while (tokens[idx] != NULL) {
                    tokens[++idx] = strtok(NULL, " =");
                }

                int pid = fork();
                if (pid == 0) {
                    err = execvp(tokens[0], tokens);
                    if (err == -1) {
                        puts("Invalid command!");
                        return 1;
                    }
                }
                else if (pid < 0) {
                    puts("Failed to fork!");
                    return 1;
                }
                else {
                    int status;
                    waitpid(pid, &status, 0);
                }
            }
        }
        else {
            puts("File is not a valid sbush shell script!");
        }
        close(fd);
    }

    return 0;

}

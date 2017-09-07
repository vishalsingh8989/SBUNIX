#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/defs.h>

#define MAX_INPUT 512
#define TRUE 1
#define FALSE 0

char  str_buf[MAX_INPUT];
char  path_buf[MAX_INPUT];
char* pipes[64];
char* tokens[64];
char* mod_tokens[64];
char  path_var[MAX_INPUT] = "/home/aahangar/workdir/rootfs/bin/";
char  ps1_var[MAX_INPUT] = "sbush>";

int   err;
char* perr;

char* getenv(const char * var_name) {
   if(!strcmp(var_name, "PATH"))
      return path_var;
   else if(!strcmp(var_name, "PS1"))
      return ps1_var;
   else
      return NULL;
}

void setenv(const char * var_name, const char * var_value, int overwrite) {
   int idx=0;
   //TODO: Implement strcpy
   if(!strcmp(var_name, "PATH")){
      while(var_value[idx] != '\0') {
         path_var[idx] = var_value[idx];
         idx++;
      }
      path_var[idx] = '\0';
   }
   else if(!strcmp(var_name, "PS1")){
      while(var_value[idx] != '\0') {
         ps1_var[idx] = var_value[idx];
         idx++;
      }
      ps1_var[idx] = '\0';
   }
}


//TODO: do some more processing here.
void setprompt() {
    char *temp;
    temp = getenv("PS1");

    if(temp == NULL) {
       puts("sbush>");
    }
    else {
       puts(getenv("PS1"));
    }
}

int execute(char* cmd, int pos, char * envp[]) {

    int pipe_ids[2];
    static int pipe_prev;

    err = pipe(pipe_ids);

    int idx = 0;
    tokens[idx] = strtok(cmd, " ");
    while (tokens[idx] != NULL) {
        tokens[++idx] = strtok(NULL, " ");
    }

    if (idx == 0) return 0;

    if (!strcmp(tokens[0], "cd")) {
        err = chdir(tokens[1]);
        return 0;
    }
    else if (!strcmp(tokens[0], "export")) {
        setenv(tokens[1], tokens[2], 1);
        return 0;
    }

    int pd, bp, status;
    if(tokens[idx-1][0] == '&') {
        bp = TRUE;
    }
    else {
        bp = FALSE;
    }

    //PATH code
    mod_tokens[0] = path_buf;
    for(int i = 1; i < idx; i++)
        mod_tokens[i] = tokens[i];
    for(int i = idx; i < 64; i++)
        mod_tokens[i] = NULL;
    
    int idx1 = 0;
    while(path_var[idx1] != '\0'){
        path_buf[idx1] = path_var[idx1];
        idx1++;
    }
    int idx2 = 0;
    while(tokens[0][idx2] != '\0'){
        path_buf[idx1+idx2] = tokens[0][idx2];
        idx2++;
    }
    path_buf[idx1+idx2] = '\0';

    if(tokens[0][0] != '/' && access(path_buf, X_OK) != -2) pd = 1;
    else pd = 0;

    if (pd == 0) {
       int idx2 = 0;
       while(tokens[0][idx2] != '\0'){
           path_buf[idx2] = tokens[0][idx2];
           idx2++;
       }
       path_buf[idx2] = '\0';
    }
    //PATH code

    pid_t pid = fork();
    
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

        int err = execvpe(mod_tokens[0], mod_tokens, envp);

        //TODO: why does it return -2 instead of -1, put the errno functionality in execvpe wrapper and always return -1 for error.
        if (err == -2 || err == -1) {
            puts("Invalid command!");
            exit(1);
        }

    }
    else if (pid < 0) {
        puts("Failed to fork!");
    }
    else {
        if (bp == FALSE) waitpid(pid, &status);
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

int execute_line(char* cmd, char* envp[]) {

   int idx = 0;
   pipes[idx] = strtok(str_buf, "|");
   while (pipes[idx] != NULL) {
       pipes[++idx] = strtok(NULL, "|");
   }
   
   if(!strcmp(pipes[0], "exit")) exit(0);
   else {
       for(int i = 0; i < idx; i++) {
           int pos;
   
           if (i == idx-1) 
               pos = 2;
           else if (i == 0)
               pos = 0;
           else 
               pos = 1;
   
           execute(pipes[i], pos, envp);
       }
   }

   return 0;
}

int main(int argc, char* argv[], char* envp[]) {
    
    if(argc == 1) {
        while (TRUE) {

            setprompt();
            perr = gets(str_buf);

            execute_line(str_buf, envp);
        }
    }
    else {
        int fd;

        fd = open(argv[1], O_RDONLY);
        err = read(fd, str_buf, 18*sizeof(char));

        if (!strcmp(str_buf, "#!rootfs/bin/sbush")) { //check if file is executable.

            int temp;
            err = read(fd, &temp, 1*sizeof(char));

            err = read(fd, &temp, 1*sizeof(char));
            while (err != 0) {
                int cidx = 0;

                while (temp != '\n' && err != 0) {
                    str_buf[cidx++] = (char) temp;
                    err = read(fd, &temp, 1*sizeof(char));
                }
                str_buf[cidx] = '\0';

                execute_line(str_buf, envp);
                err = read(fd, &temp, 1*sizeof(char));
            }
        }
        else {
            puts("File is not a valid sbush shell script!");
        }
        close(fd);
    }

    return 0;
}


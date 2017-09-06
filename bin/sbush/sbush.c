#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/defs.h>
#include<envp.h>
#include<malloc.h>

#define PRENOTBLANK     0
#define PREBLANK        1

#define MAX_INPUT 512
#define TRUE  1
#define FALSE 0
#define DEBUG 1



char  str_buf[MAX_INPUT];
char  prompt[MAX_INPUT];
char  path[MAX_INPUT];
char* pipes[64];
char* tokens[64];
int   err;
char* perr;

void print_welcome_message(){
    puts("###################################################################################\n");
    puts("# SBUSH  \n");
    puts("# author :  Vishal Jasrotia , jvishal@cs.stonybrook.edu\n");
    puts("# term   :  Fall 2017 , cse 506.\n");
    puts("#\n");
    puts("# Help-->\n");
    puts("# cd : change currect working directory Example : cd ~ is got to home, cd ../jvishal/workdir ,  cd /home/jvishal/workdir.\n");
    puts("# ls : list the content of directory. Example : ls = show content of curr dir. ls path.\n");
    
    puts("# pwd : print present working directory.\n");
    puts("# exit : exit sbush\n");
    puts("####################################################################################\n");


}

int readline(char *line, int count){
    int i = 0 ;
    int c ; 
    int enc = PRENOTBLANK;
    line[0]='\0';//if line length is 0 -vj
    
    //read and remove multiple whitespaces -vj
    while((c = getchar())!= EOF && c !='\n' && i < count-1){ //count -1 because null char is required in end -vj
        if(c == ' ' && enc == PRENOTBLANK){
            //puts(c); for testing -vj
            line[i] = c;
            i++;
            enc = PREBLANK;
        }
        else if(c == ' ' && enc == PREBLANK){
            //skip if second :/vsjwhitespace
        }else{
            line[i] = c;
            i++;
            //puts(c); //for testing -vj
            enc = PRENOTBLANK;
        }
    
    }
    //check is last char is white spaces . "ls " , "cd " fails is last char is whitespace.
    if(line[i-1] == ' ')
        --i;
    line[i] = '\0'; //missed this line and cried for 2 hours.
    i++;

    return i;

}






char* getenv(env_var *head,const char * var) {
   return get_value(head,var);
}

env_var* setenv(env_var *head,char * var_name, char * var_value) {
    
    char  *exist;
    exist = get_value(head,var_name);    
    if(exist == NULL){
        insert_env(&head, var_name, var_value, env_list_length(head) + 1);// always insert at head.
    }else{
        puts("Already exists. Overwrite - y or n?");
        char response =  getchar();
        if(response == 'y'){    
            update_env(head,var_name, var_value);    
        }else{
            puts("Abort export!!i\n");
        }    

     }
    
    return head;
}


void setprompt(env_var *head) {

    char *temp;
    temp = getenv(head,"PS1");
    if(temp == NULL) {
        puts("sbush>");
    }
    else {
        puts(temp);
    }
}

int execute(char* cmd, int pos, env_var *head) {

    int pipe_ids[2];
    static int pipe_prev;
    
    err = pipe(pipe_ids);

    int idx = 0;
    tokens[idx] = strtok(cmd, " ");
    while (tokens[idx] != NULL) { 
        tokens[++idx] = strtok(NULL, " ");
    }

    if (!strcmp(tokens[0], "cd")) {
        err = chdir(tokens[1]);
        return 0;
    }
    else if (!strcmp(tokens[0], "export")) {
        
        puts("export cmd detected\n");
        char *export_var[2];
        export_var[0] = strtok(tokens[1] , "=");
        export_var[1] = strtok(NULL , "=");
        if(export_var[0] == NULL || export_var[1] ==NULL){
            puts("Export null\n");        
        }else{
            puts("Export not null\n");
            head = setenv(head , export_var[0] ,export_var[1]);
            if(DEBUG)
                print_env_var(head); 
            //
        }
        return 0;
    }else if(strends(tokens[0] ,".sh")){
        puts("Script detected\n");
        
    }else{
        return 0;
    }

    int bp, status;
    if(tokens[idx-1][0] == '&') {
        bp = TRUE;
    }
    else {
        bp = FALSE;
    }

    int pid =  fork();
    
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
        
        int err = execvpe(tokens[0], tokens, head);

        //TODO: why does it return -2 instead of -1, put the errno functionality in execvpe wrapper and always return -1 for error.
        if (err == -2) {
            puts("Invalid command!\n");
            exit(1);
        }

    }
    else if (pid < 0) {
        puts("Failed to fork!\n");
    }
    else {
       if (bp == FALSE)waitpid(pid, &status);
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




/*
main function
*/

int main(int argc, char* argv[], char* envp[]) {        
//    char str_buf[MAX_INPUT];
    env_var  *head;
    head = NULL;
    head = setenv(head,"PS1", "sbush>");
    head = setenv(head,"PS2", ">");    
    
    print_welcome_message();
    
     strends("Hello" , "lo");
    setprompt(head);
    if(argc == 1) {
        while(TRUE) {
            //int read_count =readline(str_buf, MAX_INPUT-1);
            gets(str_buf);
            setprompt(head);
            //if(read_count==1 ){continue;};
            int idx = 0;
            
            pipes[idx] = strtok(str_buf, "|");
            while (pipes[idx] != NULL) {
                pipes[++idx] = strtok(NULL, "|");
            }
            
            //puts("Hello\n");
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

                    execute(pipes[i], pos, head);
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
                    err = execvpe(tokens[0], tokens, head);
                    if (err == -1) {
                        puts("Invalid command!\n");
                        return 1;
                    }
                }
                else if (pid < 0) {
                    puts("Failed to fork!\n");
                    return 1;
                }
                else {
                    int status;
                    waitpid(pid, &status);
                }
            }
        }
        else {
            puts("File is not a valid sbush shell script!\n");
        }
        close(fd);
    }

    return 0;

}


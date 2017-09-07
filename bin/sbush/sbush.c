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
#define DEBUG FALSE


#define LS      100
#define CAT     101
#define EXPORT  102
#define CD      103
#define SCRIPT  104
#define PWD     105
#define EXIT    110

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

int is_valid_command(char *tokens[]){

        if(!strcmp("cd" , tokens[0])){
            //puts("cmd is cd\n");
            return CD;
        }else if(!strcmp("exit" , tokens[0])){
            //puts("cmd is exit\n");
            return EXIT;
        }else if(!strcmp("export" ,tokens[0])){
            //puts("cmd is export\n");
            return EXPORT;
        }else if(!strcmp("ls" , tokens[0])){
           // puts("cmd is ls\n");
            return LS;
        }else if(!strcmp("cat" , tokens[0])){
           // puts("cmd is cat\n");
            return CAT;
        }else if(strends(tokens[0] , ".sh")){
            //puts("script detected\n");
            return SCRIPT;
        }else if(!strcmp(tokens[0], "pwd")|| !strcmp(tokens[0] ,"PWD")){
            return PWD;
        }
        

    return FALSE;

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
    line[i] = '\0'; 
    i++;

    return i;

}


void setprompt(env_var *head) {

    char *temp;
    temp = get_value(&head,"PS1");
    if(temp == NULL) {
        puts("sbush>");
    }
    else {
        puts(temp);
    }
}

int execute(char* cmd, int pos, env_var **head , env_var **paths) {

    int pipe_ids[2];
    static int pipe_prev;
    int command_type;

    err = pipe(pipe_ids);

    int idx = 0;
    tokens[idx] = strtok(cmd, " ");
    while (tokens[idx] != NULL) { 
          tokens[++idx] = strtok(NULL, " ");
    }
    command_type = is_valid_command(tokens);
 
    switch(command_type){
            case LS:
            case CD:
            case CAT:
                //other binaries.
            
                return 0;
            case PWD:
                puts(get_value(head,"PWD"));
                puts("\n");
                return 0; 
            case EXPORT:
                {
                    char* export_var[2];
                    export_var[0] = strtok(tokens[1] , "=");
                    export_var[1] = strtok(NULL , "=");
                    if(export_var[0] == NULL || export_var[1] ==NULL){
                        puts("Error in export command!! usage.  export KEY=VALUE \n");        
                    }else{
                        //puts("Export not null\n");
                        insert_env(head ,1, export_var[0] ,export_var[1]);
                        if(DEBUG){print_env_var(*head);}; 
                    }
                    return 0;
                }
            case SCRIPT:
            case EXIT:
                exit(0);
            default:
                puts("Invalid command\n");
            
        }  

  
    if (!strcmp(tokens[0], "cd")) {
        err = chdir(tokens[1]);
        return 0;
    }
    else if (!strcmp(tokens[0], "export")) {
        
           }

   /*
    else if(strends(tokens[0] ,".sh")){
        puts("Script detected\n");
        
    }else{
        puts("Invalid command!!\n");
        return 0;
    }
   */
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
       
         
        env_var *temp_path_head = *paths;
        int err;
        for(;temp_path_head !=NULL;temp_path_head = temp_path_head->next)
             err = execvpe(tokens[0], tokens, temp_path_head->value);

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
    
    env_var  *head;
    env_var  *paths;
       

    //set key value pairs;    
    head = NULL;
    
    insert_env(&head, 1, "PS1", "sbush>");// always insert at head.
    insert_env(&head, 1, "PS2", "continue-->");// always insert at head.
    int env_idx = 0; 
    char *key;
    char  *value;   
    while(*(env_idx+envp) !='\0'){
        key = strtok(*(env_idx+envp) , "=");
        value = strtok(NULL, "=");
        insert_env(&head, 1, key, value);
        env_idx++;
    }        
    insert_env(&head, 1, "PS3", "atif>");// always insert at head.
    
    paths = NULL;

    print_env_var(head);
    insert_env(&paths, 1, "bindir", "/home/jvishal/gitbucket/rootfs/bin");// always insert at head.
    insert_env(&paths, 1, "sysbin",  "/usr/bin");// always insert at head.

    print_welcome_message();
    if(argc == 1) {
        while(TRUE) {
            puts(get_value(&head,"PS1"));
            int read_count = readline(str_buf, 4096);
            
            if(read_count == 1){};
//            puts("\n");
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
                   
                    execute(pipes[i], pos, &head ,&paths);
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
                    err = execvpe(tokens[0], tokens, paths->value);
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


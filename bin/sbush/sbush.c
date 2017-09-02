#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define MAX_INPUT 512

char  scmd[MAX_INPUT];
char  tpath[MAX_INPUT]; //TODO: can use scmd instead.
char* tokens[MAX_INPUT];
char  prompt[MAX_INPUT] = "sbush> ";

int   err;
char* perr;

//Functions to be implemented for part 2.
//1. gets
//2. strtok
//3. strcmp

int main(int argc, char* argv[]) {

  int idx = 0;

  for(;;) {
      puts(prompt);

      perr = gets(scmd);
      idx = 0;

      tokens[idx] = strtok(scmd, " =");
      while (tokens[idx] != NULL) {
        idx++;
        tokens[idx] = strtok(NULL, " =");
      }

      if (perr == NULL) printf("gets failed\n");

      if (!strcmp(tokens[0], "exit")) {
          return 0;
      }
      else if (!strcmp(tokens[0], "pwd")) {
          perr = getcwd(tpath, (size_t) MAX_INPUT);
          puts(tpath);
      }
      else if (!strcmp(tokens[0], "cd")) {
          err = chdir(tokens[1]);
      }
      else if (!strcmp(tokens[0], "echo")) {
         if (tokens[1][0] == '&') {
            //check for PATH and PS1 and echo them else print error message 
         }
         else {
            for(int i = 1; i < idx; i++)
               puts(tokens[i]); //TODO: can implement something which doesn't add new line
         }
      }
      else if (!strcmp(tokens[0], "PATH")) {
         printf("TODO PATH\n");
      }
      else if (!strcmp(tokens[0], "PS1")) {
         printf("TODO PS1\n");
      }
      else {
          //TODO: open a file and read first line. Differentiate between shell script and executable.

          if (1/*SHELL*/) {
          }
          else if (tokens[idx-1][0] == '&') {
             //Use fork with execvpe
          }
          else {
             //Loop throught he tokens to see if pipes are there.
          }
      }
  }

  return 0;
}


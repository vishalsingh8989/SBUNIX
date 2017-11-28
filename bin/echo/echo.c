#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[], char* envp[]) {

  puts("-----IN ECHO-----");
  if(argc == 1) {
    return 0;
  }
  else {
    int idx = 1;
    while(argv[idx] != NULL)
      puts(argv[idx++]); //Replace with printf with no newline.
  }

  return 0;
}

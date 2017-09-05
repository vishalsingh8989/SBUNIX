#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/defs.h>

void _start(uint64_t arg_addr) {

  int argc = (int) *(&arg_addr+8);
  char* argv[2];
  char* envp[2];

  argv[0] = (char *) *(&arg_addr+9);
  argv[1] = (char *) *(&arg_addr+10);
  
  //puts(argv[0]);
  //puts(argv[1]);

  int return_code;
  return_code = main(argc, argv, envp);
  exit(return_code);
}

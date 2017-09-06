#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/defs.h>

void _start(uint64_t arg_addr) {

  int offset = 102;
  int argc = (int) *(&arg_addr+offset);
  char* argv[32] = {NULL};
  char* envp[64] = {NULL};

  //TODO: do dynamic memory allocation to get rid of this limnitation.
  if (argc > 32) {
     puts("Sbush only supports 32 arguments to main!");
     exit(1);
  }

  for (int i = 0; i < argc; i++) {
     argv[i] = (char *) *(&arg_addr+offset+i+1);
  }

  char *temp = (char *) *(&arg_addr+offset+argc+2);
  int idx = 1;
  while (temp != 0 && idx < 64) {
     envp[idx-1] = temp;
     idx++;
     temp = (char *) *(&arg_addr+offset+argc+idx+2);
  }

  //puts(argv[0]);
  //puts(argv[1]);

  int return_code;
  return_code = main(argc, argv, envp);
  exit(return_code);
}

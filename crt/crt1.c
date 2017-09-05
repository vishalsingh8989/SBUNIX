#include <stdlib.h>

void _start(int argc, char** argv, char** envp) {

  int return_code;
  return_code = main(argc+1, argv, envp);
  exit(return_code);
}

#include <stdlib.h>

void _start(int argc, char** argv, char** envp) {

  int return_code;

  return_code = main(argc, argv, envp);
  exit(return_code);
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/defs.h>

int main(int argc, char* argv[], char* envp[]) {

   int fd;
   int c = 0;
   int err = -1;

   if(argc == 2) {
      fd = open(argv[1], O_RDONLY);     
   }
   else {
      return 0;
   }

   if(fd == -1) {
      puts("file not found");
      return 1;
   }

   while(err != 0) {
      err = read(fd, &c, 1);
      putchar(c);
   }  

   return 0;
}

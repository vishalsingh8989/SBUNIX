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
      printf("fd is : %d\n", fd);
   }
   else {
      return 0;
   }

   if(fd == -1) {
	   printf("file not found\n");
      return 1;
   }
   sleep(99);

   while(err != 0) {
      err = read(fd, &c, 1);
      printf("%c\n",c);
      printf("addr :  %p\n", &c);
      //sleep(99);
   }  

   return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/defs.h>

int main(int argc, char* argv[], char* envp[]) {

   int fd;
   int c = 0;
   int size = 0;

   if(argc == 2) {
      fd = open(argv[1], O_RDONLY);
   }
   else {
	    //TODO check this.
      return 0;
   }
   if(fd == -1) {
	   printf("File : %s not found!\n", argv[1]);
	   return 1;
   }
   while(size != EOF) {
      size = read(fd, &c, 1);
      printf("%c",c);
   }

	 printf("\n");
	 close(fd);
   exit(0);
}

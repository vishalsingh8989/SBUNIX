#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/defs.h>

int main(int argc, char* argv[], char* envp[]) {

   int    num_dirs;
   int    fd;
   char   dir_struct[2048];
   struct dirent* p;
   char   dir_path[NAME_MAX+1];

   if(argc == 1) {
     getcwd(dir_path, (size_t) NAME_MAX+1);
     fd = open(dir_path, O_RDONLY | O_DIRECTORY); 
   }
   else {
     fd = open(argv[1], O_RDONLY | O_DIRECTORY);
   }

   if(fd == -1) {
     puts("Directory not found");
     return 1;
   }

   num_dirs = getdents(fd, dir_struct, 1024);

   int pos = 0;
   while(pos < num_dirs) {
     p = (struct dirent *) (dir_struct + pos);
     puts(p->d_name);
     puts("\t");
     pos += p->reclen;
   }
    puts("\n"); 
   close(fd);

   return 0;
}



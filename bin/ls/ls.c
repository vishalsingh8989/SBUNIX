#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<dirent.h>
#include<sys/kprintf.h>
#include<sys/defs.h>

int main(int argc, char* argv[], char* envp[]) {

    char buf[256];
    struct dirent *dirent;
    getcwd(buf, 256);
    DIR *dir = opendir(buf);
    while ((dirent = readdir(dir))) {
        puts(dirent->d_name);
    }
    puts("\n");
    return 0;

//   if(argc == 1) {
//     getcwd(dir_path, (size_t) NAME_MAX+1);
//     fd = open(dir_path, O_RDONLY | O_DIRECTORY);
//   }
//   else {
//     fd = open(argv[1], O_RDONLY | O_DIRECTORY);
//   }
//
//   if(fd == -1) {
//     puts("Directory not found");
//     return 1;
//   }
//
//   num_dirs = getdents(fd, dir_struct, 1024);
//
//   int pos = 0;
//   while(pos < num_dirs) {
//     p = (struct dirent *) (dir_struct + pos);
//     puts(p->d_name);
//     pos += p->reclen;
//   }
//
//   close(fd);

//   return 0;
}

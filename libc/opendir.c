#include<dirent.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/kprintf.h>
#include<sys/defs.h>

DIR* opendir(const char *name)
{
    int dir_fd = -1;
    if (-1 == (dir_fd = open(name, O_DIRECTORY | O_RDONLY))) {
        puts("opendir failed.\n");
        return NULL;
    }
    DIR *dir= NULL;//(DIR *) malloc(sizeof(DIR));
    dir->dfd = dir_fd;
    //dir->ptrdir = (struct dirent *) malloc(sizeof(struct dirent));
    return dir;
}

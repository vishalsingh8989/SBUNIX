#include<dirent.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/kprintf.h>
#include<sys/defs.h>

struct dirent *readdir(DIR * dir)
{
    struct dirent *direntry = dir->ptrdir;
    int bytes = 0;
    if (-1 ==(bytes = getdents(dir->dfd, direntry->d_name, sizeof(struct dirent)))) {
        puts("getdents() failed.\n");
        return NULL;
    }
    if (0 == bytes) {
        return NULL;
    }
//    if (-1 == lseek(dir->dfd, dir_ent->offset, SEEK_SET)) {
//        puts("getdents() failed.\n");
//        exit(1);
//    }
    return dir->ptrdir;
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/defs.h>
#include <sys/syscall.h>


DIR *opendir(const char *name)
{
    int fd = -1;
    if (-1 == (fd = open(name,  O_DIRECTORY))) {
        printf("open :  %s   failed.", name);
        return NULL;
    }
    DIR *dir = (DIR *) malloc(sizeof(DIR));
    dir->dfd = fd;
    dir->ptr_dir = (struct dirent *) malloc(sizeof(struct dirent));
    return dir;
}

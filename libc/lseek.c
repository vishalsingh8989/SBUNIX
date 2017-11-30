#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/defs.h>
#include <sys/syscall.h>


uint64_t lseek(int fildes, off_t offset, int whence)
{
    uint64_t out = syscall_3(__NR_lseek, (uint64_t) fildes, (uint64_t) offset, (uint64_t) whence);
    return out ;
}

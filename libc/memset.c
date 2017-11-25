#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/defs.h>
#include <sys/syscall.h>
void memset(void* dest, int value, int count)
{
    uint8_t *dest_t = (uint8_t *) dest;
    for(int i = 0; i < count; i++)
        *dest_t++ = value;
}

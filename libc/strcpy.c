#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/defs.h>

char *strcopy(char *d, const char *s)
{
    char *dest_iter = d;
    const char *src_iter = s;



    while (dest_iter && src_iter && *src_iter != '\0') {
        *dest_iter = *src_iter;
        dest_iter++;
        src_iter++;
    }
    *dest_iter = '\0';
    return dest_iter;
}

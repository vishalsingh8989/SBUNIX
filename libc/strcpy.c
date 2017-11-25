#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/defs.h>

char *strcpy(char *dst, const char *src)
{
    char *dst_t = dst;

    while(*src != '\0')
        *dst++ = *src++;

    return dst_t;
}

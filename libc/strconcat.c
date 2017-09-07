
#include <stdlib.h>
#include<stdio.h>
char *strconcat(char *dst, char *src)
{
    char *new_str;
    char *temp;
    int dst_len = 0, src_len = 0;

    dst_len = strlen(dst);
    src_len = strlen(src);
    new_str = (char *) malloc(dst_len + src_len + 1);
    temp = new_str;

    if (src == NULL)
        return new_str;

    while(*dst)
    {
        *temp++ = *dst++;

    }
    while(*src)
        *temp++ = *src++;
    *++temp = '\0';

    return new_str;
}

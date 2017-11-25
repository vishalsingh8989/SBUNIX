#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/defs.h>

char *strconcat(char *dst, const char *src)
{

    int dst_len = 0;
    char *dest_iter = dst;
    const char *src_iter = src;

    dst_len = strlen(dst);
    //src_len = strlen(src);
    int idx = 0;
    while(idx<dst_len){
    		idx++;
    		dest_iter++;
    }

    while (dest_iter && src_iter && *src_iter != '\0') {
        *dest_iter = *src_iter;
        dest_iter++;
        src_iter++;
    }
    *dest_iter = '\0';
    return dest_iter;
    }

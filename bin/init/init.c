#include <stdio.h>

int main(int argc, char **argv)
{
    char* video_p = (char *) 0xffffffff800b8000;
    char msg[10] = {'I', 'n', ' ', 'I', 'n', 'i', 't'};

    for(int i = 0; i < 14; i+=2)
        video_p[i] = msg[i/2];

    return 0;
}

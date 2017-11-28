#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/defs.h>

char * gets(char * buf) {
    int temp = -1;
    int idx  = 0;

    //TODO: skip initial spaces.

    temp = getchar();

    while (temp != '\n' && temp != EOF) {

       buf[idx++] = temp;
       temp = getchar();
       if(temp == -1){
    	   	   buf[idx] = '\0';
    	   	   idx--;
       }

    }
    buf[idx] = '\0';

    return buf;
}

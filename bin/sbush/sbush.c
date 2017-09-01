#include <stdio.h>
#include <unistd.h>

#define MAX_INPUT 512

char scmd[512];
char tpath[512];
int err;

int strcmp(const char * str1, const char * str2);

int main(int argc, char* argv[]) {
  for(;;) {
    puts("sbush> ");
    gets(scmd);
    if (~strcmp(scmd, "exit")) {
        return 0;
    }
    else if (~strcmp(scmd, "pwd")) {
        getcwd(tpath, (size_t) MAX_INPUT);
        puts(tpath);
    }
    else if (scmd[0] == 'c' && scmd[1] == 'd') {
        chdir(scmd+3);
    }
  }
}

int strcmp(const char * str1, const char * str2) {
    char cmp1 = 1, cmp2 = 1;
    int result = 1;
    int idx = 0;

    //TODO: change to do while
    while (cmp1 != '\0' && cmp2 != '\0' && result != 0) {
        cmp1 = str1[idx];
        cmp2 = str2[idx];
        result = (cmp1 == cmp2);

        if (cmp1 == '\0' && cmp2 != '\0') result = 0;
        if (cmp2 == '\0' && cmp1 != '\0') result = 0;

        idx++;
    }

    return ~result;

}

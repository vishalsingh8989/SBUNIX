#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/defs.h>
/* from KR book*/

void reverse(char s[])
 {
     int i, j;
     char c;

     for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
         c = s[i];
         s[i] = s[j];
         s[j] = c;
     }
 }

/* itoa:  convert n to characters in s  from KR book*/
void itoa(int n, char s[]){

	int i, sign;


    if ((sign = n) < 0)  /* record sign */
        n = -n;          /* make n positive */
    i = 0;
    do {       /* generate digits in reverse order */
        s[i++] = n % 10 + '0';   /* get next digit */
    } while ((n /= 10) > 0);     /* delete it */
    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';
    reverse(s);
}


int main(int argc, char* argv[], char* envp[]) {

	puts("************In ls*************\n");
	for(int i=0 ;i < argc ; i++){
		puts(argv[i]);
	}

	sleep(99999);
	sleep(99999);
	puts("************In ls*************\n");

	char buff[NAME_MAX+1];
	memset(buff, '\0', NAME_MAX+1);
	if(argc == 1){
		getcwd(buff, NAME_MAX+1);
	}else if(argc == 2){
		strcpy(buff, argv[1]);
	}else{

		puts("Multiple directories not supported\n");

	}
	puts("*********");
	puts("Executing ls for ");
	puts(buff);
	int fidx = open(buff, 0);
	struct dirent dir_buff ;
	struct dirent* ptr_dir = &dir_buff;
	ptr_dir->offset=fidx;

	memset(dir_buff.d_name, '\0', sizeof(dir_buff.d_name));
	while(getdents(fidx, ptr_dir, 100) !=-1){
		puts(ptr_dir->d_name);
		memset(dir_buff.d_name, '\0', sizeof(dir_buff.d_name));


	}
	puts("*********");
}

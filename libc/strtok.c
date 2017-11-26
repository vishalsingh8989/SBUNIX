#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/defs.h>

char *strchr(const char *s, int c)
{
    while (*s != (char)c)
        if (!*s++)
            return 0;
    return (char *)s;
}

size_t strcspn(const char *s1, const char *s2)
{
    size_t ret=0;
    while(*s1)
        if(strchr(s2,*s1))
            return ret;
        else
            s1++,ret++;
    return ret;
}

int strspn(const char *p, const char *s)
{
	int i, j;

	for (i = 0; p[i]; i++) {
		for (j = 0; s[j]; j++) {
			if (s[j] == p[i])
				break;
		}
		if (!s[j])
			break;
	}
	return (i);
}

char *strtok(char * str, const char * delim)
{
	puts("str is : ");
	puts(str);
	puts("\n");

    static char* p=0;
    if(str){
        p=str;
    }
    else if(!p){
    		puts("RETURN 0\n");
        return 0;
    }
    str=p+strspn(p,delim);
    p=str+strcspn(str,delim);
    if(p==str){
    		puts("RETURN p==0\n");
        return p=0;
    }
    p = *p ? *p=0,p+1 : 0;
    puts("RETURN ");
    puts(str);
    puts("\n");
    return str;
}

//char* strtok(char * str1, const char * str2) {
//   static int pos = 0;
//   static char *str;
//   int start = pos;
//
//   if (str1 != NULL) {
//      str = str1;
//      pos = 0;
//      start = 0;
//   }
//
//   if (str == NULL) return NULL;
//
//   while(str[pos] != '\0' && str[pos] != EOF) {
//      if (str[pos] == str2[0]) { //TODO: add support for more delimiters.
//          str[pos] = '\0';
//          if (start == pos) { //Skipping the initial delimiters.
//             start++;
//             pos++;
//          }
//          else if (str[start] != '\0') {
//             pos++;
//             puts("RETURN  not NULL ");
//             puts(&str[start]);
//             puts("\n");
//             return &str[start];
//          }
//      }
//      pos++;
//   }
//
//   if (str[start] == '\0') {
//	  puts("RETURN NULL\n");
//      return NULL;
//   }
//   else{
//	   puts("RETURN  not NULL ");
//	   puts(&str[start]);
//	   puts("\n");
//      return &str[start];
//   }
//}

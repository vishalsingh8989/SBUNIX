#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/defs.h>


char* strtok(char * str1, const char * str2) {
   static int pos = 0;
   static char *str;
   int start = pos;

   //printf("str : %s\n", str1);
   if (str1 != NULL) {
      str = str1;
      pos = 0;
      start = 0;
   }

   if (str == NULL) return NULL;

   while(str[pos] != '\0' && str[pos] != EOF) {
      if (str[pos] == str2[0]) { //TODO: add support for more delimiters.
          str[pos] = '\0';
          if (start == pos) { //Skipping the initial delimiters.
             start++;
             pos++;
          }
          else if (str[start] != '\0') {
             pos++;
             //puts("RETURN  not NULL ");
             //puts(&str[start]);
             //puts("\n");
             return &str[start];
          }
      }
      pos++;
   }

   if (str[start] == '\0') {
	  //puts("RETURN NULL\n");
      return NULL;
   }
   else{
	   //puts("RETURN  not NULL ");
	   //puts(&str[start]);
	   //puts("\n");
      return &str[start];
   }
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/defs.h>

//TODO: should folow strcmp convention...should output binary difference
int strcmp(const char * str1, const char * str2) {
    char cmp1 = 1, cmp2 = 1;
    int result = 1;
    int idx = 0;
  
    if (str1 == NULL || str2 == NULL)
       return -1;

    //TODO: change to do while
    while (cmp1 != '\0' && cmp2 != '\0' && result != 0) {
        cmp1 = str1[idx];
        cmp2 = str2[idx];
        result = (cmp1 == cmp2);

        if (cmp1 == '\0' && cmp2 != '\0') result = 0;
        if (cmp2 == '\0' && cmp1 != '\0') result = 0;

        idx++;
    }
    return !result;
}

int strends(char *string , char *pattern)
{
    int str_len = strlen(string);
    int patt_len = strlen(pattern);
    int diff = str_len - patt_len;
    if(diff>=0){
        
        int str_idx = diff;
        int patt_idx = 0;
        for(;str_idx <= str_len && patt_idx <= patt_len; ){
            if( !(string[str_idx++] == pattern[patt_idx++])){
                return 0;
            }
        }
        return 1;
    
 
    //    return 
    }else{
        return 0;
    }  


}


int strcmp_old(const char * str_one, const char * str_two){

    int i = 0;
    while(str_one[i] == str_two[i] ){
        if(str_one[i] == '\0'){
            return 0;// equal
            }
        i++;
    }
    return 1;
}

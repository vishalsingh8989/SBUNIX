#include <sys/utils.h>
#include <sys/defs.h>


void memset(void* dest, int value, int count){
    uint8_t *dest_t = (uint8_t *) dest;
    for(int i = 0; i < count; i++)
        *dest_t++ = value;
}

uint64_t strlen( const char *s){
    uint64_t len = 0;

    while (*s  !=  '\0') {
    		len++;
        s++;
    }
    return len;
}

int strstr(char *a, char *b){

		if(strlen(a) < strlen((const char*)b)) return -1;
	   int position = 0;
	   char *x, *y;
	   x = (char*)a;
	   y = b;
	   while(*a){
		  while(*x==*y){
			 x++;
			 y++;
			 if(*x=='\0'||*y=='\0')
				break;
		  }
		  if(*y=='\0')
			 break;
		  a++;
		  position++;
		  x = (char*)a;
		  y = b;
	   }
	   if(*a)
		  return position;
	   else
		  return -1;
}


char *strcopy(char *d, const char *s)
{
    char *dest_iter = d;
    const char *src_iter = s;
    while (dest_iter && src_iter && *src_iter != '\0') {
        *dest_iter = *src_iter;
        dest_iter++;
        src_iter++;
    }
    *dest_iter = '\0';
    return dest_iter;
}






uint64_t atoi(char *s){
	char *iterator = (char *) s;
    uint64_t num = 0;
    while (*iterator) {
    		num = num * 10 + (*iterator++ - '0');
    }
    return num;
}

uint64_t otoi(uint64_t o)
{
    uint64_t decimal = 0, oct = o;
    uint64_t mul = 1;
    while (oct) {
        decimal += mul * (oct % 10);
        mul *= 8;
        oct /= 10;
    }
    return decimal;
}





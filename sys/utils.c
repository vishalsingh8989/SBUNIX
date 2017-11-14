#include <sys/defs.h>
#include <sys/utils.h>
#include <sys/kprintf.h>
#include <sys/vmm.h>

void memcpy(void* dest, void* src, int count)
{
    uint8_t * dest_t = (uint8_t *) dest;
    uint8_t * src_t = (uint8_t *) src;

    for(int i = 0; i < count; i++)
        *dest_t++ = *src_t++;
}

void memset(void* dest, int value, int count)
{
    uint8_t *dest_t = (uint8_t *) dest;
    for(int i = 0; i < count; i++)
        *dest_t++ = value;
}

void clr_term()
{
    for(int i = 0; i < MAX_Y; i++) 
        for(int j = 0; j < MAX_X*2; j++)
            kprintf(" ");
}

void print_welcome()
{
    kprintf("\n\n");
    kprintf("\t\t===========================\n");
    kprintf("\t\t=----Welcome to SBUnix----=\n");
    kprintf("\t\t===========================\n");
    kprintf("\n\n");
}

void kpanic(const char *fmt, ...)
{
    __asm__ __volatile__("cli;");
    kprintf(fmt);
    kprintf("\n");
    kprintf("Kernal is Panicking!!!\n");
//    kprintf("\t            _  _   \n");
//    kprintf("\t      ___ (~ )( ~) \n");
//    kprintf("\t     /   \\_\\ \\/ /  \n");
//    kprintf("\t    |   D_ ]\\ \\/   \n");
//    kprintf("\t    |   D _]/\\ \\   \n");
//    kprintf("\t     \\___/ / /\\ \\  \n");
//    kprintf("\t          (_ )( _) \n");
//    kprintf("\t            ~  ~   \n");
    while(1) {
        __asm__ __volatile__ ("hlt;");
    }
}

uint64_t align_up (uint64_t addr) 
{
    uint64_t offset = ((uint64_t) addr % PAGE_SIZE);

    if (offset != 0) offset = PAGE_SIZE - offset;

    return addr + offset;
}

uint64_t align_down (uint64_t addr) 
{
    uint64_t offset = ((uint64_t) addr % PAGE_SIZE);

    return addr - offset;
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


uint64_t atoi(char *num)
{
    int i = 0;
    uint64_t inum = 0;

    while(num[i] >= '0' && num[i] <= '9') {
        inum = inum*10 + (num[i] - '0');
        i++;
    }

    return inum;
}

uint64_t otod(uint64_t onum)
{
    uint64_t dnum = 0;
    int base = 1;

    for(uint64_t otemp = onum; otemp != 0; otemp/=10) {
        dnum += (otemp % 10) * base;
        base = base * 8;
    }

    return dnum;
}

//uint64_t atoi(char *s){
//	char *iterator = (char *) s;
//    uint64_t num = 0;
//    while (*iterator) {
//    		num = num * 10 + (*iterator++ - '0');
//    }
//    return num;
//}
//
//uint64_t otoi(uint64_t o)
//{
//    uint64_t decimal = 0, oct = o;
//    uint64_t mul = 1;
//    while (oct) {
//        decimal += mul * (oct % 10);
//        mul *= 8;
//        oct /= 10;
//    }
//    return decimal;
//}



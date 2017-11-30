#include <sys/defs.h>
#include <sys/utils.h>
#include <sys/kprintf.h>
#include <sys/string.h>
#include <sys/vmm.h>

void sleep(int s){
//	for(int i = 0 ;i< s*99999; i ++){
//
//	}
}

int dir_match(char *dirname, char *patt){
	if (strlen(dirname) > strlen(patt)){
		//kprintf("");
		return -1;
	}

	char *dir_ptr = dirname;
	char *patt_ptr = patt;
	int pos = 0;
	while(*dir_ptr == *patt_ptr && *patt_ptr != '\0'){
		pos++;
		dir_ptr++;
		patt_ptr++;

	}
	if(pos  == strlen(dirname)){
		//kprintf("match :  %s, %s\n", dirname, patt);
		return 1;
	}else{
		//kprintf("Not match :  %s, %s\n", dirname, patt);
		return -1;
	}

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

void clr_term()
{
    for(int i = 0; i < MAX_Y; i++)
        for(int j = 0; j < MAX_X*2; j++)
            kprintf(" ");

	reset_cord();
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
    //kprintf(fmt);
    //kprintf("\n");
    kprintf("Kernal is Panicking!!!\n");
    //kprintf("\t            _  _   \n");
    //kprintf("\t      ___ (~ )( ~) \n");
    //kprintf("\t     /   \\_\\ \\/ /  \n");
    //kprintf("\t    |   D_ ]\\ \\/   \n");
    //kprintf("\t    |   D _]/\\ \\   \n");
    //kprintf("\t     \\___/ / /\\ \\  \n");
    //kprintf("\t          (_ )( _) \n");
    //kprintf("\t            ~  ~   \n");
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

/* from KR book*/
/* itoa:  convert n to characters in s  from KR book*/
char *itoa(int value, char *str, int base)
{
    char *p_str = str;

    //int index = 0;
    int n = value;
    if (0 == n) {
        *p_str++ = '0';
        *p_str = '\0';
    } else {
        while (n) {
            if (16 == base) {
                int bval = (n % base);
                if (10 == bval) {
                    *p_str = 'A';
                } else if (11 == bval) {
                    *p_str = 'B';
                } else if (12 == bval) {
                    *p_str = 'C';
                } else if (13 == bval) {
                    *p_str = 'D';
                } else if (14 == bval) {
                    *p_str = 'E';
                } else if (15 == bval) {
                    *p_str = 'F';
                } else {
                    *p_str = (n % base) + '0';
                }
            } else {
                *p_str = (n % base) + '0';
            }
            n /= base;
            p_str++;
        }
        *p_str = '\0';
        int str_len = p_str - str;
        int i = 0;
        int ch = 0;
        for (i = 0; i < (str_len / 2); i++) {
            ch = *(str + i);
            *(str + i) = *(str + str_len - 1 - i);
            *(str + str_len - 1 - i) = ch;
        }
    }
    return str;
}

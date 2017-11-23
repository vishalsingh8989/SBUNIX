#include <sys/defs.h>
#include <sys/utils.h>
#include <sys/kprintf.h>
#include <sys/string.h>
#include <sys/vmm.h>

/*
static void memcpy(void* dest, void* src, int count)
{
    uint8_t * dest_t = (uint8_t *) dest;
    uint8_t * src_t = (uint8_t *) src;

    for(int i = 0; i < count; i++)
        *dest_t++ = *src_t++;
}

static void memset(void* dest, int value, int count)
{
    uint8_t *dest_t = (uint8_t *) dest;
    for(int i = 0; i < count; i++)
        *dest_t++ = value;
}
*/

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
    kprintf("\t            _  _   \n");
    kprintf("\t      ___ (~ )( ~) \n");
    kprintf("\t     /   \\_\\ \\/ /  \n");
    kprintf("\t    |   D_ ]\\ \\/   \n");
    kprintf("\t    |   D _]/\\ \\   \n");
    kprintf("\t     \\___/ / /\\ \\  \n");
    kprintf("\t          (_ )( _) \n");
    kprintf("\t            ~  ~   \n");
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

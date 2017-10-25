#include <sys/utils.h>
#include <sys/defs.h>
#include <sys/kprintf.h>

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
    kprintf(fmt);
    kprintf("\n");
    kprintf("Kernal is Panicking!!!\n");
    kprintf("\t            _  _   \n");  
    kprintf("\t      ___ (~ )( ~) \n");
    kprintf("\t     /   \\_\\ \\/ /  \n");
    kprintf("\t    |   D_ ]\\ \\/   \n");
    kprintf("\t    |   D _]/\\ \\   \n");
    kprintf("\t     \\___/ / /\\ \\  \n");
    kprintf("\t          (_ )( _) ");
    kprintf("\t    mark    ~  ~   ");
    while(1) {
        __asm__ __volatile__ ("hlt;");
    }
}

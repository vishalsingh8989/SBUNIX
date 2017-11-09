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


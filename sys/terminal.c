#include <sys/defs.h>
#include <sys/kprintf.h>
#include <sys/process.h>

char term_buf[64];
char input_ready = 0;
volatile uint64_t term_idx = 0;
volatile uint64_t read_idx = 0;

void upd_term_buf(char c)
{
    //TODO: don't use static storage, use kmalloc for each process.

    term_buf[term_idx % 64] = c;

    if(c == BACKSPACE) {
       term_idx--;
    }
    else if(c == ENTER) {
       input_ready = 1;
       term_idx++;
       //kprintf("\nInput Ready! in Buf\n");
    }
    else {
       term_idx++;
    }

    pchar(c);
}

void term_read(uint64_t addr, uint64_t size)
{
    while(!input_ready) {
        __asm__ __volatile__("sti;");
        kprintf("");
    }
    //kprintf("Input Ready! in Read: %c\n", term_buf[read_idx % 64]);

    char * addri = (char *) addr;
    volatile char c;

    for(int i = 0; i < size; i++) {
        //*(addri+i) = term_buf[--term_idx];
        c = term_buf[read_idx++ % 64];
        *(addri+i) = c;
        if(c == ENTER) input_ready = 0;
    }
}

void term_write(uint64_t addr, uint64_t size)
{
    char * addri = (char *) addr;

    for(int i = 0; i < size; i++) {
        pchar(*addri);
        addri+=1;
    }
}

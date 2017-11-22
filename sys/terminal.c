#include <sys/defs.h>
#include <sys/kprintf.h>
#include <sys/process.h>

char term_buf[64];
volatile uint64_t term_idx = 0;

void upd_term_buf(char c)
{
    //TODO: don't use static storage, use kmalloc for each process.
    term_buf[term_idx % 64] = c;
    term_idx++;

    pchar(c);
    //kprintf("idx: %d\n", term_idx);

}

void term_read(uint64_t addr, uint64_t size)
{
    __asm__ __volatile__("sti;");
    while(term_idx < size); //TODO: change to enter pressed.

    uint64_t addri = addr;

    //kprintf("Returning char: %c at idx: %d to callee\n", term_buf[term_idx], term_idx-1);

    for(int i = 0; i < size; i++) {
        *(uint64_t *) addri++ = term_buf[--term_idx];
    }
}

void term_write(uint64_t addr, uint64_t size)
{
    uint64_t addri = addr;

    for(int i = 0; i < size; i++) {
        pchar(*(char *) addri++);
    }
}

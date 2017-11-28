#include <sys/defs.h>
#include <sys/kprintf.h>
#include <sys/process.h>

char term_buf[64];
volatile uint64_t term_idx = 0;

void upd_term_buf(char c)
{
    //TODO: don't use static storage, use kmalloc for each process.


    //if(c !=-1){
    		term_buf[term_idx % 64] = c;
    		term_idx++;
    		pchar(c);
    //}else{
    	//	term_buf[term_idx % 64] = '\0';
    //		term_idx--;
    //		backspace();
    //}


}

void term_read(uint64_t addr, uint64_t size)
{
    __asm__ __volatile__("sti;");
    while(term_idx < size); //TODO: change to enter pressed.

    char * addri = (char *) addr;

    //kprintf("Returning char: %d at idx: %d to callee\n", term_buf[term_idx], term_idx-1);

    for(int i = 0; i < size; i++) {
        *(addri+i) = term_buf[--term_idx];
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

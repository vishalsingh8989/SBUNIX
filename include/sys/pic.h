#include <sys/defs.h>

#define outb(value, port) __asm__ ("outb %%al, %%dx\n\t"::"al"(value), "dx"(port))

void init_pic();
void pic_send_eoi(uint8_t irq);

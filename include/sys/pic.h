
#define outb(value, port) __asm__ ("outb %%al, %%dx\n\t"::"al"(value), "dx"(port))

void init_pic();

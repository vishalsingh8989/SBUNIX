#include <sys/pic.h>
#include <sys/defs.h>

//From osdev
void pic_send_eoi(unsigned char irq) {
    if(irq >= 8)
        outb(0xa0, 0x20);
    else
        outb(0x20, 0x20);
}

void init_pic() {

    unsigned char a1, a2;

    a1 = inb(0x21);
    a2 = inb(0xa1);

    outb(0x20, 0x11);
    outb(0xa0, 0x11);
    outb(0x21, 0x20); 
    outb(0xa1, 0x28);
    outb(0x21, 0x04);
    outb(0xa1, 0x02);
    outb(0x21, 0x01);
    outb(0xa1, 0x01);

    outb(0x21, a1);
    outb(0xa1, a2);

}

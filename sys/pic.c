#include <sys/pic.h>

//From osdev
void init_pic() {
    outb(0x11, 0x20);
    outb(0x11, 0xa0);
    /*Remaps IRQ0-IRQ7 to 0x20-0x27 in interrupt vector table*/
    outb(0x20, 0x21); 
    /*Remaps IRQ8-IRQ15 to 0x28-0x2F in interrupt vector table*/
    outb(0x28, 0xa1);
    /*PIC2 is connected to PIC1 via IRQ2*/
    outb(0x04, 0x21);
    outb(0x02, 0xa1);
    /*Enables 8086/88 mode*/
    outb(0x01, 0x21);
    outb(0x01, 0xa1);
    /*Disables all interrupts from IRQ0-IRQ7*/
    outb(0xff, 0x21);
    /*Disables all interrupts from IRQ8-IRQ15*/
    outb(0xff, 0xa1);
}

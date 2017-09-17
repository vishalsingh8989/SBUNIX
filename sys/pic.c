#include <sys/pic.h>
#include <sys/defs.h>
#include<sys/kprintf.h>
//From osdev
void pic_send_eoi(uint8_t irq) {
    //kprintf("int %d !!", irq); 
    if(irq >= 8)
        outb(0xa0, 0x20);
     else
        outb(0x20, 0x20);
}

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
}

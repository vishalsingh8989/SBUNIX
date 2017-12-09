#include <sys/defs.h>
#include <sys/idt.h>
#include <sys/kprintf.h>

//Note: From OSdev.
struct idte_t {
   uint16_t offset_1; // offset bits 0..15
   uint16_t selector; // a code segment selector in GDT or LDT
   uint8_t ist;       // bits 0..2 holds Interrupt Stack Table offset, rest of bits zero.
   uint8_t type_attr; // type and attributes
   uint16_t offset_2; // offset bits 16..31
   uint32_t offset_3; // offset bits 32..63
   uint32_t zero;     // reserved
}__attribute__((packed));

struct idtr_t {
    uint16_t size;
    uint64_t addr;
}__attribute__((packed));

static struct idte_t idt[256];

static struct idtr_t idtr = {sizeof(idt), (uint64_t)idt};

extern void _isr0();
extern void _isr1();
extern void _isr2();
extern void _isr3();
extern void _isr4();
extern void _isr5();
extern void _isr6();
extern void _isr7();
extern void _isr8();
extern void _isr9();
extern void _isr10();
extern void _isr11();
extern void _isr12();
extern void _isr13();
extern void _isr14();
extern void _isr16();
extern void _isr17();
extern void _isr18();
extern void _isr19();
extern void _isr20();
extern void _isr32();
extern void _isr33();
extern void _isr128();
extern void _isrxx();

void set_idt(uint8_t intr_no, uint64_t func, uint8_t type_attr) {
    idt[intr_no].offset_1  = (func >>  0) & 0x0000ffff;
    idt[intr_no].selector  = 0x8;
    idt[intr_no].ist       = 0x0;
    idt[intr_no].type_attr = type_attr;
    idt[intr_no].offset_2  = (func >> 16) & 0x0000ffff;
    idt[intr_no].offset_3  = (func >> 32) & 0xffffffff;
    idt[intr_no].zero      = 0x0;
}

void init_idt() {

    klog(BOOTLOG, "Set up Interrrupt descriptor table.\n");
    for(int i = 0; i < 256; i++) {
       set_idt(i, (uint64_t) &_isrxx  , 0x8e);
    }

    set_idt(  0,  (uint64_t) &_isr0   , 0x8e);
    set_idt(  1,  (uint64_t) &_isr1   , 0x8e);
    set_idt(  2,  (uint64_t) &_isr2   , 0x8e);
    set_idt(  3,  (uint64_t) &_isr3   , 0x8e);
    set_idt(  4,  (uint64_t) &_isr4   , 0x8e);
    set_idt(  5,  (uint64_t) &_isr5   , 0x8e);
    set_idt(  6,  (uint64_t) &_isr6   , 0x8e);
    set_idt(  7,  (uint64_t) &_isr7   , 0x8e);
    set_idt(  8,  (uint64_t) &_isr8   , 0x8e);
    set_idt(  9,  (uint64_t) &_isr9   , 0x8e);
    set_idt( 10,  (uint64_t) &_isr10  , 0x8e);
    set_idt( 11,  (uint64_t) &_isr11  , 0x8e);
    set_idt( 12,  (uint64_t) &_isr12  , 0x8e);
    set_idt( 13,  (uint64_t) &_isr13  , 0x8e);
    set_idt( 14,  (uint64_t) &_isr14  , 0x8e);
    set_idt( 16,  (uint64_t) &_isr16  , 0x8e);
    set_idt( 17,  (uint64_t) &_isr17  , 0x8e);
    set_idt( 18,  (uint64_t) &_isr18  , 0x8e);
    set_idt( 19,  (uint64_t) &_isr19  , 0x8e);
    set_idt( 20,  (uint64_t) &_isr20  , 0x8e);
    set_idt( 32,  (uint64_t) &_isr32  , 0x8e);
    set_idt( 33,  (uint64_t) &_isr33  , 0x8e);
    set_idt( 128, (uint64_t) &_isr128 , 0xee);

    __asm__ __volatile__("lidt  %0\n\t"::"m"(idtr));
    klog(BOOTLOG, "Set up Interrrupt descriptor table : Successful\n");
}

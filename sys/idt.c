#include <sys/defs.h>
#include <sys/idt.h>

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
extern void _isr32();
extern void _isr33();
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

    for(int i = 0; i < 256; i++) {
       set_idt(i, (uint64_t) &_isrxx  , 0x8e); 
    }

    set_idt(  0, (uint64_t) &_isr0  , 0x8e); 
    set_idt( 32, (uint64_t) &_isr32 , 0x8e); 
    set_idt( 33, (uint64_t) &_isr33 , 0x8e); 
    __asm__ __volatile__("lidt  %0\n\t"::"m"(idtr));
}


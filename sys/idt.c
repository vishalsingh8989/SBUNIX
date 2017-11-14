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
extern void _isr1();
extern void _isr4();
extern void _isr6();
extern void _isr8();
extern void _isr12();
extern void _isr14();
extern void _isr17();
extern void _isr32();
extern void _isr33();
extern void _isr128();
extern void _isr129();
extern void _isr130();
extern void _isr131();
extern void _isr132();
extern void _isr133();
extern void _isr134();
extern void _isr135();
extern void _isr136();
extern void _isr137();
extern void _isr138();
extern void _isr139();
extern void _isr140();
extern void _isr141();
extern void _isr142();
extern void _isr143();
extern void _isr144();
extern void _isr145();
extern void _isr146();
extern void _isr147();
extern void _isr148();
extern void _isr149();
extern void _isr150();
extern void _isr151();
extern void _isr152();
extern void _isr153();
extern void _isr154();
extern void _isr155();
extern void _isr156();
extern void _isr157();
extern void _isr158();
extern void _isr159();
extern void _isr160();
extern void _isr161();
extern void _isr162();
extern void _isr163();
extern void _isr164();
extern void _isr165();
extern void _isr166();
extern void _isr167();
extern void _isr168();
extern void _isr169();
extern void _isr170();
extern void _isr171();
extern void _isr172();
extern void _isr173();
extern void _isr174();
extern void _isr175();
extern void _isr176();
extern void _isr177();
extern void _isr178();
extern void _isr179();
extern void _isr180();
extern void _isr181();
extern void _isr182();
extern void _isr183();
extern void _isr184();
extern void _isr185();
extern void _isr186();
extern void _isr187();
extern void _isr188();
extern void _isr189();
extern void _isr190();
extern void _isr191();
extern void _isr192();
extern void _isr193();
extern void _isr194();
extern void _isr195();
extern void _isr196();
extern void _isr197();
extern void _isr198();
extern void _isr199();

extern void _isrxx();

void set_idt(uint8_t intr_no, uint64_t func, uint8_t type_attr) {
    idt[intr_no].offset_1  = (func >>  0) & 0x0000ffff;
    idt[intr_no].selector  = 0x8;
    idt[intr_no].ist       = 0;
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

    set_idt( 128, (uint64_t) &_isr128 , 0xee);

    set_idt( 129, (uint64_t) &_isr129 , 0xee);
    set_idt( 130, (uint64_t) &_isr130 , 0xee);
    set_idt( 131, (uint64_t) &_isr131 , 0xee);
    set_idt( 132, (uint64_t) &_isr132 , 0xee);
    set_idt( 133, (uint64_t) &_isr133 , 0xee);
    set_idt( 134, (uint64_t) &_isr134 , 0xee);
    set_idt( 135, (uint64_t) &_isr135 , 0xee);
    set_idt( 136, (uint64_t) &_isr136 , 0xee);
    set_idt( 137, (uint64_t) &_isr137 , 0xee);
    set_idt( 138, (uint64_t) &_isr138 , 0xee);
    set_idt( 139, (uint64_t) &_isr139 , 0xee);
    set_idt( 140, (uint64_t) &_isr140 , 0xee);
    set_idt( 141, (uint64_t) &_isr141 , 0xee);
    set_idt( 142, (uint64_t) &_isr142 , 0xee);
    set_idt( 143, (uint64_t) &_isr143 , 0xee);
    set_idt( 144, (uint64_t) &_isr144 , 0xee);
    set_idt( 145, (uint64_t) &_isr145 , 0xee);
    set_idt( 146, (uint64_t) &_isr146 , 0xee);
    set_idt( 147, (uint64_t) &_isr147 , 0xee);
    set_idt( 148, (uint64_t) &_isr148 , 0xee);
    set_idt( 149, (uint64_t) &_isr149 , 0xee);
    set_idt( 150, (uint64_t) &_isr150 , 0xee);
    set_idt( 151, (uint64_t) &_isr151 , 0xee);
    set_idt( 152, (uint64_t) &_isr152 , 0xee);
    set_idt( 153, (uint64_t) &_isr153 , 0xee);
    set_idt( 154, (uint64_t) &_isr154 , 0xee);
    set_idt( 155, (uint64_t) &_isr155 , 0xee);
    set_idt( 156, (uint64_t) &_isr156 , 0xee);
    set_idt( 157, (uint64_t) &_isr157 , 0xee);
    set_idt( 158, (uint64_t) &_isr158 , 0xee);
    set_idt( 159, (uint64_t) &_isr159 , 0xee);
    set_idt( 160, (uint64_t) &_isr160 , 0xee);
    set_idt( 161, (uint64_t) &_isr161 , 0xee);
    set_idt( 162, (uint64_t) &_isr162 , 0xee);
    set_idt( 163, (uint64_t) &_isr163 , 0xee);
    set_idt( 164, (uint64_t) &_isr164 , 0xee);
    set_idt( 165, (uint64_t) &_isr165 , 0xee);
    set_idt( 166, (uint64_t) &_isr166 , 0xee);
    set_idt( 167, (uint64_t) &_isr167 , 0xee);
    set_idt( 168, (uint64_t) &_isr168 , 0xee);
    set_idt( 169, (uint64_t) &_isr169 , 0xee);
    set_idt( 170, (uint64_t) &_isr170 , 0xee);
    set_idt( 171, (uint64_t) &_isr171 , 0xee);
    set_idt( 172, (uint64_t) &_isr172 , 0xee);
    set_idt( 173, (uint64_t) &_isr173 , 0xee);
    set_idt( 174, (uint64_t) &_isr174 , 0xee);
    set_idt( 175, (uint64_t) &_isr175 , 0xee);
    set_idt( 176, (uint64_t) &_isr176 , 0xee);
    set_idt( 177, (uint64_t) &_isr177 , 0xee);
    set_idt( 178, (uint64_t) &_isr178 , 0xee);
    set_idt( 179, (uint64_t) &_isr179 , 0xee);
    set_idt( 180, (uint64_t) &_isr180 , 0xee);
    set_idt( 181, (uint64_t) &_isr181 , 0xee);
    set_idt( 182, (uint64_t) &_isr182 , 0xee);
    set_idt( 183, (uint64_t) &_isr183 , 0xee);
    set_idt( 184, (uint64_t) &_isr184 , 0xee);
    set_idt( 185, (uint64_t) &_isr185 , 0xee);
    set_idt( 186, (uint64_t) &_isr186 , 0xee);
    set_idt( 187, (uint64_t) &_isr187 , 0xee);
    set_idt( 188, (uint64_t) &_isr188 , 0xee);
    set_idt( 189, (uint64_t) &_isr189 , 0xee);
    set_idt( 190, (uint64_t) &_isr190 , 0xee);
    set_idt( 191, (uint64_t) &_isr191 , 0xee);
    set_idt( 192, (uint64_t) &_isr192 , 0xee);
    set_idt( 193, (uint64_t) &_isr193 , 0xee);
    set_idt( 194, (uint64_t) &_isr194 , 0xee);
    set_idt( 195, (uint64_t) &_isr195 , 0xee);
    set_idt( 196, (uint64_t) &_isr196 , 0xee);
    set_idt( 197, (uint64_t) &_isr197 , 0xee);
    set_idt( 198, (uint64_t) &_isr198 , 0xee);
    set_idt( 199, (uint64_t) &_isr199 , 0xee);


    __asm__ __volatile__("lidt  %0\n\t"::"m"(idtr));
}


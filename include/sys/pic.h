#ifndef _PIC_H
#define _PIC_H

#include <sys/defs.h>
#include <sys/asm_utils.h>

void init_pic();
void pic_send_eoi(uint8_t irq);
#endif

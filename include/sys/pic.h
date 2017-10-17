#ifndef _PIC_H
#define _PIC_H

#include <sys/defs.h>
#include <sys/asm_utils.h>

#define		PIC_DIV_ZERO_IRQ		0
#define		PIC_TIMER_IRQ		32
#define		PIC_KEYBOARD_IRQ		33



void init_pic();
void pic_send_eoi(uint8_t irq);
#endif

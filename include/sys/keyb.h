#ifndef _KEYB_H
#define _KEYB_H

#include <sys/asm_utils.h>
#include <sys/kprintf.h>

//Note: for osdev
// 27 - escape
// 8  - backspace
// 9  - tab
// 10 - enter
// 17 - left cntl
// 18 - left/right shift
// 19 - left/right alt
// 20 - Capslock
char keymap[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=',
    8,   9, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']',
   10,  17, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
   18, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 18, '*',
   19, ' ', 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '7', '8', '9', '-',
   '4', '5', '6', '+', '1', '2', '3', '0', '.'};

int kread() {
   unsigned int c = 0;
   do {
       if(inb(0x64) & 0x1) {
           c = inb(0x60);
           if (c > 0) return c;
       }
   }
   while(1);
}

char getchar() {
   static int shift_pressed = 0;
   static int caps_pressed = 0;
   static int caps_on = 0;

   int c = kread();
   //kprintf("c: %d\n", c);

   char result;

   switch (c) {
       case 0x2A:
       case 0x36:
           shift_pressed = 1;
           return 0;
           break;

       case 0xAA:
       case 0xB6:
           shift_pressed = 0;
           return 0;
           break;

       case 0x3A:
       case 0xBA:
           caps_pressed++;
           if (!(caps_pressed % 2))
               caps_on = !caps_on;
           return 0;
           break;

       case 0x1D:
       case 0x9D:
           return keymap[29];

       default:
           result = (shift_pressed || caps_on) ? keymap[c]-32 : keymap[c];
           shift_pressed = 0;
   }

   if (c & 0x80)
       return 0;
   else
       return result;
}

#endif

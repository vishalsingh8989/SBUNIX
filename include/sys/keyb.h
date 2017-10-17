
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


/*
 * os dev
 *
 */
 void update_cursor(int row, int col)
 {
    unsigned short position=(row*MAX_X) + col;

    // cursor LOW port to vga INDEX register
    outb(0x3D4, 0x0F);
    outb(0x3D5, (unsigned char)(position&0xFF));
    // cursor HIGH port to vga INDEX register
    outb(0x3D4, 0x0E);
    outb(0x3D5, (unsigned char )((position>>8)&0xFF));
 }

char kread() {
   char c = 0;
   do {
       if(inb(0x60) != c) {
           c = inb(0x60);
           if (c > 0) return c;
       }
   }
   while(1);
}

char getchar(int c) {
    static int shift_pressed = 0;
    static int caps_on = 0;

    char result;
    switch (c) {
       case 42:
            shift_pressed = 1;
            return 0;
       case 170:
            shift_pressed = 0;
            return 0;
       case 58:
            return 0;
       case 186:
            caps_on = !caps_on;
            return 0;
       case 29: //control key
       case 157:
            return keymap[29];
       case 0x48: //up arrow
       case 0x4B://left arrow
       case 0x4D: //right arrow
       case 0x50://down arrow
    	   	   //kprintf("  arrow key pressed !!\n");
    	   	   //TODO update cursor on arrow key pressed
    	   	   return 0;
       default:
            //for debugging
            //kprintf("  indefault !!");
            //if(shift_pressed){kprintf("  shift_pressed!! ");}else{kprintf("  shift_not pressed !! ");}
            
            //if(caps_on){kprintf("  caps_pressed!! ");}else{kprintf("  caps_on_not pressed !! ");}
            
            result = (shift_pressed || caps_on) ? keymap[c-128]-32 : keymap[c-128];
    }

    return result;
}

#endif

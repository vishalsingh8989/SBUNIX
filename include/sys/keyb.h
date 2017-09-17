#include <sys/asm_utils.h>

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
       default:
            //for debugging
            //kprintf("  indefault !!");
            //if(shift_pressed){kprintf("  shift_pressed!! ");}else{kprintf("  shift_not pressed !! ");}
            
            //if(caps_on){kprintf("  caps_pressed!! ");}else{kprintf("  caps_on_not pressed !! ");}
            
            result = (shift_pressed || caps_on) ? keymap[c-128]-32 : keymap[c-128];
    }

    return result;
}


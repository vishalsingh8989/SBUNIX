#include <sys/kprintf.h>
#include <sys/keyb.h>
#include <sys/defs.h>
#include <sys/pic.h>

void timer_int_handler() {
   static int i = 0, s = 0, m = 0, h = 0;

   i++;
   if(i == 18) {
       s++;
       i = 0;
   }
   if(s == 60) {
       m++;
       s = 0;
   }
   if(m == 60) {
       h++;
       m = 0;
   }
   if(h == 24) {
       h = 0;
   }

   //kprintf("i: %d, s: %d, m: %d, h:%d\n", i, s, m, h);

   char sl = (char) (s%10+48);
   char sh = (char) (s/10+48);
   char ml = (char) (m%10+48);
   char mh = (char) (m/10+48);
   char hl = (char) (h%10+48);
   char hh = (char) (h/10+48);

   pchar_xy(sl , GREEN, 79, 24);
   pchar_xy(sh , GREEN, 78, 24);
   pchar_xy(':', GREEN, 77, 24);
   pchar_xy(ml , GREEN, 76, 24);
   pchar_xy(mh , GREEN, 75, 24);
   pchar_xy(':', GREEN, 74, 24);
   pchar_xy(hl , GREEN, 73, 24);
   pchar_xy(hh , GREEN, 72, 24);

   pic_send_eoi(0);
}

void div0_int_handler() {
   static int i = 0;
   kprintf("Div0 Interrupt Fired!...%d\n", i);
   i++;
}

void keyboard_int_handler() {
    char c;

    //kprintf("there was a key board interrupt, c: %c\n", c);

    //c = inb(0x60);
    c = getchar();
    if (c != 0) {
        if (c == 17){
          pchar_xy('[', RED, 68, 24);
          pchar_xy('^', RED, 69, 24);
          pchar_xy('C', RED, 70, 24);
          pchar_xy(']', RED, 71, 24);
        }
        else {
          pchar_xy('[', RED, 68, 24);
          pchar_xy(' ', RED, 69, 24);
          pchar_xy(c  , RED, 70, 24);
          pchar_xy(']', RED, 71, 24);
        }
    }

    pic_send_eoi(1);
}

void default_int_handler() {
//    kprintf("Unknown interrupt happened.\n");
//    kprintf("Unknown interrupt happened.\n");
//    kprintf("Unknown interrupt happened..\n");
//    kprintf("Unknown interrupt happened..\n");
//    kprintf("Unknown interrupt happened...\n");
//    kprintf("Unknown interrupt happened...\n");
//    kprintf("Unknown interrupt happened...\n");
}

void getcwd_int_handler(){
    kprintf("getcwd_int_handler  happened.\n");
    kprintf("getcwd_int_handler  happened......\n");


}

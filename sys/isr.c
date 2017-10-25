#include <sys/kprintf.h>
#include <sys/keyb.h>
#include <sys/defs.h>
#include <sys/pic.h>
#include <sys/utils.h>

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
   kpanic("-- Div0 Interrupt Fired --\n");
}

void keyboard_int_handler() {
    char c;

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

void debug_excep_handler() {
    kpanic("-- Debug Exception Fired --");
}

void overflow_handler() {
    kpanic("-- Overflow Exception Fired --");
}

void invalid_opcode_handler() {
    kpanic("-- Invalid Opcode Exception Fired --");
}

void double_fault_handler() {
    kpanic("-- Double Fault Exception Fired --");
}

void stack_fault_handler() {
    kpanic("-- Stack Fault Exception Fired --");
}

void alignment_check_handler() {
    kpanic("-- Alignment Check Exception Fired --");
}

void page_fault_handler() {
    kpanic("-- Page Fault Execption Fired --");
}

void default_int_handler() {
    kpanic("-- Unknown Interrupt Fired --");
}

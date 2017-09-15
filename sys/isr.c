#include <sys/kprintf.h>

void timer_int_handler() {
   static int i = 0;
   kprintf("Timer Interrupt Fired!...%d\n", i);
   i++;
}

void div0_int_handler() {
   static int i = 0;
   kprintf("Div0 Interrupt Fired!...%d\n", i);
   i++;
}

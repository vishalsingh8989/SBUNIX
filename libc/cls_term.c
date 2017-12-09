#include <stdio.h>
#include <sys/defs.h>
#include <sys/syscall.h>
#include <unistd.h>

int  cls_term(){
	syscall_0(__NR_clearterm);
	return 0;
}

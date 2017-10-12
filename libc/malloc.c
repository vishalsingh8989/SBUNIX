#include<malloc.h>
#include<sys/defs.h>
#include<sys/syscall.h>
#include<sys/kprintf.h>
#include<stdlib.h>



void *malloc(size_t size) {
	  //kprintf("malloc.c called");
	  void *ptr_curr = (void *)syscall_1(__NR_brk, 0) ;
	  void *ptr_new = (void *)syscall_1(__NR_brk, size) ;
	  if (ptr_new == (void*) -1) {
	    return NULL; // failed.
	  } else {
	    return ptr_curr;
	  }
	}



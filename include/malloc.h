#ifndef _MALLOC_H
#define _MALLOC_H

#include<sys/defs.h>
#define PROT_READ      1
#define PROT_WRITE     2
#define MAP_PRIVATE    0x002
#define MAP_ANON       0x020
#define MAP_GROWSDOWN  0x100 //not required as of now -vj
#define SYS_MMAP       9



void *malloc(size_t size);

#endif

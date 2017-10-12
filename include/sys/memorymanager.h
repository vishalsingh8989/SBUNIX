#ifndef _MEMORY_H
#define _MEMORY_H

#include <sys/defs.h>
#define NUM_PAGES	24000
#define BASE_MEM   	0xffffffff00000000
#define FREE_BLOCK 	0
#define ALLOCATED	1



void init_pages(list_node **h, uint64_t mem_start, uint64_t size, uint64_t num_pages);

#endif

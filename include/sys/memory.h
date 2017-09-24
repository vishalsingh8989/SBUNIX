#ifndef _MEMORY_H
#define _MEMORY_H

#include<sys/defs.h>
#define NUM_PAGES	24000

/*single page entry
 *
 */

typedef struct page_node {
    void *start;
    void *end;
    struct page_node *next;
    struct page_node *prev;
}page_node;

/* struct to keep track of all memory.
 */
typedef struct memory_map{

	uint64_t kernel_start;
	uint64_t kernel_end;
	uint64_t screen_start;
	uint64_t screen_end;
	uint64_t ahci_start;
	uint64_t ahci_end;
} memory_map;



void init_kernel_memory()

#endif

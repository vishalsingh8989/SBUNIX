#ifndef __PHY_MM_H_
#define __PHY_MM_H_

#include <sys/defs.h>

//OSDev
#define	PTE_PRESENT			1		//0000000000000000000000000000001
#define	PTE_WRITABLE			2		//0000000000000000000000000000010
#define	PTE_USER				4		//0000000000000000000000000000100
#define	PTE_WRITETHOUGH		8		//0000000000000000000000000001000
#define	PTE_NOT_CACHEABLE	0x10		//0000000000000000000000000010000
#define	PTE_ACCESSED			0x20		//0000000000000000000000000100000
#define	PTE_DIRTY			0x40		//0000000000000000000000001000000
#define	PTE_PAT				0x80		//0000000000000000000000010000000
#define	PTE_CPU_GLOBAL		0x100		//0000000000000000000000100000000
#define	PTE_LV4_GLOBAL		0x200		//0000000000000000000001000000000
#define PTE_FRAME	 		0x7FFFF000 	//1111111111111111111000000000000

#define FREE_BLOCK			0
#define ALLOCATED			1
#define PAGE_SIZE			4096


typedef struct pd {
	uint32_t start;
	uint32_t end;
	struct pd *next;
	struct pd *prev;
	int state;// 0 free . 1 not free
} __attribute__((__packed__)) page_desc;



void memset_kernel(void *dest, int c, uint64_t count);
void load_cr3(uint64_t val);
uint64_t read_cr0();
uint64_t read_cr3();
uint64_t phys_address(uint64_t* address);
page_desc* get_free_pages(page_desc** head,uint32_t num_of_pages);
void init_pages(page_desc **h, uint64_t mem_start, uint64_t size, uint64_t num_pages);

void* alloc_mem(uint64_t size);
void print_frames();
#endif

#ifndef _VMM_H
#define _VMM_H

#include <sys/defs.h>

#define PAGE_SIZE 4096
#define KERNAL_BASE_ADDRESS 0xffffffff80000000

//From 4.13.5 linux kernal source.
#define _PAGE_BIT_PRESENT	  0	    /* is present */
#define _PAGE_BIT_RW		  1	    /* writeable */
#define _PAGE_BIT_USER		  2	    /* userspace addressable */
#define _PAGE_BIT_PWT		  3	    /* page write through */
#define _PAGE_BIT_PCD		  4	    /* page cache disabled */
#define _PAGE_BIT_ACCESSED	  5	    /* was accessed (raised by CPU) */
#define _PAGE_BIT_DIRTY		  6	    /* was written to (raised by CPU) */
#define _PAGE_BIT_PSE		  7	    /* 4 MB (or 2MB) page */
#define _PAGE_BIT_PAT		  7	    /* on 4KB pages */
#define _PAGE_BIT_GLOBAL	  8	    /* Global TLB entry PPro+ */

#define _PAGE_PRESENT	      (1 << _PAGE_BIT_PRESENT)
#define _PAGE_RW	          (1 << _PAGE_BIT_RW)
#define _PAGE_USER	          (1 << _PAGE_BIT_USER)
#define _PAGE_PWT	          (1 << _PAGE_BIT_PWT)
#define _PAGE_PCD	          (1 << _PAGE_BIT_PCD)
#define _PAGE_ACCESSED	      (1 << _PAGE_BIT_ACCESSED)
#define _PAGE_DIRTY	          (1 << _PAGE_BIT_DIRTY)
#define _PAGE_PSE	          (1 << _PAGE_BIT_PSE)
#define _PAGE_GLOBAL	      (1 << _PAGE_BIT_GLOBAL)

#define PML4_IDX(a)     ((a >> 39) & 0x1FF)
#define PDPT_IDX(a)     ((a >> 30) & 0x1FF)
#define PDE_IDX(a)      ((a >> 21) & 0x1FF)
#define PT_IDX(a)       ((a >> 12) & 0x1FF)

struct PageStat {
    struct PageStat *next;
    uint16_t ref;
}__attribute__((__packed__));

typedef struct PageStat page_stat_t;

struct page_table {
    uint64_t pte[512];
}__attribute__((aligned(0x1000)));

struct page_directory {
    uint64_t pde[512];
}__attribute__((aligned(0x1000)));

struct page_directory_pointer {
    uint64_t pdpe[512];
}__attribute__((aligned(0x1000)));

struct page_map_level_4 {
    uint64_t pml4e[512];
}__attribute__((aligned(0x20)));

void vmm_init(uint32_t *modulep, void *physbase, void *physfree);

uint64_t *  kmalloc(uint64_t size);

#endif 

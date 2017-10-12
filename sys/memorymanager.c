#include<sys/data_utils.h>
#include<sys/defs.h>
#include<sys/syscall.h>
#include<sys/memorymanager.h>
#include<sys/kprintf.h>
#include<sys/pci.h>
#include<sys/lib_utils.h>
#include<malloc.h>

extern uint64_t free_page_info;


#define KERNEL_MEM   0xffffffff80000000 //  - from linker script




uint64_t *phy_to_vir(uint64_t *addr){
	return (uint64_t *)(addr + KERNEL_MEM);
}

uint64_t *vir_to_phy(uint64_t *addr){
    return (uint64_t *) (addr - KERNEL_MEM);
}

void init_pages(page_desc **h, uint64_t mem_start, uint64_t size, uint64_t num_pages) {


	free_page_info =  mem_start;
	uint64_t  frame_start;



	memset((void *) free_page_info, 0, num_pages * sizeof(list_node));
	kprintf("Initialize physical memory....\n");


	uint16_t length = 0;
	for( frame_start  = mem_start + 40*PAGESIZE   //first 40 for bump allocator
		 ;frame_start < mem_start + size - PAGESIZE // corner case
		 ;frame_start = frame_start + PAGESIZE ){

		//kprintf("Insert frame %p in list\n", frame_start);

		add_page_desc(h, length, frame_start, frame_start+PAGESIZE );
		length++;

	}

	uint64_t* phy_kern_pt = (uint64_t *)(mem_start + 10*PAGESIZE);//alloc_mem(PAGESIZE); //bump allocator ;

	//phy_kern_pt = move_up(phy_kern_pt);// TODO aligment of 4096. alignement right now is not proper.
	uint64_t* vir_kern_pt  = phy_to_vir(phy_kern_pt);
	//kprintf("Physical memory initialized successfully.\n");

	//print_list((list_node*)mem_start);
	kprintf("Kernel page table at %p and %p.\n", phy_kern_pt, vir_kern_pt);
	//print_list(*h);

}

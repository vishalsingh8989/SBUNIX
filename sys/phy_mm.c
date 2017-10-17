#include<sys/data_utils.h>
#include<sys/defs.h>
#include<sys/syscall.h>
#include<sys/phy_mm.h>
#include<sys/kprintf.h>
#include<sys/pci.h>
#include<sys/lib_utils.h>
#include<malloc.h>

#define KERNEL_MEM   0xffffffff80000000 //  - from linker script


extern uint64_t free_page_info;
extern page_desc* page_desc_head;


//pdir*	_cur_directory=0;
uint32_t _cur_pdbr;
char physbase;


void *memset_kernel(void *dest, int c, uint64_t count)
{
    uint64_t i = 0;
    char *mem = dest;
    for (i = 0; i < count; i++) {
        if (mem)
            *mem++ = c;
    }
    return dest;
}
/*
 * load cr3 .
 * dont use 32 bit value : Error: unsupported instruction `mov'. - vj
 */
void load_cr3(uint64_t val) {

	kprintf("Load cr3\n");
	__asm__ __volatile__("mov %0, %%cr3"
			:
			: "r" (val));

}

uint64_t read_cr0(){
	   uint64_t cr0;
	    __asm__ __volatile__ (
	        "mov %%cr0, %%rax\n\t"
	        "mov %%eax, %0\n\t"
	        //"mov %%cr2, %%rax\n\t"
	        //"mov %%eax, %1\n\t"
	        //"mov %%cr3, %%rax\n\t"
	        //"mov %%eax, %2\n\t"
	    : "=m" (cr0)
	    : /* no input */
	    : "%rax"
	    );
	    return cr0;

}


uint64_t read_cr3(){
	   uint64_t cr3;
	    __asm__ __volatile__ (
	        //"mov %%cr0, %%rax\n\t"
	        //"mov %%eax, %0\n\t"
	        //"mov %%cr2, %%rax\n\t"
	        //"mov %%eax, %1\n\t"
	        "mov %%cr3, %%rax\n\t"
	        "mov %%eax, %0\n\t"
	    : "=m" (cr3)
	    : /* no input */
	    : "%rax"
	    );
	    return cr3;

}




uint64_t phys_address(uint64_t* address){

	return (uint64_t)address & PTE_FRAME;

}

uint64_t *phy_to_vir(uint64_t *addr){
	return (uint64_t *)(addr + KERNEL_MEM);
}

uint64_t *vir_to_phy(uint64_t *addr){
    return (uint64_t *) (addr - KERNEL_MEM);
}





page_desc* get_free_pages(page_desc** head, uint32_t num_of_pages){
	page_desc* temp1 = *head;
	page_desc* free_page_head = *head;

	page_desc* prev = NULL;

	uint32_t count = 0;
	while(count < num_of_pages){
		prev = free_page_head;
		free_page_head = free_page_head->next;
		count++;
	}
//	kprintf("free page start    at %p\n", temp1->start);
//	kprintf("free page end      at %p\n", prev->end);
//	kprintf("new free page head at %p\n", free_page_head->start);
//	kprintf("************************************\n");

	*head = free_page_head;
	prev->next=NULL;

	return temp1;

}



void init_pages(page_desc **h, uint64_t mem_start, uint64_t size, uint64_t num_pages) {


	free_page_info =  mem_start;
	uint64_t  frame_start;

	//uint16_t s = (num_pages * sizeof(list_node))/PAGESIZE;

	//not working ..rebooting system .
	memset((void *) free_page_info, 0, num_pages * sizeof(page_desc));

	kprintf("Initialize physical memory....\n");

	uint32_t length = 1;
	for( frame_start  = mem_start + 100*PAGESIZE   //first 100 for bump allocator
		 ;frame_start < mem_start + size - PAGESIZE // corner case
		 ;frame_start = frame_start + PAGESIZE ){
//		if(length %199 ==0 )
//			kprintf("Insert frame %p\n", frame_start);
		add_page_desc(h, length, frame_start, frame_start + PAGESIZE - 1);
		length++;

	}

	uint64_t* phy_kern_pt = (uint64_t *)(mem_start + 10*PAGESIZE);//alloc_mem(PAGESIZE); //bump allocator ;

	//phy_kern_pt = move_up(phy_kern_pt);// TODO aligment of 4096. alignement right now is not proper.
	uint64_t* vir_kern_pt  = phy_to_vir(phy_kern_pt);
	kprintf("Physical memory initialized successfully. %d\n", length);

	//print_list((list_node*)mem_start);
	kprintf("Kernel page table at %p and %p.\n", phy_kern_pt, vir_kern_pt);
	//print_list(*h);

}


//OSDEV SAMPLE
//#define PAGE_DIRECTORY_INDEX(x) (((x) >> 22) & 0x3ff)
//#define PAGE_TABLE_INDEX(x) (((x) >> 12) & 0x3ff)
//#define PAGE_GET_PHYSICAL_ADDRESS(x) (*x & ~0xfff)

uint32_t page_directory_index(uint32_t addr){
	return (addr>>22)&0x3ff;
}

uint32_t page_table_index(uint32_t addr){
	return (addr>>12)&0x3ff;
}

uint64_t page_get_physical_address(uint32_t* addr){
	return (*addr & ~0xfff);
}


//void pt_entry_set_val (pt_entry* entry, uint32_t VAL){
//	*entry = *entry | VAL;
//}
//void pt_entry_unset_val (pt_entry* entry, uint32_t VAL){
//	*entry = *entry & ~VAL;
//}
//
///*
// * mask out the frame address using PTE_FRAME
// */
//void pt_entry_set_frame(pt_entry* entry, uint32_t physical_addr){
//
//	*entry = (*entry & ~PTE_FRAME) | physical_addr;
//}
//
//
//void pd_entry_set_val (pd_entry* entry, uint32_t VAL){
//	*entry = *entry | VAL;
//}
//
//void pd_entry_unset_val(pt_entry* entry, uint32_t VAL){
//	*entry = *entry & ~VAL;
//
//}
//
//void pd_entry_set_frame(pd_entry* entry , uint32_t addr){
//	*entry = (*entry & ~PTE_FRAME) | addr; //PDE AND PTE structure is same.
//}
//
//
//
//
//
//int vmm_alloc_page(pt_entry* entry) {
//
//	//find and  allocate a free physical frame.
//	page_desc* free_page = get_free_page (&page_desc_head, 1);
//
//
//	//if free frame available then map it to the page
//	pt_entry_set_frame (entry, (uint32_t)free_page->start);
//	pt_entry_set_val (entry, PTE_PRESENT);
//
//	return true;
//}
//
//uint64_t pt_entry_pfn(pt_entry* entry){
//	return ((uint64_t)entry) & PTE_FRAME;
//}
//
//
//void vmm_free_page(pt_entry* entry) {
//
//	uint64_t p = pt_entry_pfn (entry);
//	if (p){// add in front of free list
//		add_page_desc(&page_desc_head, 1, p, p+PAGESIZE );
//	}
//
//	pt_entry_unset_val (entry, PTE_PRESENT);
//
//}
//
//pt_entry* vmm_pt_lookup_entry (ptable* pt,uint32_t addr) {
//
//	if (pt)
//		return &pt->m_entries[ page_table_index (addr) ];
//	return 0;
//}
//pd_entry* vmm_pdir_lookup_entry (pdir* pd, uint32_t addr) {
//
//	if (pd)
//		return &pd->m_entries[ page_table_index (addr) ];
//	return 0;
//}
//
//
//
//int vmm_switch_pdir(pdir* dir) {
//
//	if (!dir)
//		return false;
//
//	_cur_directory = dir;
//
//	load_cr3 (_cur_pdbr);
//	return true;
//}
//
//pdir* vmm_get_directory () {
//
//	return _cur_directory;
//}
//
//uint64_t pmm_alloc_block (int size) {
//
//	page_desc* free_page = get_free_page (&page_desc_head, size);
//
////	if (frame == -1)
////		return 0;	//out of memory
//
//	//memset (free_page->start);
//	//memset((void*)free_page->start, 0, PAGESIZE);
//
//	return free_page->start;
//
//
//}
//
//
//
//void vmm_map_page (void* phys, void* virt) {
//
//   //! get page directory
//   pdir* pdir = vmm_get_directory ();
//
//   //! get page table
//   pd_entry* entry = &pdir->m_entries [page_directory_index((uint64_t) virt) ];
//   if ( (*entry & PTE_PRESENT) != PTE_PRESENT) {
//	   ///! page table not present, allocate it
//
//		ptable* table = (ptable*) pmm_alloc_block (1);
//		if (!table){
//			return;
//		}
//
//		pd_entry* entry = &pdir->m_entries[page_directory_index ( (uint64_t) virt) ];
//
//		//! map in the table (Can also just do *entry |= 3) to enable these bits
//		pd_entry_set_val (entry, PTE_PRESENT);
//		pd_entry_set_val (entry, PTE_WRITABLE);
//		pd_entry_set_frame (entry, (uint64_t)table);
//
//
//   }
//
//   //! get table
//      ptable* table = (ptable*) page_get_physical_address ( entry );
//
//      //! get page
//      pt_entry* page = &table->m_entries [ page_table_index( (uint64_t) virt) ];
//
//      //! map it in (Can also do (*page |= 3 to enable..)
//      pt_entry_set_frame ( page, (uint64_t) phys);
//      pt_entry_set_val ( page, PTE_PRESENT);
//
//}
//
//void vmmngr_initialize (uint64_t mem_start) {
//
//	//! allocate default page table
//	dsleep();
//	uint64_t kernel_base = 0xffffffff80000000;
//	ptable* table = (ptable*) pmm_alloc_block (1);
//	kprintf("Table 1 start address %p\n", table);
//	dsleep();
//	dsleep();
//	dsleep();
//	if (!table)
//		return;
//
//	//! allocates 3gb page table
//	ptable* table2 = (ptable*) pmm_alloc_block (1);
//	kprintf("Table 2 start address %p\n", table2);
//	dsleep();
//	dsleep();
//	dsleep();
//	if (!table2)
//		return;
//
//	//! clear page table
//   //memset (table, 0, sizeof (ptable));
//
//    //1st 4mb are idenitity mapped
//	//char physbase;
//   for (int i=0, frame=0x0, virt=physbase; i<1024; i++, frame+=4096, virt+=4096) {
//
//      //! create a new page
//      pt_entry page=0;
//      pt_entry_set_val (&page, PTE_PRESENT);
//      pt_entry_set_frame (&page, frame);
//
//
//
//      //! ...and add it to the page table
//      table2->m_entries [page_table_index (virt) ] = page;
//   }
//
//   //! map 1mb to 3gb (where we are at)
//   for (int i=0, frame=mem_start + 200*PAGESIZE, virt=kernel_base; i<1024; i++, frame+=4096, virt+=4096) {
//
//      //! create a new page
//      pt_entry page=0;
//      pt_entry_set_val (&page,PTE_PRESENT);
//      pt_entry_set_frame (&page, frame);
//      kprintf("Mapp %p - %p\n", page_table_index(virt), page);
//
//      //! ...and add it to the page table
//      table->m_entries [page_table_index(virt) ] = page;
//   }
//
//   //! create default directory table
//   pdir*   dir = (pdir*) pmm_alloc_block(3);
//   if (!dir)
//      return;
//
//
//
//
//  //! clear directory table and set it as current
//  //memset (dir, 0, sizeof (pdirectory));
//
//   //! get first entry in dir table and set it up to point to our table
////   pd_entry* entry = &dir->m_entries [page_directory_index (0xc0000000) ];
////   pd_entry_set_val (entry, PTE_PRESENT);
////   pd_entry_set_val (entry, PTE_WRITABLE);
////   pd_entry_set_frame (entry, (uint64_t)table);
//
//	kprintf("Mapped succesfully  1 %p\n", page_directory_index (kernel_base) );
//	dsleep();dsleep();
//	//! get first entry in dir table and set it up to point to our table
//
//	pd_entry* entry = &dir->m_entries [page_directory_index (physbase) ];
//	pd_entry_set_val (entry, PTE_PRESENT);
//	pd_entry_set_val (entry, PTE_WRITABLE);
//	pd_entry_set_frame (entry, (uint64_t)table);
//
//	kprintf("Mapped succesfully  2 %p\n", page_directory_index (physbase) );
//	dsleep();dsleep();
//
//
//	pd_entry* entry2 = &dir->m_entries [page_directory_index (kernel_base) ];
//	pd_entry_set_val (entry2, PTE_PRESENT);
//	pd_entry_set_val (entry2, PTE_WRITABLE);
//	pd_entry_set_frame (entry2, (uint64_t)table2);
//
//
//   //! store current PDBR
//	kprintf("Mapped succesfully  3.\n");
//	dsleep();dsleep();
//   _cur_pdbr = (uint64_t) &dir->m_entries;
//
//	kprintf("Mapped succesfully  4.\n");
//	dsleep();dsleep();
//   //! switch to our page directory
//   vmm_switch_pdir(dir) ;
//
//
//
//
//}

















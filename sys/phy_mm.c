#include<sys/data_utils.h>
#include<sys/defs.h>
#include<sys/syscall.h>
#include<sys/phy_mm.h>
#include<sys/kprintf.h>
#include<sys/pci.h>
#include<sys/lib_utils.h>
#include<malloc.h>


void memset_kernel(void *dest, int c, uint64_t count)
{
    uint64_t i = 0;

	kprintf("dest  %p, c ==  %d , count == %d" ,dest, c, count );
	//dsleep();dsleep();dsleep();dsleep();
    char *mem = dest;
    for (i = 0; i < count; i++) {
    			kprintf("memset kernel %d\n", i);
           *mem++ = c;
    }

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

uint64_t *phys_to_vir(uint64_t *addr){
	return (uint64_t *)(addr + KERNEL_MEM);
}

uint64_t *vir_to_phys(uint64_t *addr){
    return (uint64_t *) (addr - KERNEL_MEM);
}



int frame_count(page_desc** head){
	page_desc* free_page_head = *head;
	int c = 0;
	while(free_page_head !=NULL )
	{
		c++;
		free_page_head = free_page_head->next;
	}
	return c;
}


page_desc* get_free_frame(page_desc** head){

	page_desc* free_page_head = *head;

	page_desc* prev = free_page_head;

	//uint32_t count = 0;
	while(free_page_head != NULL && free_page_head->state != FREE_BLOCK){
		prev = free_page_head;
		free_page_head = free_page_head->next;

	}
//	kprintf("free page start    at %p\n", temp1->start);
//	kprintf("free page end      at %p\n", prev->end);
//	kprintf("new free page head at %p\n", free_page_head->start);
//	kprintf("************************************\n");

	if(prev == *head) //first is free
	{
		*head = free_page_head->next;
		free_page_head->next = NULL;
		return free_page_head;
	}else{
		prev->next = free_page_head->next;
		free_page_head->next=NULL;
		return free_page_head;
	}



	return NULL;

}

void* alloc_phys_mem(uint64_t size){

	uint64_t start  = free_page_info;
	if (size!=0){
		free_page_info = free_page_info + size;
		return (void *)start;
	}else{
		return (void *)start; //get current using this
	}


}

/*
 * allocate mem between page_desc_head - page_desc_end
 * */
void* alloc_phys_pagedesc(uint64_t size){


	//kprintf("alloc_pagedesc %p , %p\n", alloc_pagedesc_start , (uint64_t)page_desc_end);
//	dsleep();dsleep();dsleep();
	uint64_t start  = alloc_pagedesc_start;
	if (size!=0){
		alloc_pagedesc_start = alloc_pagedesc_start + size;
		if(alloc_pagedesc_start <= (uint64_t)page_desc_end)
			return (void *)start;
		else
			return (void *)0;
	}else{
		return (void *)start; //get current using this
	}


}

void *alloc_vir_mem(uint64_t size){ //vir bump allocator -Mike - vj
    static char *vir_free;

    char *start = NULL;
    if (!vir_free) {//first time align
    		vir_free = move_4k_up((uint64_t)free_start_vir, PAGE_SIZE);
    }

    if (size == 0) {
    		return (void*)start;//currrent
    } else if (size > 0) {
    		start = vir_free;
    		vir_free += size;
    		vir_free = move_4k_up((uint64_t)vir_free, PAGE_SIZE);
    		return (void*)start;
    }
    return NULL;
}

void add_page_desc(page_desc **head, int pos, uint64_t start, uint64_t end , uint8_t state){
	page_desc *temp1,*temp2;


    temp1 = temp2 = NULL;
    int curr = 0;

    page_desc *new_node=(page_desc* ) alloc_phys_pagedesc(sizeof(page_desc)); //using  bump allocation

    //kprintf("Add frame %p at %p\n", start, (uint64_t)new_node);
    if(!new_node){
        kprintf("Memory Error\n");
        dsleep();dsleep();dsleep();
        return;
    }
    new_node->start = start;
    new_node->end = end;
    new_node->state = state; //0 for free
    temp1=*head;

    if(pos==1 || temp1==NULL){
    		new_node->next=*head;
        *head=new_node;

    }
    else{
        while(temp1!=NULL && (curr<pos)){
        		curr++;
            temp2=temp1;
            temp1=temp1->next;
        }

        new_node->next=temp2->next;
        temp2->next=new_node;
    }
}

void* move_4k_up(uint64_t addr ,uint64_t alignat ){

	uint64_t diff = ( addr % alignat);
	if (diff  == 0){
		return (void*)addr; //alreay aligned
	}
	return (void*)((addr + alignat)  - diff );

}


void init_pages(page_desc **h, uint64_t mem_start, uint64_t size, uint64_t num_pages) {

	kprintf("Initialize physical memory....\n");

	//list data will be saved in head and end in frames.. mark allocated for frames.
	page_desc_head = alloc_phys_mem(num_pages * sizeof(page_desc));
	page_desc_end  =  move_4k_up((uint64_t)alloc_phys_mem(0) , PAGESIZE);

	kprintf("Frame description  storage  at %p  - %p\n", page_desc_head, page_desc_end);


	alloc_pagedesc_start = (uint64_t)page_desc_head;

	////memset to slow -vj
	//memset_kernel((void *) page_desc_head, 0, num_pages * sizeof(page_desc));


	uint8_t state  = FREE_BLOCK;
	uint64_t length = 1;
	for(uint64_t frame_start  = mem_start
		 ;frame_start <= mem_start + size  // corner case
		 ;frame_start = frame_start + PAGESIZE ){

		if((frame_start >= (uint64_t)page_desc_head &&  frame_start<= (uint64_t)page_desc_end)
			||(frame_start>=(uint64_t)phybase_ptr   &&  frame_start<= (uint64_t)phyfree_ptr)){
			state = RESERVED;

		}else{
			state  = FREE_BLOCK;
			//kprintf("State %d\n", state);
		}

		//not required , use direct mem to store
		add_page_desc(h, length, frame_start, frame_start + PAGESIZE - 1 ,state) ;
		length++;
		//dsleep();

	}
	kprintf("Num of frames %d\n", length);

	free_start_vir = phys_to_vir(phyfree_ptr);
	kprintf("Virtual mem start %p\n", free_start_vir);
 //   table_map_phy = alloc_bytes(PAGE_SIZE);
 //   phys_pml4e = (uint64_t) kern_to_phys_addr(p_ker_pml4e);
//	uint64_t* phy_kern_pt = (uint64_t *)(mem_start + 10*PAGESIZE);//alloc_mem(PAGESIZE); //bump allocator ;
//
//	//phy_kern_pt = move_up(phy_kern_pt);// TODO aligment of 4096. alignement right now is not proper.
//	uint64_t* vir_kern_pt  = phy_to_vir(phy_kern_pt);
//	kprintf("Physical memory initialized successfully. %d\n", length);
//
//	//print_list((list_node*)mem_start);
//	kprintf("Kernel page table at %p and %p.\n", phy_kern_pt, vir_kern_pt);
	//print_list(*h);

}

void print_frames(){

	kprintf("********************\n");
	page_desc* head= page_desc_head;
	kprintf("kernel loaded %p - %p\n",phybase_ptr, phyfree_ptr);
	kprintf("Print reserved blocks\n");
    while( head != NULL) {
    		//if(head->start >= (uint64_t)phybase_ptr && head->start <=(uint64_t)phyfree_ptr){
    		if(head->state!=FREE_BLOCK){
    			dsleep();
			kprintf("info at %p -> ", &head);
			kprintf("%p - ", head->start);
			kprintf("%p ",head->end);
			kprintf("allocated %d\n", head->state);
    		}
		head=head->next;
			//if(count==5)break;
    }
    kprintf("\n");
}




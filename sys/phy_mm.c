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
page_desc* page_desc_head;
page_desc* page_desc_end;


uint64_t alloc_pagedesc_start;
uint64_t *phyfree_ptr;
uint64_t *phybase_ptr;


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

void* alloc_mem(uint64_t size){

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
void* alloc_pagedesc(uint64_t size){


	kprintf("alloc_pagedesc %p , %p\n", alloc_pagedesc_start , (uint64_t)page_desc_end);
//	dsleep();dsleep();dsleep();
	uint64_t start  = free_page_info;
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

void add_page_desc(page_desc **head, int pos, uint64_t start, uint64_t end , uint8_t state){
	page_desc *temp1,*temp2;


    temp1 = temp2 = NULL;
    int curr = 0;

    page_desc *new_node=(page_desc* ) alloc_pagedesc(sizeof(page_desc)); //using  bump allocation

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

void * move_4k_up(uint64_t addr ,uint64_t alignat ){

	uint64_t diff = ( addr % alignat);
	if (diff  == 0){
		return (void*)addr; //alreay aligned
	}
	return (void*)((addr + alignat)  - diff );

}


void init_pages(page_desc **h, uint64_t mem_start, uint64_t size, uint64_t num_pages) {

	//kprintf("Initialize physical memory....\n");

	//list data will be saved in head and end in frames.. mark allocated for frames.
	page_desc_head = alloc_mem(num_pages * sizeof(page_desc));
	page_desc_end  =  move_4k_up((uint64_t)alloc_mem(0) , PAGESIZE);

	//if(page_desc_head){}

	uint8_t state  = FREE_BLOCK;
	//uint64_t length = 1;
	for(uint64_t frame_start  = mem_start
		 ;frame_start < mem_start + size - PAGESIZE // corner case
		 ;frame_start = frame_start + PAGESIZE ){


//			kprintf("Insert frame %p\n", frame_start);

		if((frame_start >= (uint64_t)page_desc_head &&  frame_start<= (uint64_t)page_desc_end)
			||(frame_start>=(uint64_t)phybase_ptr   &&  frame_start<= (uint64_t)phyfree_ptr))
		{
			state = ALLOCATED;
			kprintf("Insert frame %p - %p, state %d\n", frame_start, frame_start + PAGESIZE - 1,state);
			//dsleep();
		}else{
			state  = FREE_BLOCK;
			//kprintf("State %d\n", state);
		}
	}

	kprintf("page_desc_ storage  at %p  - %p\n", page_desc_head, page_desc_end);
	kprintf("kernel loaded %p - %p\n",phybase_ptr, phyfree_ptr);

	//page_desc_head  = (page_desc *) move_4k_up((uint64_t) page_desc_head ,PAGESIZE );
	kprintf("Size required to store   %d\n", num_pages * sizeof(page_desc));
	alloc_pagedesc_start = (uint64_t)page_desc_head;
	kprintf("size of page desc %d\n", sizeof(page_desc));
	////memset to slow -vj
	//memset_kernel((void *) page_desc_head, 0, num_pages * sizeof(page_desc));
	kprintf("page_desc_head aligned at %p , %p\n", page_desc_head, alloc_pagedesc_start);

//
//
//		//not required , use direct mem to store
//		add_page_desc(h, length, frame_start, frame_start + PAGESIZE - 1 ,state) ;
//		length++;
//
//	}
//
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
    while(head!=NULL ){

			kprintf("info at %p -> ", &head);
			kprintf("%p - ", head->start);
			kprintf("%p ",head->end);
			kprintf("allocated %d\n", head->state);

			head=head->next;
			//if(count==5)break;
    }
    kprintf("\n");
}




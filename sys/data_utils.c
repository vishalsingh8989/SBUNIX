#include <sys/defs.h>
#include <sys/data_utils.h>
#include <sys/kprintf.h>
#include <sys/syscall.h>
#include <sys/pci.h>
#include <malloc.h>



uint64_t free_page_info;

/*
 * bump allocation
 */


int get_length(page_desc *head){
    int count= 0;
    page_desc *curr=head;
    while(curr!=NULL){
        count++;
        curr=curr->next;
    }
    return count;
}

void print_list(page_desc *head){
	int count = 0;
	kprintf("********************\n");
	page_desc* temp1= head;
    while(head!=NULL ){
    			if (temp1 !=head){
    				count++;
				kprintf("info at %p -> ", &head);
				kprintf("%p - ", head->start);
				kprintf("%p ",head->end);
				kprintf("allocated %d\n", head->state);
    			}
			head=head->next;
			if(count==5)break;
    }
    kprintf("\n");
}

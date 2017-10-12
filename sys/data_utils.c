#include <sys/defs.h>
#include <sys/data_utils.h>
#include <sys/kprintf.h>
#include <sys/syscall.h>
#include <sys/memorymanager.h>
#include <sys/pci.h>
#include <malloc.h>



uint64_t free_page_info;

/*
 * bump allocation
 */
void* alloc_mem(uint64_t size){

	uint64_t start = free_page_info;
	if (size!=0)
		free_page_info = free_page_info + size;
		return (void *)start;

}


void add_page_desc(page_desc **head, int pos, uint64_t start, uint64_t end ){
	page_desc *temp1,*temp2;
    temp1 = temp2 = NULL;
    int curr = 0;
    page_desc *new_node=(page_desc* ) alloc_mem(sizeof(page_desc)); //using  bump allocation
    if(!new_node){
        kprintf("Memory Error\n");
        return;
    }
    new_node->start = start;
    new_node->end = end;
    new_node->state = FREE_BLOCK; //0 for free
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


int get_length(list_node *head){
    int count= 0;
    list_node *curr=head;
    while(curr!=NULL){
        count++;
        curr=curr->next;
    }
    return count;
}

void print_list(list_node *head){
	kprintf("********************\n");
	list_node* temp1= head;
    while(head!=NULL ){
    			if (temp1 !=head){
				kprintf("info at %p -> ", &head);
				kprintf("%p - ", head->start);
				kprintf("%p ",head->end);
				kprintf("allocated %d\n", head->state);
    			}
			head=head->next;
    }
    kprintf("\n");
}

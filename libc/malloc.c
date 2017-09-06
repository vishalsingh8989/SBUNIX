#include<malloc.h>
#include<sys/defs.h>
#include<sys/syscall.h>
#include<stdlib.h>


void *malloc(size_t size){
/*    block_info *block = NULL;
    block = get_block(size);
    if(block == NULL)   
        return NULL;


    return (block+1); // +1 for block_info adjuestment; 
*/  
   
    size_t result =(size_t) syscall_6((uint64_t) 9, (uint64_t) NULL,
                (uint64_t) (size +sizeof(block_info)),
                (uint64_t) (PROT_READ | PROT_WRITE),
                (uint64_t) (MAP_ANON | MAP_PRIVATE),
                (uint64_t) - 1, (uint64_t) 0);
    block_info *new_block = (block_info *) result;
    new_block->size = (size + sizeof(block_info));

    return (void *) (result + sizeof(block_info));


}

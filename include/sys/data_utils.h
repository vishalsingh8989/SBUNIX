#ifndef _DATA_UTILS_H_
#define _DATA_UTILS_H_
#include<sys/defs.h>
#include<sys/phy_mm.h>




void add_page_desc(page_desc **head, int pos, uint64_t start, uint64_t end );
void print_list(page_desc *head);
int get_length(page_desc *head);

void* alloc_mem(uint64_t size);
#endif

#ifndef _DATA_UTILS_H_
#define _DATA_UTILS_H_
#include<sys/defs.h>


typedef struct list_node{
    uint64_t start;
    uint64_t end;
    struct list_node *next;
    struct list_node *prev;
    int state;// 0 free . 1 not free
} __attribute__((__packed__)) list_node;

typedef list_node page_desc;

void add_page_desc(list_node **head, int pos, uint64_t start, uint64_t end );
void print_list(list_node *head);
int get_length(list_node *head);

void* alloc_mem(uint64_t size);
#endif

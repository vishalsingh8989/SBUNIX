#ifndef _MM_TYPES_H
#define _MM_TYPES_H

#include <sys/defs.h>
#include <sys/fs.h>

struct mm_struct {
    int count;
    uint64_t pml4;
    uint64_t context;
    uint64_t start_code, end_code, start_data, end_data;
    uint64_t start_brk, brk, start_stack, start_mmap;
    uint64_t arg_start, arg_end, env_start, env_end;
    uint64_t rss, total_vm, locked_vm;
    uint64_t def_flags;
    struct vm_area_struct * mmap;
};

struct vm_area_struct {
    struct mm_struct * vm_mm;
    struct vm_area_struct * vm_next;
    struct file * file;
    uint64_t vm_start;
    uint64_t vm_end;
    uint64_t vm_page_prot;
    uint64_t vm_flags;
};

#endif

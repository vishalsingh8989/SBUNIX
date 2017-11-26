#ifndef _MM_TYPES_H
#define _MM_TYPES_H

#include <sys/defs.h>
#include <sys/fs.h>

#define VM_FILE  0
#define VM_HEAP  1
#define VM_STACK 2

typedef struct file {
    uint64_t f_start;
    uint64_t f_size;
    uint64_t f_pgoff;
    uint64_t f_bss_size;
} file_t;

typedef struct mm_struct {
    int count;
    uint64_t pml4;
    uint64_t context;
    uint64_t start_code, end_code, start_data, end_data;
    uint64_t start_brk, brk, start_stack, start_mmap;
    uint64_t arg_start, arg_end, env_start, env_end;
    uint64_t rss, total_vm, locked_vm;
    uint64_t def_flags;
    struct vm_area_struct * mmap;
} mm_struct_t;

typedef struct vm_area_struct {
    struct mm_struct * vm_mm;
    struct vm_area_struct * vm_next;
    file_t * file;
    uint64_t vm_type;
    uint64_t vm_start;
    uint64_t vm_end;
    uint64_t vm_page_prot;
    uint64_t vm_flags;
} vm_area_struct_t;

#endif

#include <stdio.h>
#include <sys/defs.h>
#include <sys/syscall.h>
#include <sys/vmm.h>
#include <unistd.h>

typedef struct vm_struct {
    size_t size;
    struct vm_struct *next;
} vm_struct_t;

void *malloc(size_t size)
{
    size_t alloc_size = sizeof(vm_struct_t) + size;
    size_t addr =
        (size_t) syscall_6((uint64_t) __NR_munmap, (uint64_t) NULL,
                (uint64_t) alloc_size,
                (uint64_t) (PROT_READ | PROT_WRITE),
                (uint64_t) (MAP_ANONYMOUS | MAP_PRIVATE),
                (uint64_t) - 1, (uint64_t) 0);
    vm_struct_t *mem_block = (vm_struct_t *) addr;
    mem_block->size = alloc_size;
    return (void *) (addr + sizeof(vm_struct_t));
}

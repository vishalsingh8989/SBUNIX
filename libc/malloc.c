#include <stdio.h>
#include <sys/defs.h>
#include <sys/syscall.h>
#include <sys/mm_types.h>


#define PROT_READ	0x1	/* page can be read */
#define PROT_WRITE	0x2	/* page can be written */
#define PROT_EXEC	0x4	/* page can be executed */
#define PROT_SEM	0x8	/* page may be used for atomic ops */
#define PROT_NONE	0x0	/* page can not be accessed */
#define PROT_GROWSDOWN	0x01000000	/* mprotect flag: extend change to start of growsdown vma */
#define PROT_GROWSUP	0x02000000	/* mprotect flag: extend change to end of growsup vma */

#define MAP_SHARED	0x01	/* Share changes */
#define MAP_PRIVATE	0x02	/* Changes are private */
#define MAP_TYPE	0x0f	/* Mask for type of mapping */
#define MAP_FIXED	0x10	/* Interpret addr exactly */
#define MAP_ANONYMOUS	0x20	/* don't use a file */


void *malloc(size_t size)
{
//    size_t alloc_size = sizeof(vm_area_struct_t) + size;
//    size_t addr =
//        (size_t) syscall_6((uint64_t) SYS_mmap, (uint64_t) NULL,
//                (uint64_t) alloc_size,
//                (uint64_t) (PROT_READ | PROT_WRITE),
//                (uint64_t) (MAP_ANONYMOUS | MAP_PRIVATE),
//                (uint64_t) - 1, (uint64_t) 0);
//    vm_struct_t *mem_block = (vm_struct_t *) addr;
//    mem_block->size = alloc_size;
//    return (void *) (addr + sizeof(vm_struct_t));
	return NULL;
}

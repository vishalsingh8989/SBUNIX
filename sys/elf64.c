#include <sys/defs.h>
#include <sys/elf64.h>
#include <sys/vmm.h>
#include <sys/tarfs.h>
#include <sys/utils.h>
#include <sys/kprintf.h>
#include <sys/string.h>
#include <sys/process.h>
#include <sys/mm_types.h>
#include <sys/fs.h>

uint64_t atoi(char *num)
{
    int i = 0;
    uint64_t inum = 0;

    while(num[i] >= '0' && num[i] <= '9') {
        inum = inum*10 + (num[i] - '0');
        i++;
    }

    return inum;
}

uint64_t otod(uint64_t onum)
{
    uint64_t dnum = 0;
    int base = 1;

    for(uint64_t otemp = onum; otemp != 0; otemp/=10) {
        dnum += (otemp % 10) * base;
        base = base * 8;
    }

    return dnum;
}

//TODO: replace this with tarfs_open()
void *get_bin_addr(const char *fname)
{
    struct posix_header_ustar *tstart = (struct posix_header_ustar *) &_binary_tarfs_start;

    if(fname == NULL) {
        return NULL;
    }

    uint64_t *rp = (uint64_t *) tstart;

    while(rp[0] || rp[1] || rp[2]) {
        if(!strcmp(tstart->name, fname)) {
            return (void *) tstart;
        }
        else {
            uint64_t inum = atoi(tstart->size);
            inum = otod(inum);

            if(inum%512) inum = ((inum/512)*512)+512;

            tstart = (struct posix_header_ustar *) ((uint64_t) tstart + inum + sizeof(struct posix_header_ustar));

            rp = (uint64_t *) tstart;

        }
    }

    return NULL;
}

void allocate_heap(task_struct_t *task)
{
    vm_area_struct_t *vma = (vm_area_struct_t *) kmalloc(PAGE_SIZE);

    vm_area_struct_t *vma_temp = task->mm->mmap;
    while(vma_temp->vm_next != NULL)
        vma_temp = vma_temp->vm_next;
    vma_temp->vm_next = vma;

    //vma->vm_next = task->mm->mmap;
    //task->mm->mmap = vma;

    task->mm->brk = HEAP_START;

    vma->vm_mm    = task->mm;
    vma->vm_start = HEAP_START;
    vma->vm_end   = HEAP_START + PAGE_SIZE;
    vma->vm_flags = (IS_RD + IS_WR);
    vma->file     = NULL;
    vma->vm_next  = NULL;

}

void allocate_stack(task_struct_t *task)
{
    vm_area_struct_t *vma = (vm_area_struct_t *) kmalloc(PAGE_SIZE);

    vm_area_struct_t *vma_temp = task->mm->mmap;
    while(vma_temp->vm_next != NULL)
        vma_temp = vma_temp->vm_next;
    vma_temp->vm_next = vma;

    //vma->vm_next = task->mm->mmap;
    //task->mm->mmap = vma;

    vma->vm_mm    = task->mm;
    //vma->vm_start = STACK_TOP + PAGE_SIZE;
    //vma->vm_end   = STACK_TOP;
    vma->vm_start = STACK_TOP;
    vma->vm_end   = STACK_TOP + PAGE_SIZE;
    vma->vm_flags = (IS_RD + IS_WR);
    vma->file     = NULL;
    vma->vm_next  = NULL;

    task->mm->brk  = STACK_TOP;
    task->stack_p = (uint64_t) (STACK_TOP + PAGE_SIZE - 16);
}

void print_elf_info(task_struct_t *task)
{
    mm_struct_t *mm_temp = task->mm;
    vm_area_struct_t *vma_temp = task->mm->mmap;

    kprintf("MM Struct: \n");
    kprintf("RIP: %p, PML4: %p, Start Code: %p, End Code: %p, Start Data: %p, End Data: %p\n",
             task->rip, mm_temp->pml4, mm_temp->start_code, mm_temp->end_code,
             mm_temp->start_data, mm_temp->end_data);

    while(vma_temp != NULL) {
        kprintf("VM Area Struct: \n");
        kprintf("VM_START: %p, VM_END: %p, VM_FLAGS: %p\n", vma_temp->vm_start, vma_temp->vm_end, vma_temp->vm_flags);
        vma_temp = vma_temp->vm_next;
    }
}

int load_elf(task_struct_t *task, const char *fname)
{

    struct posix_header_ustar * p_hdr_ustar = (struct posix_header_ustar *) get_bin_addr(fname);
    Elf64_Ehdr *e_hdr = (Elf64_Ehdr *) (p_hdr_ustar + 1);

    if(e_hdr == NULL)
        return -1;

    Elf64_Phdr *p_hdr = (Elf64_Phdr *) ((uint64_t)e_hdr + e_hdr->e_phoff);
    task->rip = e_hdr->e_entry; //TODO: should be assigned to stack[510], i dont' need rip then

    task->mm = (mm_struct_t *) kmalloc(PAGE_SIZE);

    for(int i = 0; i < e_hdr->e_phnum; i++)
    {
        if(p_hdr->p_type == 1) {
            vm_area_struct_t *vma = (vm_area_struct_t *) kmalloc(PAGE_SIZE);
            memset(vma, 0, sizeof(vm_area_struct_t));

            vma->vm_mm    = task->mm;
            vma->vm_start = p_hdr->p_vaddr;
            vma->vm_end   = p_hdr->p_vaddr + p_hdr->p_memsz;
            vma->vm_flags = p_hdr->p_flags;
            vma->file     = NULL;
            vma->vm_next  = NULL;

            if(task->mm->mmap == NULL)
                task->mm->mmap = vma;
            else {
                vm_area_struct_t *vma_temp = task->mm->mmap;
                while(vma_temp->vm_next != NULL)
                    vma_temp = vma_temp->vm_next;
                vma_temp->vm_next = vma;
                //vma->vm_next = task->mm->mmap;
                //task->mm->mmap = vma;
            }

            //TODO: Map process to virtual address
            /*
            uint64_t st = align_down(p_hdr->p_vaddr);
            uint64_t sz = (align_down(vma->vm_end) - align_down(vma->vm_start))/PAGE_SIZE;
            if(sz == 0) sz = 1;
            for(int i = 0; i < sz; i++) {
                uint64_t temp = (uint64_t) kmalloc(PAGE_SIZE);
                map_proc((temp - KERNAL_BASE_ADDRESS), st);
                st += PAGE_SIZE;
            }
            */

            if(vma->vm_flags == (IS_RD + IS_XE))
            {
                task->mm->start_code = vma->vm_start;
                task->mm->end_code   = vma->vm_end;

                vma->file = (file_t *) kmalloc(PAGE_SIZE);
                vma->file->f_start = (uint64_t) e_hdr;
                vma->file->f_pgoff = p_hdr->p_offset;
                vma->file->f_size  = p_hdr->p_filesz;
                //memcpy((void*) vma->vm_start, (void*) ((uint64_t) e_hdr + p_hdr->p_offset), p_hdr->p_filesz);
            }
            else if(vma->vm_flags == (IS_RD + IS_WR)) { //TODO: check this condition.
                task->mm->start_data = vma->vm_start;
                task->mm->end_data   = vma->vm_end;

                vma->file = (file_t *) kmalloc(PAGE_SIZE);
                vma->file->f_start = (uint64_t) e_hdr;
                vma->file->f_pgoff = p_hdr->p_offset;
                vma->file->f_size  = p_hdr->p_filesz;
                //memcpy((void*) vma->vm_start, (void*) ((uint64_t) e_hdr + p_hdr->p_offset), p_hdr->p_filesz);
            }
        }
        p_hdr++;
    }

    allocate_heap(task);
    allocate_stack(task);
    //print_elf_info(task);

    return 0;
}

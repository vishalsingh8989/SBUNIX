#include <sys/vmm.h>
#include <sys/defs.h>
#include <sys/kprintf.h>
#include <sys/utils.h>
#include <sys/asm_utils.h>
#include <sys/process.h>
#include <logger.h>

int num_pages;

page_stat_t* free_pages_list;
page_stat_t* pages_list;

static void * preinit_alloc (uint64_t physfree, int size)
{
    static char *nextfree;
    char *result;

    if(!nextfree) {
        nextfree = (char *) align_up(KERNAL_BASE_ADDRESS + physfree);
    }
    else {
        nextfree = (char *) align_up((uint64_t) nextfree);
    }

    result = nextfree;
    nextfree += size;

    return result;
}

void pages_init (void* physbase, void* physfree)
{
    for(int i = 0; i < num_pages; i++) {
        uint64_t paddr = i*PAGE_SIZE;
        if (paddr <= (uint64_t) physfree + 2*PAGE_SIZE) {
            pages_list[i].ref = 1;
            pages_list[i].next = NULL;
        }
        else {
            pages_list[i].ref = 0;
            pages_list[i].next = free_pages_list;
            free_pages_list = &pages_list[i];
        }
    }
}

static uint64_t get_pa(page_stat_t* page)
{
    uint64_t diff = (page - pages_list);
    uint64_t addr = diff * PAGE_SIZE;
    return addr;
}

static uint64_t get_va(page_stat_t* page)
{
    uint64_t diff = (page - pages_list);
    uint64_t addr = KERNAL_BASE_ADDRESS + (diff * PAGE_SIZE);
    return addr;
}

uint64_t page_alloc()
{
    page_stat_t * page = NULL;

    if (!free_pages_list) {
        kprintf("Out of memory in page_alloc()\n");
        return -1;
    }
    else {
        page = free_pages_list;
        free_pages_list = free_pages_list->next;
    }

    uint64_t addr = get_va(page);
    memset((void *) addr, 0, PAGE_SIZE);
    page->ref = 1;

    return get_pa(page);
}

uint64_t get_mapping(struct page_map_level_4* pmap_l4, uint64_t vaddr)
{
    struct page_table *ptable;
    struct page_directory *pdir;
    struct page_directory_pointer *pdir_p;
    uint64_t entry;

    entry = pmap_l4->pml4e[PML4_IDX(vaddr)];
    if(entry & _PAGE_PRESENT) {
        pdir_p = (struct page_directory_pointer *) (entry & 0xfffffffffffff000);
    }
    else {
        klog(INFO,"Entry not mapped!!!\n");
        return -1;
    }

    entry = pdir_p->pdpe[PDPT_IDX(vaddr)];
    if(entry & _PAGE_PRESENT) {
        pdir = (struct page_directory *) (entry & 0xfffffffffffff000);
    }
    else {
        klog(INFO,"Entry not mapped!!!\n");
        return -1;
    }

    entry = pdir->pde[PDE_IDX(vaddr)];
    if(entry & _PAGE_PRESENT) {
        ptable = (struct page_table *) (entry & 0xfffffffffffff000);
    }
    else {
        klog(ERR, "Entry not mapped!!!\n");
        return -1;
    }

    entry = ptable->pte[PT_IDX(vaddr)];
    if(entry & _PAGE_PRESENT) {
        return (uint64_t) (entry & 0xfffffffffffff000);
    }
    else {
        klog(ERR, "Entry not mapped!!!\n");
        return -1;
    }
}

void * pa_to_va(void * addr)
{
    return addr + KERNAL_BASE_ADDRESS;
}

void * va_to_pa(void * addr)
{
    return addr - KERNAL_BASE_ADDRESS;
}

void setup_child_ptables(uint64_t cpml4)
{

    struct page_map_level_4 * parent_pml4 = (struct page_map_level_4 *) read_cr3();
    struct page_map_level_4 * child_pml4  = (struct page_map_level_4 *) cpml4;

    parent_pml4 = (struct page_map_level_4 *) pa_to_va(parent_pml4);
    child_pml4  = (struct page_map_level_4 *) pa_to_va(child_pml4);

    volatile int pml4_idx = 0;
    for(;  pml4_idx < 511; pml4_idx++) {
        uint64_t p_pml4_entry = parent_pml4->pml4e[pml4_idx];
        uint64_t c_pml4_entry = 0;
        if(p_pml4_entry & _PAGE_PRESENT) {
            struct page_directory_pointer *c_pdir_p;
            struct page_directory_pointer *p_pdir_p;

            c_pdir_p      = (struct page_directory_pointer *) page_alloc();
            c_pml4_entry  = (uint64_t) c_pdir_p;
            c_pml4_entry |= (_PAGE_PRESENT | _PAGE_RW | _PAGE_USER);
            child_pml4->pml4e[pml4_idx] = c_pml4_entry;

            p_pdir_p = (struct page_directory_pointer *) (p_pml4_entry & 0xfffffffffffff000);
            p_pdir_p = (struct page_directory_pointer *) pa_to_va(p_pdir_p);
            c_pdir_p = (struct page_directory_pointer *) (c_pml4_entry & 0xfffffffffffff000);
            c_pdir_p = (struct page_directory_pointer *) pa_to_va(c_pdir_p);

            volatile int pdp_idx = 0;
            for(; pdp_idx < 512; pdp_idx++) {
                uint64_t p_pdp_entry = p_pdir_p->pdpe[pdp_idx];
                uint64_t c_pdp_entry = 0;
                if(p_pdp_entry & _PAGE_PRESENT) {
                    struct page_directory *c_pdir;
                    struct page_directory *p_pdir;

                    c_pdir       = (struct page_directory *) page_alloc();
                    c_pdp_entry  = (uint64_t) c_pdir;
                    c_pdp_entry |= (_PAGE_PRESENT | _PAGE_RW | _PAGE_USER);
                    c_pdir_p->pdpe[pdp_idx] = c_pdp_entry;

                    p_pdir = (struct page_directory *) (p_pdp_entry & 0xfffffffffffff000);
                    p_pdir = (struct page_directory *) pa_to_va(p_pdir);
                    c_pdir = (struct page_directory *) (c_pdp_entry & 0xfffffffffffff000);
                    c_pdir = (struct page_directory *) pa_to_va(c_pdir);

                    volatile int pd_idx = 0;
                    for(; pd_idx < 512; pd_idx++) {
                        uint64_t p_pd_entry = p_pdir->pde[pd_idx];
                        uint64_t c_pd_entry = 0;

                        if(p_pd_entry & _PAGE_PRESENT) {
                            struct page_table *p_pt;
                            struct page_table *c_pt;

                            c_pt        = (struct page_table  *) page_alloc();
                            c_pd_entry  = (uint64_t) c_pt;
                            c_pd_entry |= (_PAGE_PRESENT | _PAGE_RW | _PAGE_USER);
                            c_pdir->pde[pd_idx] = c_pd_entry;

                            p_pt = (struct page_table *) (p_pd_entry & 0xfffffffffffff000);
                            p_pt = (struct page_table *) pa_to_va(p_pt);
                            c_pt = (struct page_table *) (c_pd_entry & 0xfffffffffffff000);
                            c_pt = (struct page_table *) pa_to_va(c_pt);

                            volatile int pt_idx = 0;
                            for(; pt_idx < 512; pt_idx++) {
                                uint64_t p_pt_entry = p_pt->pte[pt_idx];
                                if(p_pt_entry & _PAGE_PRESENT) {
                                    uint64_t pte;
                                    pte  = p_pt_entry & 0xfffffffffffff000;
                                    pte |= (_PAGE_PRESENT | _PAGE_USER);
                                    c_pt->pte[pt_idx] = pte;
                                    p_pt->pte[pt_idx] = pte;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    //child_pml4->pml4e[510] = parent_pml4->pml4e[510];
    child_pml4->pml4e[511] = parent_pml4->pml4e[511];
}

void delete_ptables(uint64_t addr)
{
    struct page_map_level_4 * pml4  = (struct page_map_level_4 *) addr;
    pml4  = (struct page_map_level_4 *) pa_to_va(pml4);

    volatile int pml4_idx = 0;
    for(;  pml4_idx < 511; pml4_idx++) {
        uint64_t pml4_entry = pml4->pml4e[pml4_idx];
        if(pml4_entry & _PAGE_PRESENT) {
            struct page_directory_pointer *pdir_p;

            pdir_p = (struct page_directory_pointer *) (pml4_entry & 0xfffffffffffff000);
            pdir_p = (struct page_directory_pointer *) pa_to_va(pdir_p);

            volatile int pdp_idx = 0;
            for(; pdp_idx < 512; pdp_idx++) {
                uint64_t pdp_entry = pdir_p->pdpe[pdp_idx];
                if(pdp_entry & _PAGE_PRESENT) {
                    struct page_directory *pdir;

                    pdir = (struct page_directory *) (pdp_entry & 0xfffffffffffff000);
                    pdir = (struct page_directory *) pa_to_va(pdir);

                    volatile int pd_idx = 0;
                    for(; pd_idx < 512; pd_idx++) {
                        uint64_t pd_entry = pdir->pde[pd_idx];
                        if(pd_entry & _PAGE_PRESENT) {
                            struct page_table *pt;

                            pt = (struct page_table *) (pd_entry & 0xfffffffffffff000);
                            pt = (struct page_table *) pa_to_va(pt);

                            volatile int pt_idx = 0;
                            for(; pt_idx < 512; pt_idx++) {
                                uint64_t pt_entry = pt->pte[pt_idx];
                                if(pt_entry & _PAGE_PRESENT) {
                                    uint64_t pte;
                                    pte  = pt_entry & 0xfffffffffffff000;
                                    pte  = (uint64_t) pa_to_va((uint64_t *) pte);
                                    kfree((uint64_t *) pte);
                                }
                            }
                            kfree((uint64_t *) pt);
                        }
                    }
                    kfree((uint64_t *) pdir);
                }
            }
            kfree((uint64_t *) pdir_p);
        }
    }
    kfree((uint64_t *) addr);
}

void map_proc(uint64_t paddr, uint64_t vaddr)
{
    struct page_table *ptable;
    struct page_directory *pdir;
    struct page_directory_pointer *pdir_p;
    uint64_t entry;

    struct page_map_level_4 * pmap_l4 = (struct page_map_level_4 *) read_cr3();
    pmap_l4 = (struct page_map_level_4 *) (KERNAL_BASE_ADDRESS | (uint64_t) pmap_l4);

    entry = pmap_l4->pml4e[PML4_IDX(vaddr)];
    if(entry & _PAGE_PRESENT) {
        pdir_p = (struct page_directory_pointer *) (entry & 0xfffffffffffff000);
        pdir_p = (struct page_directory_pointer *) pa_to_va(pdir_p);
    }
    else {
        pdir_p = (struct page_directory_pointer *) page_alloc();
        entry = (uint64_t) pdir_p;
        entry |= (_PAGE_PRESENT | _PAGE_RW | _PAGE_USER);
        pmap_l4->pml4e[PML4_IDX(vaddr)] = entry;
        pdir_p = (struct page_directory_pointer *) pa_to_va(pdir_p);
    }

    entry = pdir_p->pdpe[PDPT_IDX(vaddr)];
    if(entry & _PAGE_PRESENT) {
        pdir = (struct page_directory *) (entry & 0xfffffffffffff000);
        pdir = (struct page_directory *) pa_to_va(pdir);
    }
    else {
        pdir = (struct page_directory *) page_alloc();
        entry = (uint64_t) pdir;
        entry |= (_PAGE_PRESENT | _PAGE_RW | _PAGE_USER);
        pdir_p->pdpe[PDPT_IDX(vaddr)] = entry;
        pdir = (struct page_directory *) pa_to_va(pdir);
    }

    entry = pdir->pde[PDE_IDX(vaddr)];
    if(entry & _PAGE_PRESENT) {
        ptable = (struct page_table *) (entry & 0xfffffffffffff000);
        ptable = (struct page_table *) pa_to_va(ptable);
    }
    else {
        ptable = (struct page_table *) page_alloc();
        entry = (uint64_t) ptable;
        entry |= (_PAGE_PRESENT | _PAGE_RW | _PAGE_USER);
        pdir->pde[PDE_IDX(vaddr)] = entry;
        ptable = (struct page_table *) pa_to_va(ptable);
    }

    entry = ptable->pte[PT_IDX(vaddr)];
    if(entry & _PAGE_PRESENT) {
        klog(INFO, "Page %p -> %p already Mapped, chainging to %p\n", vaddr, entry, entry | 0x7);
        entry |= (_PAGE_PRESENT | _PAGE_RW | _PAGE_USER);
        ptable->pte[PT_IDX(vaddr)] = entry;
    }
    else {
        entry = paddr;
        entry |= (_PAGE_PRESENT | _PAGE_RW | _PAGE_USER);
        ptable->pte[PT_IDX(vaddr)] = entry;
    }

    write_cr3((uint64_t) pmap_l4 - KERNAL_BASE_ADDRESS);
}

void map_addr(struct page_map_level_4* pmap_l4, uint64_t paddr, uint64_t vaddr)
{
    struct page_table *ptable;
    struct page_directory *pdir;
    struct page_directory_pointer *pdir_p;
    uint64_t entry;

    entry = pmap_l4->pml4e[PML4_IDX(vaddr)];
    if(entry & _PAGE_PRESENT) {
        pdir_p = (struct page_directory_pointer *) (entry & 0xfffffffffffff000);
    }
    else {
        pdir_p = (struct page_directory_pointer *) page_alloc();
        entry = (uint64_t) pdir_p;
        entry |= (_PAGE_PRESENT | _PAGE_RW | _PAGE_USER);
        pmap_l4->pml4e[PML4_IDX(vaddr)] = entry;
    }

    entry = pdir_p->pdpe[PDPT_IDX(vaddr)];
    if(entry & _PAGE_PRESENT) {
        pdir = (struct page_directory *) (entry & 0xfffffffffffff000);
    }
    else {
        pdir = (struct page_directory *) page_alloc();
        entry = (uint64_t) pdir;
        entry |= (_PAGE_PRESENT | _PAGE_RW | _PAGE_USER);
        pdir_p->pdpe[PDPT_IDX(vaddr)] = entry;
    }

    entry = pdir->pde[PDE_IDX(vaddr)];
    if(entry & _PAGE_PRESENT) {
        ptable = (struct page_table *) (entry & 0xfffffffffffff000);
    }
    else {
        ptable = (struct page_table *) page_alloc();
        entry = (uint64_t) ptable;
        entry |= (_PAGE_PRESENT | _PAGE_RW | _PAGE_USER);
        pdir->pde[PDE_IDX(vaddr)] = entry;
    }

    entry = ptable->pte[PT_IDX(vaddr)];
    if(entry & _PAGE_PRESENT) {
        klog(ERR, "Page already Mapped!!!\n");
    }
    else {
        entry = paddr;
        entry |= (_PAGE_PRESENT | _PAGE_RW | _PAGE_USER);
        ptable->pte[PT_IDX(vaddr)] = entry;
    }

}

void map_addr_range(struct page_map_level_4* pmap_l4, uint64_t paddr, uint64_t vaddr, uint64_t size)
{
    uint64_t addrp, addrv;
    addrp = align_up(paddr);
    addrv = align_up(vaddr);
    for(int i = 0; i < size; i++) {
        map_addr(pmap_l4, (uint64_t) addrp, addrv);
        addrp += PAGE_SIZE;
        addrv += PAGE_SIZE;
    }
}

void vmm_init(uint32_t* modulep, void* physbase, void* physfree)
{
    klog(INFO, "physfree %p, physbase %p\n", (uint64_t)physfree, (uint64_t)physbase);

    struct smap_t {
        uint64_t base, length;
        uint32_t type;
    }__attribute__((packed)) *smap;

    while(modulep[0] != 0x9001) modulep += modulep[1]+2;

    num_pages = 0;
    for(smap = (struct smap_t*)(modulep+2); smap < (struct smap_t*)((char*)modulep+modulep[1]+2*4); ++smap) {
        if (smap->type == 1 && (smap->length/PAGE_SIZE) != 0) {
            klog(INFO, "Available Physical Memory [%p-%p]\n", smap->base, smap->base + smap->length);
            if (smap->base > (uint64_t) physfree) {
                num_pages += (smap->length < PAGE_SIZE) ? 1 : smap->length/PAGE_SIZE;
            }
            else if (smap->base + smap->length > (uint64_t) physfree) {
                num_pages += (smap->length < PAGE_SIZE) ? 1 : (smap->base + smap->length - (uint64_t) physfree)/PAGE_SIZE;
            }
            else {
                num_pages += (smap->length < PAGE_SIZE) ? 1 : (smap->length)/PAGE_SIZE;
            }
        }
    }
    total_pages = num_pages;

    pages_list = preinit_alloc((uint64_t) physfree, num_pages * sizeof(page_stat_t));

    pages_init(physbase, physfree);

    struct page_map_level_4* pmap_l4;
    pmap_l4 = (struct page_map_level_4 *) page_alloc();
    //pmap_l4 = (struct page_map_level_4 *) preinit_alloc((uint64_t) physfree, PAGE_SIZE);
    memset((void *) pmap_l4, 0, PAGE_SIZE);

    uint64_t phy_addr;
    uint64_t vir_addr;
    int size;

    //Identity Mapping kernal.
    phy_addr = (uint64_t) physbase;
    vir_addr = phy_addr + KERNAL_BASE_ADDRESS;
    size = (physfree - physbase)/PAGE_SIZE;
    klog(INFO, "phy_addr: %p, vir_addr: %p, size: %d\n", phy_addr, vir_addr, size);
    map_addr_range(pmap_l4, phy_addr, vir_addr, size);

    for(smap = (struct smap_t*)(modulep+2); smap < (struct smap_t*)((char*)modulep+modulep[1]+2*4); ++smap) {
        if (smap->type == 1 && smap->length != 0) { //TODO: fix this
            if (smap->base > (uint64_t) physfree) {
                phy_addr = smap->base;
                vir_addr = phy_addr + KERNAL_BASE_ADDRESS;
                size = (smap->length < PAGE_SIZE) ? 1 : smap->length/PAGE_SIZE;
                klog(INFO, "phy_addr: %p, vir_addr: %p, size: %d\n", phy_addr, vir_addr, size);
                map_addr_range(pmap_l4, phy_addr, vir_addr, size);
            }
            else if (smap->base + smap->length > (uint64_t) physfree) {
                phy_addr = (uint64_t) physfree;
                vir_addr = phy_addr + KERNAL_BASE_ADDRESS;
                size = (smap->length < PAGE_SIZE) ? 1 : (smap->base + smap->length - (uint64_t) physfree)/PAGE_SIZE;
                klog(INFO, "phy_addr: %p, vir_addr: %p, size: %d\n", phy_addr, vir_addr, size);
                map_addr_range(pmap_l4, phy_addr, vir_addr, size);
            }
            else {
                phy_addr = smap->base;
                vir_addr = phy_addr + KERNAL_BASE_ADDRESS;
                size = (smap->length < PAGE_SIZE) ? 1 : (smap->length)/PAGE_SIZE;
                klog(INFO, "phy_addr: %p, vir_addr: %p, size: %d\n", phy_addr, vir_addr, size);
                map_addr_range(pmap_l4, phy_addr, vir_addr, size);
            }
        }
    }

    map_addr_range(pmap_l4, 0xb8000, 0xffffffff800b8000, 1);
    klog(IMP, "PML4(PHY): %p, PML4(VIR): %p, 511: %p\n", pmap_l4, (uint64_t) pmap_l4 + KERNAL_BASE_ADDRESS, pmap_l4->pml4e[511]);

    //uint64_t temp = get_mapping(pmap_l4, 0xffffffff800b8000);
    //kprintf("Temp: %p\n", temp);

    //uint64_t pml4_addr = (uint64_t) pmap_l4;
    //map_addr_range(pmap_l4, pml4_addr, pml4_addr, 1);
    //pmap_l4->pml4e[PML4_IDX(pml4_addr)] = pml4_addr | (_PAGE_PRESENT | _PAGE_RW | _PAGE_USER);

    //uint64_t self_idx = (PML4_IDX(KERNAL_BASE_ADDRESS) - 1) & 0x1ff;
    //pmap_l4->pml4e[self_idx] = pml4_addr | (_PAGE_PRESENT | _PAGE_RW | _PAGE_USER);
    //pmap_l4->pml4e[510] = pml4_addr | _PAGE_PRESENT | _PAGE_RW;

    /*for(uint64_t addr = (uint64_t) physbase + KERNAL_BASE_ADDRESS; addr <= (uint64_t) physfree + KERNAL_BASE_ADDRESS; addr += 0x1000) {
        uint64_t temp = get_mapping(pmap_l4, addr);
        kprintf("V-ADDR: %p mapped to P-ADDR: %p\n", addr, temp);
    }*/
    //kprintf("physfree %p, physbase %p\n", (uint64_t)physfree, (uint64_t)physbase);

    volatile uint64_t cr0;
    volatile uint64_t cr4;
    volatile uint64_t ia32_efer;

    __asm__ __volatile__("movq %%cr0, %0;":"=r"(cr0));
    __asm__ __volatile__("movq %%cr4, %0;":"=r"(cr4));
    cr4 = cr4 | (1 << 7);
    cr4 = cr4 & 0xffffffffffffffef;
    __asm__ __volatile__("movq %0, %%cr4;" :: "r"(cr4));
    __asm__ __volatile__("movq %%cr4, %0;":"=r"(cr4));
    ia32_efer = rdmsr(MSR_EFER);

    klog(IMP, "CR0: %p, CR4: %p, ia32_efer: %p\n", cr0, cr4, ia32_efer);

    //load_cr3((uint64_t) pmap_l4 - KERNAL_BASE_ADDRESS);
    write_cr3((uint64_t) pmap_l4);

    //temp = get_mapping(pmap_l4, 0xffffffff800b8000);
    //kprintf("Temp: %p\n", temp);

    klog(INFO, "Page Table Setup Sucessfull\n");
}

uint64_t * kmalloc(uint64_t size)
{
    if(!free_pages_list || size==0) {
        return NULL;
    }

    uint64_t num_pages = align_up(size)/PAGE_SIZE;

    uint64_t * addr = (uint64_t *) get_va(free_pages_list);

    //TODO: can I do this ?
    for(int i = 0; i < num_pages; i++) {
        if(page_alloc() == -1)
            return NULL;
        else
            memset((void *) addr, 0, PAGE_SIZE);
    }

    pages_used += num_pages;
    //kprintf("Pages Used: %d\n", pages_used);

    return addr;
}

void kfree(uint64_t * vaddr)
{
    page_stat_t* page = pages_list + ((uint64_t) vaddr - KERNAL_BASE_ADDRESS) / PAGE_SIZE;
    page->ref = 0;
    page->next = free_pages_list;
    free_pages_list = page;

    pages_used--;
}

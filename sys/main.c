#include <sys/defs.h>
#include <sys/gdt.h>
#include <sys/idt.h>
#include <sys/pic.h>
#include <sys/pci.h>
#include <sys/kprintf.h>
#include <sys/tarfs.h>
#include <sys/ahci.h>

extern uint64_t *abar;
//extern uint64_t *sata_port;
uint64_t disk_buf[1024];

#define INITIAL_STACK_SIZE 4096
uint8_t initial_stack[INITIAL_STACK_SIZE]__attribute__((aligned(16)));
uint32_t* loader_stack;
extern char kernmem, physbase;

void start(uint32_t *modulep, void *physbase, void *physfree)
{
  struct smap_t {
    uint64_t base, length;
    uint32_t type;
  }__attribute__((packed)) *smap;
  while(modulep[0] != 0x9001) modulep += modulep[1]+2;
  for(smap = (struct smap_t*)(modulep+2); smap < (struct smap_t*)((char*)modulep+modulep[1]+2*4); ++smap) {
    if (smap->type == 1 /* memory */ && smap->length != 0) {
      kprintf("Available Physical Memory [%p-%p]\n", smap->base, smap->base + smap->length);
    }
  }
  kprintf("physfree %p\n", (uint64_t)physfree);
  kprintf("tarfs in [%p:%p]\n", &_binary_tarfs_start, &_binary_tarfs_end);

  //__asm__("int $0");
  //__asm__("int $32");
  //__asm__("int $40");

  __asm__("sti");

  hba_mem_t * abar_t = (hba_mem_t *) abar;
  probe_port(abar_t);
  for(int i = 0; i < 1024; i++)
      disk_buf[i] = 0x0101010101010101;
  disk_rw(&abar_t->ports[0], 0, 1, 1, disk_buf, 1);
  for(int i = 0; i < 1024; i++)
      disk_buf[i] = 0x0;
  disk_rw(&abar_t->ports[0], 0, 1, 1, disk_buf, 0);
  for(int i = 0; i < 10; i++)
      kprintf("disk_buf: %x\n", disk_buf[i]);

  while(1);
}

void boot(void)
{
  // note: function changes rsp, local stack variables can't be practically used
  register char *temp2;

  for(temp2 = (char*)0xb8001; temp2 < (char*)0xb8000+160*25; temp2 += 2) *temp2 = 7 /* white */;

  __asm__ volatile (
    "cli;"
    "movq %%rsp, %0;"
    "movq %1, %%rsp;"
    :"=g"(loader_stack)
    :"r"(&initial_stack[INITIAL_STACK_SIZE])
  );

  init_gdt();
  init_idt();
  init_pic();
  init_pci();

  start(
    (uint32_t*)((char*)(uint64_t)loader_stack[3] + (uint64_t)&kernmem - (uint64_t)&physbase),
    (uint64_t*)&physbase,
    (uint64_t*)(uint64_t)loader_stack[4]
  );

  kprintf("!!!!! start() returned !!!!!\n");
  while(1) __asm__ volatile ("hlt");
}

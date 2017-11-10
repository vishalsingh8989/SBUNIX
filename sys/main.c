#include <sys/defs.h>
#include <sys/gdt.h>
#include <sys/idt.h>
#include <sys/pic.h>
#include <sys/pci.h>
#include <sys/vmm.h>
#include <sys/kprintf.h>
#include <sys/tarfs.h>
#include <sys/ahci.h>

extern uint64_t *abar;

#define INITIAL_STACK_SIZE 4096
uint8_t initial_stack[INITIAL_STACK_SIZE]__attribute__((aligned(16)));
uint32_t* loader_stack;
extern char kernmem, physbase;

void start(uint32_t *modulep, void *physbase, void *physfree)
{
  //struct smap_t {
  //  uint64_t base, length;
  //  uint32_t type;
  //}__attribute__((packed)) *smap;
  //while(modulep[0] != 0x9001) modulep += modulep[1]+2;
  //for(smap = (struct smap_t*)(modulep+2); smap < (struct smap_t*)((char*)modulep+modulep[1]+2*4); ++smap) {
  //  if (smap->type == 1 /* memory */ && smap->length != 0) {
  //    kprintf("Available Physical Memory [%p-%p]\n", smap->base, smap->base + smap->length);
  //  }
  //}
  //kprintf("physfree %p\n", (uint64_t)physfree);

  kprintf("tarfs in [%p:%p]\n", &_binary_tarfs_start, &_binary_tarfs_end);

  hba_mem_t * abar_t = (hba_mem_t *) abar;
  probe_port(abar_t);

  vmm_init(modulep, physbase, physfree);
  init_tarfs();
  kprintf("tarfs in [%p:%p]\n", &_binary_tarfs_start, &_binary_tarfs_end);

  //__asm__("int $0");

  /*
  uint8_t * dwr_buf = (uint8_t *) 0x100000;
  uint8_t * drd_buf = (uint8_t *) 0x900000;

  for(int j = 0; j < 100; j++) {

    for(int i = 0; i < 4096; i++)
        dwr_buf[i] = j;

    kprintf("Writing Sector: %d ", j);
    kprintf("dwr_buf[0]: %d ", dwr_buf[0]);
    kprintf("dwr_buf[1023]: %d ", dwr_buf[1023]);
    kprintf("dwr_buf[4095]: %d\n", dwr_buf[4095]);

    disk_rw(&abar_t->ports[0], j*8, 0, 8, dwr_buf, 1);
    disk_rw(&abar_t->ports[0], j*8, 0, 8, drd_buf, 0);

    kprintf("Reading Sector: %d, ", j);
    kprintf("drd_buf[0]: %d ", drd_buf[0]);
    kprintf("drd_buf[1023]: %d ", drd_buf[1023]);
    kprintf("drd_buf[4095]: %d\n", drd_buf[4095]);
    kprintf("------Finished Test for Sector %d--------\n", j);
  }
  */

  __asm__("sti");
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

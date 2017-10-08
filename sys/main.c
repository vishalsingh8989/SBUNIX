#include <sys/defs.h>
#include <sys/gdt.h>
#include <sys/idt.h>
#include <sys/pic.h>
#include <sys/pci.h>
#include <sys/kprintf.h>
#include <sys/tarfs.h>
#include <sys/ahci.h>

//#define INITIAL_STACK_SIZE 4096  //Original value
#define INITIAL_STACK_SIZE 4096
#define PAGESIZE 4096
uint8_t initial_stack[INITIAL_STACK_SIZE]__attribute__((aligned(16)));
uint32_t* loader_stack;
extern char kernmem, physbase;

extern uint64_t *abar;
uint64_t num_pages;


void start(uint32_t *modulep, void *physbase, void *physfree)
{
	num_pages = 0;
  struct smap_t {
    uint64_t base, length;
    uint32_t type;
  }__attribute__((packed)) *smap;
  while(modulep[0] != 0x9001) modulep += modulep[1]+2;
  for(smap = (struct smap_t*)(modulep+2); smap < (struct smap_t*)((char*)modulep+modulep[1]+2*4); ++smap) {
    if (smap->type == 1 /* memory */ && smap->length != 0) {
    		dsleep();
    		if(smap->base !=0){
    			num_pages = num_pages + (smap->base+smap->length - (uint64_t)physfree)/PAGESIZE;
    		}
        kprintf("Available Physical Memory [%x-%x] , length : %p\n", smap->base, smap->base + smap->length, smap->length);
    }
  }

  kprintf("kernmem %p\n", (uint64_t)&kernmem);
  kprintf("physbase %p\n", (uint64_t)physbase);
  kprintf("physfree %p\n", (uint64_t)physfree);
  kprintf("tarfs in [%p:%p]\n", &_binary_tarfs_start, &_binary_tarfs_end);
  kprintf("Num of pages :  %d\n", num_pages);




	  __asm__("sti");


    hba_mem_t * abar_t = (hba_mem_t *) abar;
    for(int i = 0 ;i <  10 ; i++){
    			dsleep();
    }
    probe_port(abar_t);

    // working only for drd_buf[0]
    uint8_t * dwr_buf = (uint8_t *) 0x1100000;
    uint8_t * drd_buf = (uint8_t *) 0x1700000;



    //uint8_t * dwr_buf = (uint8_t *) 0x100000000;
    //uint8_t * drd_buf = (uint8_t *) 0x101000000;
    for(int j = 0; j < 3; j++) {

      for(int i = 0; i < 4096; i++)
          dwr_buf[i] = j;

      kprintf("Writing Sector: %d ", j);
      kprintf("dwr_buf[0]: %d, ", dwr_buf[0]);
      kprintf("dwr_buf[1023]: %d, ", dwr_buf[1023]);
      kprintf("dwr_buf[4095]: %d\n", dwr_buf[4095]);

      disk_rw(&abar_t->ports[1], j*8, 0, 8, dwr_buf, 1);
      disk_rw(&abar_t->ports[1], j*8, 0, 8, drd_buf, 0);


      kprintf("Reading Sector: %d ", j);
      for(int k = 0 ; k < 4096 ;  k = k + 400){
    	  	  kprintf("d-",k, drd_buf[k]);

      }

      for(int l = 0 ;l <  2 ; l++){
          	  	    			dsleep();
        }
      //kprintf("drd_buf[1k]: %d ,", drd_buf[1023]);
      //kprintf("drd_buf[4k]: %d\n", drd_buf[4095]);
    }



// not a wise idea - vishal just for cls testing

  while(1);
}

void boot(void)
{
  // note: function changes rsp, local stack variables can't be practically used
  register char *temp2;
  cls();
  for(temp2 = (char*)0xb8001; temp2 < (char*)0xb8000+160*25; temp2 += 2) *temp2 = 7 /* white */;

  __asm__(
    "cli;"
    "movq %%rsp, %0;"
    "movq %1, %%rsp;"
    :"=g"(loader_stack)
    :"r"(&initial_stack[INITIAL_STACK_SIZE])
  );

  init_gdt();
  init_idt();
  init_pic();
  scan_pci();

  start(
    (uint32_t*)((char*)(uint64_t)loader_stack[3] + (uint64_t)&kernmem - (uint64_t)&physbase),
    (uint64_t*)&physbase,
    (uint64_t*)(uint64_t)loader_stack[4]
  );

  kprintf("!!!!! start() returned !!!!!\n");
  while(1);
}

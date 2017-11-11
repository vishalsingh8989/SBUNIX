#include <sys/kprintf.h>
#include <sys/keyb.h>
#include <sys/defs.h>
#include <sys/pic.h>
#include <sys/utils.h>
#include <sys/idt.h>
#include <sys/vmm.h>
#include <sys/process.h>
#include <sys/asm_utils.h>
#include <sys/syscall.h>

extern void _isr128(void);

#define OFFS 0xffffffff00000000

//void syscall_handler(uint64_t s_num, uint64_t arg1, uint64_t arg2, uint64_t arg3)
uint64_t syscall_handler(cpu_regs* regs)
{
    uint64_t s_num = regs->rax;
    uint64_t arg1  = regs->rdi;
    uint64_t arg2  = regs->rsi;
    uint64_t arg3  = regs->rdx;
   
    kprintf("In the syscall handler, syscall no: %d\n", s_num);
    kprintf("arg1: %x, arg2: %x, arg3: %x\n", arg1, arg2, arg3);

    switch(s_num) {

        case __NR_exit:
            kprintf("Executing exit syscall\n");
            sys_exit();
            return 0;

        default:
            return -1;
    }
}

void init_syscall()
{
    uint64_t efer;

    efer = rdmsr(EFER);
    wrmsr(EFER, efer | EFER_SCE);

    wrmsr(LSTAR, (uint64_t)_isr128);
}

void timer_int_handler() {
   static int i = 0, s = 0, m = 0, h = 0;

   i++;
   if(i == 18) {
       s++;
       i = 0;
   }
   if(s == 60) {
       m++;
       s = 0;
   }
   if(m == 60) {
       h++;
       m = 0;
   }
   if(h == 24) {
       h = 0;
   }

   char sl = (char) (s%10+48);
   char sh = (char) (s/10+48);
   char ml = (char) (m%10+48);
   char mh = (char) (m/10+48);
   char hl = (char) (h%10+48);
   char hh = (char) (h/10+48);

   pchar_xy(sl , GREEN, 79, 24);
   pchar_xy(sh , GREEN, 78, 24);
   pchar_xy(':', GREEN, 77, 24);
   pchar_xy(ml , GREEN, 76, 24);
   pchar_xy(mh , GREEN, 75, 24);
   pchar_xy(':', GREEN, 74, 24);
   pchar_xy(hl , GREEN, 73, 24);
   pchar_xy(hh , GREEN, 72, 24);

   pic_send_eoi(0);
}

void div0_int_handler(cpu_regs *regs) {
   kpanic("-- Div0 Interrupt Fired --\n");
}

void keyboard_int_handler() {
    char c;

    c = getchar();
    if (c != 0) {
        if (c == 17){
          pchar_xy('[', RED, 68, 24);
          pchar_xy('^', RED, 69, 24);
          pchar_xy('C', RED, 70, 24);
          pchar_xy(']', RED, 71, 24);
        }
        else {
          pchar_xy('[', RED, 68, 24);
          pchar_xy(' ', RED, 69, 24);
          pchar_xy(c  , RED, 70, 24);
          pchar_xy(']', RED, 71, 24);
        }
    }

    pic_send_eoi(1);
}

void debug_excep_handler(cpu_regs *regs) {
    kpanic("-- Debug Exception Fired --");
}

void overflow_handler() {
    kpanic("-- Overflow Exception Fired --");
}

void invalid_opcode_handler() {
    //while(1);
    kpanic("-- Invalid Opcode Exception Fired --");
}

void double_fault_handler() {
    kpanic("-- Double Fault Exception Fired --");
}

void stack_fault_handler() {
    kpanic("-- Stack Fault Exception Fired --");
}

void alignment_check_handler() {
    kpanic("-- Alignment Check Exception Fired --");
}

void page_fault_handler(cpu_regs *regs) {
    kprintf("-- Page Fault Execption Fired --\n");

    uint64_t error = regs->error & 0xf;
    kprintf("Int Id: %d, Error: %d\n", regs->int_id, error);

    uint64_t fault_addr = read_cr2();
    kprintf("Faulting address: %p\n", fault_addr);

    uint64_t p_write_err = error & PF_W;
    uint64_t p_prot_err  = error & PF_P;
    //uint64_t p_user_err  = error & PF_U;
    //uint64_t p_rsvd_err  = error & PF_R;
    //uint64_t p_insf_err  = error & PF_I;

    /*
    if(p_prot_err & !p_write_err) {
        kprintf("Page Fault at addr: %p\n", fault_addr);
        kpanic("Read permission error");
    }

    if(p_rsvd_err) {
        kprintf("Page Fault at addr: %p\n", fault_addr);
        kpanic("Reserved page error");
    }
    */

    vm_area_struct_t *vma = curr_task->mm->mmap;

    while(vma) {
        if(fault_addr >= vma->vm_start && fault_addr <= vma->vm_end) break;
        vma = vma->vm_next;
    }

    if(vma == NULL) {
        //TODO: Grow stack
        //TODO: Grow heap
        //TODO: Stack Overflow!
        //TODO: Segmention Fault!
        //kprintf("TODO: Handle growing stack, growing heap, stack overflow, SEGV\n");
        //TODO: this should be for seq fault only
        kpanic("TODO: Handle growing stack, growing heap, stack overflow, SEGV");
    }

    if(p_prot_err && p_write_err) {
        //TODO: Handle COW
        kprintf("TODO: Handle COW\n");
    }

    uint64_t page_addr = (uint64_t) kmalloc(PAGE_SIZE);
    page_addr = page_addr - KERNAL_BASE_ADDRESS; //TODO: wirte va_to_pa();
    uint64_t falign_addr = align_down(fault_addr);
    map_proc(page_addr, falign_addr);

    /*
    if(curr_task->stack_p >= vma->vm_start && curr_task->stack_p <= vma->vm_end) {
        //TODO: this was stack allocation and mapping. Don't need to copy.
        return;
    }
    */

    if(vma->file == NULL)
        return;

    //Copy contents.
    uint64_t src, dst;
    int size;

    if(falign_addr <= vma->vm_start){
        src = vma->file->f_start + vma->file->f_pgoff;
        dst = vma->vm_start;
    }
    else {
        src = vma->file->f_start + vma->file->f_pgoff + (falign_addr - vma->vm_start);
        dst = falign_addr;
    }

    if(vma->file->f_size <= PAGE_SIZE) {
        size = vma->file->f_size;
    }
    else if((falign_addr+PAGE_SIZE) <= (vma->vm_end - vma->file->f_bss_size)) {
        size = PAGE_SIZE;
    }
    else {
        size = (falign_addr+PAGE_SIZE) - (vma->vm_end - vma->file->f_bss_size);
    }

    memcpy((void *) dst, (void*) src, size);
}


void default_int_handler() {
    //while(1);
    kpanic("-- Unknown Interrupt Fired --");
}

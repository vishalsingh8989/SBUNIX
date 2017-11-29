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
#include <sys/terminal.h>
#include <dirent.h>
#include <logger.h>


extern void _isr128(void);
uint64_t k_rsp;
uint64_t u_rsp;

#define OFFS 0xffffffff00000000

uint64_t syscall_handler(cpu_regs* regs)
{
    uint64_t s_num = regs->rax;
    uint64_t arg1  = regs->rdi;
    uint64_t arg2  = regs->rsi;
    uint64_t arg3  = regs->rdx;
    //uint64_t arg4  = regs->r10;
    uint64_t arg5  = regs->r8;
    uint64_t arg6  = regs->r9;
    uint64_t ret;

    //klog(INFO,"In the syscall handler, syscall no: %d\n", s_num);
    //klog(INFO,"arg1: %x, arg2: %x, arg3: %x\n", arg1, arg2, arg3);

    //TODO: change char* to const char * and cast to uint64_t is not needed.
    switch(s_num) {

        case __NR_exit:
            klog(INFO,"Executing Exit Syscall\n");
            sys_exit();
            return 0;

        case __NR_read:
            klog(INFO,"Executing Read Syscall\n");
            ret = sys_read((uint64_t) arg1, (uint64_t) arg2, (uint64_t) arg3);
            return ret;

        case __NR_write:
            //klog(INFO,"Executing Write Syscall\n");
            ret = sys_write((uint64_t) arg1, (uint64_t) arg2, (uint64_t) arg3);
            return ret;

        case __NR_fork:
        		klog(INFO,"Executing Fork Syscall\n");
            ret = sys_fork();
            return ret;

        case __NR_execve:
            klog(INFO,"Executing Execve Syscall\n");
            ret = sys_execve((char *) arg1, (char **) arg2, (char **) arg3);
            return ret;

        case __NR_wait4:
        		klog(INFO,"Executing Wait4 Syscall\n");
            ret = sys_waitpid((uint64_t) arg1, (uint64_t) arg2, (uint64_t) arg3);
            return ret;

        case __NR_open:
        		klog(INFO,"Executing Open Syscall\n");
            ret = sys_open((char *) arg1, (uint64_t) arg2);
            return ret;

        case __NR_close:
        		klog(INFO,"Executing Close Syscall\n");
            ret = sys_close((uint64_t) arg1);
            return ret;

        case __NR_access:
            klog(INFO,"Executing access Syscall\n");
            ret = sys_access((char *) arg1, (uint64_t) arg2);
            return ret;

        case __NR_pipe:
            klog(INFO,"Executing pipe Syscall\n");
            ret = sys_pipe((uint64_t*) arg1);
            return ret;

        case __NR_dup2:
            klog(INFO,"Executing dup2 Syscall\n");
            ret = sys_dup2((uint64_t) arg1, (uint64_t) arg2);
            return ret;

        case __NR_getdents:
        		klog(INFO,"Executing getdents Syscall\n");
            ret = sys_getdents((uint64_t) arg1, (struct dirent *) arg2, (uint64_t) arg3);
            return ret;

        case __NR_getcwd:
        		klog(INFO,"Executing getcwd Syscall\n");
            ret = sys_getcwd((char *) arg1, (uint64_t) arg2);
            return ret;

        case __NR_chdir:
            klog(INFO,"Executing chdir Syscall\n");
            ret = sys_chdir((char *) arg1);
            return ret;

        case __NR_sched_yield:
            klog(INFO,"Executing yield Syscall\n");
            sys_sched_yield();
            return 0;

        case __NR_shutdown:
            klog(INFO,"Executing shutdown Syscall\n");
            sys_shutdown((uint64_t) arg1);
            return 0;
        case __NR_mmap:
        		klog(INFO,"Executing __NR_mmap Syscall\n");
        		ret = sys_mmap((void *) arg1,
        					(uint64_t) arg2,
        		        		(int32_t) arg3,
						(int32_t) arg3,
        		            (int32_t) arg5,
						(uint64_t) arg6);
        		return ret;
        case __NR_fstat:
        		klog(INFO,"Executing __NR_fstat Syscall\n");
        		ret = sys_fstat((int ) arg1, (fstat_t*) arg2);
        		return ret;

        case __NR_lseek:
        		klog(INFO,"Executing __NR_lseek Syscall\n");
        		ret = syscall_lseek((uint32_t) arg1, (uint64_t) arg2, (uint32_t) arg3);
        		return  ret;
        case __NR_syslog:
        		klog(INFO,"Executing __NR_syslog Syscall\n");
        		ret = syscall_ps((uint64_t) arg1);
        		return  ret;
        default:
            return -1;
    }
}

void init_syscall()
{
    uint64_t efer;

    efer = rdmsr(EFER);
    wrmsr(EFER, efer | EFER_SCE);
    wrmsr(STAR, (uint64_t) 0x8 << 32 | (uint64_t) 0x1B << 48);

    wrmsr(LSTAR, (uint64_t)_isr128);
    wrmsr(SFMASK, 0xC0000084);
}

//void pnum_xy (uint64_t value, int base, int x) {
//    if (value <= (base-1)) {
//        if (value < 10) pchar_xy((char) (value+48), RED, x++, 24);
//        else pchar_xy((char) (value+87), RED, x++, 24);
//    }
//    else {
//        pnum_xy(value/base, base, x++);
//        pnum_xy(value - (value/base)*base, base, x++);
//    }
//}

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

   //pnum_xy(pages_used, 10, 60);

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
        //if(c!=1){ //backspace
        //int prompt_len = strlen(PWD) + strlen(PS1);
        //int termidx = term_idx();

        //if(termidx  > prompt_len && c == BACKSPACE){

        		upd_term_buf(c);
        //}else if()

    }

    pic_send_eoi(1);
}

void debug_excep_handler(cpu_regs *regs) {
    kpanic("-- Debug Exception Fired --");
}

void overflow_handler() {
    kpanic("-- Overflow Exception Fired --");
}



void double_fault_handler() {
    kpanic("-- Double Fault Exception Fired --");
}

void stack_fault_handler() {
    kpanic("-- Stack Fault Exception Fired --");
}


void page_fault_handler(cpu_regs *regs) {
    klog(INFO,"-- Page Fault Execption Fired --\n");

    uint64_t error = regs->error & 0xf;
    klog(INFO,"Int Id: %d, Error: %d\n", regs->int_id, error);

    uint64_t fault_addr = read_cr2();
    klog(INFO,"Faulting address: %p\n", fault_addr);

    uint64_t p_write_err = error & PF_W;
    uint64_t p_prot_err  = error & PF_P;
    //uint64_t p_user_err  = error & PF_U;
    uint64_t p_rsvd_err  = error & PF_R;
    //uint64_t p_insf_err  = error & PF_I;


    if(p_prot_err & !p_write_err) {
        klog(INFO,"Page Fault at addr: %p\n", fault_addr);
        kpanic("Read permission error");
    }

    if(p_rsvd_err) {
        klog(INFO,"Page Fault at addr: %p\n", fault_addr);
        kpanic("Reserved page error");
    }

    vm_area_struct_t *vma = curr_task->mm->mmap;

    while(vma) {
        if(vma->vm_type == VM_STACK)
          if(fault_addr <= vma->vm_start && fault_addr >= vma->vm_end) break;

        if(fault_addr >= vma->vm_start && fault_addr <= vma->vm_end) break;
        vma = vma->vm_next;
    }

    if(vma == NULL) {
        klog(INFO,"Growing Stack!\n");
        uint64_t page_addr = (uint64_t) kmalloc(PAGE_SIZE);
        page_addr = page_addr - KERNAL_BASE_ADDRESS; //TODO: wirte va_to_pa();
        uint64_t falign_addr = align_down(fault_addr);
        map_proc(page_addr, falign_addr);
        return;

        //TODO: Grow stack
        //TODO: Grow heap
        //TODO: Stack Overflow!
        //TODO: Segmention Fault!
        //klog(INFO,"TODO: Handle growing stack, growing heap, stack overflow, SEGV\n");
        //TODO: this should be for seq fault only
        //kpanic("TODO: Handle growing stack, growing heap, stack overflow, SEGV");
    }

    if(p_prot_err && p_write_err) {
        //TODO: Handle COW
        klog(INFO,"Doing Copy On Write\n");
        //uint64_t page_addr = (uint64_t) kmalloc(PAGE_SIZE);
        //page_addr = page_addr - KERNAL_BASE_ADDRESS; //TODO: wirte va_to_pa();
        uint64_t falign_addr = align_down(fault_addr);
        map_proc(falign_addr, falign_addr);
        //memcpy((void *) page_addr, (void*) falign_addr, PAGE_SIZE);
        tlb_flush(curr_task->pml4);
        return;
    }

    //TODO: return for stack and heap growing.
    uint64_t page_addr = (uint64_t) kmalloc(PAGE_SIZE);
    page_addr = page_addr - KERNAL_BASE_ADDRESS; //TODO: wirte va_to_pa();
    uint64_t falign_addr = align_down(fault_addr);
    map_proc(page_addr, falign_addr);

    if(vma->file == NULL)
        return;

    //Copy file contents.
    uint64_t src, dst;
    int size;

    klog(INFO,"Copying file contents to %p\n", vma->vm_start);
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


void nmi_int_handler() {
    kpanic("-- NMI Interrupt Fired --");
}

void breakpoint_excep_handler() {
    kpanic("-- Breakpoint Exception Fired --");
}

void range_exeed_excep_handler() {
    kpanic("-- Bound Range Exceeded Exception Fired --");
}

void invalid_opcode_handler() {
    kpanic("-- Invalid Opcode Exception Fired --");
}

void no_device_excep_handler() {
    kpanic("-- No device found Exception Fired --");
}

void coproc_seg_overrun_handler() {
    kpanic("-- Coprocessor segment overrun Interrupt Fired --");
}

void invalid_tss_excep_handler() {
    kpanic("-- Invalid TSS exception Fired --");
}

void segment_not_present_handler() {
    kpanic("-- Segment not found exception Fired --");
}

void general_prot_fault_handler() {
    kpanic("-- General protection fault exception Fired --");
}



void alignment_check_handler() {
    kpanic("-- Alignment Check Exception Fired --");
}

void fpu_error_handler() {
    kpanic("-- FPU error interrupt Fired --");
}

void machine_check_handler() {
    kpanic("-- Machine check Exception Fired --");
}

void simd_fpu_excep_handler() {
    kpanic("-- SIMD FPU Exception Fired --");
}

void virtualization_excep_handler() {
    kpanic("-- Virtualiztion Exception Fired --");
}

void default_int_handler() {
    //while(1);
    kpanic("-- Unknown Interrupt Fired --");
}

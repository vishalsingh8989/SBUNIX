#include <sys/kprintf.h>
#include <sys/keyb.h>
#include <sys/defs.h>
#include <sys/pic.h>
#include <sys/utils.h>
#include <sys/idt.h>
#include <sys/vmm.h>
#include <sys/process.h>
#include <sys/asm_utils.h>
#include <sys/utils.h>
#include <sys/syscall.h>
#include <sys/terminal.h>
#include <sys/time.h>
#include <sys/env.h>
#include <dirent.h>
#include <logger.h>




char weekdayname[7][4] = {
    "Mon",
    "Tue",
	"Wed",
	"Thu",
	"Fri",
	"Sat",
	"Sun"
};

char monthname[13][5] = {
    "Jan",
    "Feb",
	"Mar",
	"Apr",
	"May",
	"Jun",
	"Jul",
	"Aug",
	"Oct",
	"Sep",
	"Nov",
	"Dec"
};


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
            klog(INFO, "Executing Exit Syscall\n");
            sys_exit();
            return 0;

        case __NR_read:
            klog(INFO,"Executing Read Syscall\n");
            ret = sys_read((uint64_t) arg1, (void *) arg2, (uint64_t) arg3);
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
            klog(INFO, "Executing pipe Syscall\n");
            ret = sys_pipe((uint64_t*) arg1);
            return ret;

        case __NR_dup2:
            klog(INFO, "Executing dup2 Syscall\n");
            ret = sys_dup2((uint64_t) arg1, (uint64_t) arg2);
            return ret;

        case __NR_getdents:
            klog(INFO, "Executing getdents Syscall\n");
            ret = sys_getdents((uint64_t) arg1, (struct dirent *) arg2, (uint64_t) arg3);
            return ret;

        case __NR_getcwd:
            klog(INFO, "Executing getcwd Syscall\n");
            ret = sys_getcwd((char *) arg1, (uint64_t) arg2);
            return ret;

        case __NR_chdir:
            klog(INFO, "Executing chdir Syscall\n");
            ret = sys_chdir((char *) arg1);
            return ret;

        case __NR_sched_yield:
            klog(INFO, "Executing yield Syscall\n");
            sys_sched_yield();
            return 0;

        case __NR_shutdown:
            klog(INFO, "Executing shutdown Syscall\n");
            sys_shutdown((uint64_t) arg1);
            return 0;

        case __NR_mmap:
            klog(INFO, "Executing __NR_mmap Syscall\n");
            ret = sys_mmap((void *) arg1, (uint64_t) arg2, (int32_t) arg3, (int32_t) arg3, (int32_t) arg5, (uint64_t) arg6);
            return ret;

        case __NR_fstat:
          klog(INFO, "Executing __NR_fstat Syscall\n");
          //ret = sys_fstat((int ) arg1, (fstat_t*) arg2);
          return 0;

        case __NR_lseek:
          klog(INFO, "Executing __NR_lseek Syscall\n");
          ret = sys_lseek((uint32_t) arg1, (uint64_t) arg2, (uint32_t) arg3);
          return  ret;

        case __NR_ps:
        		klog(INFO,"Executing __NR_syslog Syscall\n");
        		ret = sys_ps();
        		return  ret;

        case __NR_gettimeofday:
        		klog(INFO,"Executing __NR_gettimeofday Syscall\n");
        		ret = sys_gettimeofday((struct tm*)arg1);
        		return ret;

        case __NR_clearterm:
            klog(INFO,"Executing __NR_clearterm Syscall\n");
            ret =  sys_clear_term();
            return ret;

        case __NR_setuid:
           klog(INFO,"Executing __NR_clearterm Syscall\n");
           ret =  sys_setuid(arg1);
            return ret;

        default:
            return -1;
    }
}

void init_syscall()
{
    klog(BOOTLOG, "Intialize syscalls.\n");
    uint64_t efer;

    efer = rdmsr(EFER);
    wrmsr(EFER, efer | EFER_SCE);
    wrmsr(STAR, (uint64_t) 0x8 << 32 | (uint64_t) 0x1B << 48);

    wrmsr(LSTAR, (uint64_t)_isr128);
    wrmsr(SFMASK, 0xC0000084);
    klog(BOOTLOG, "Intialize syscalls. : Successful\n");
}

void timer_int_handler() {

    int sched = 0;
    struct tm utc;
    read_rtc(&utc);

    static int i = 0, s = 0, m = 0, h = 0;

    i++;
    if(i == 18) {
       s++;
       i = 0;
       sched = 1;
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

    // uptime start
    char sl = (char) (s%10+48);
    char sh = (char) (s/10+48);
    char ml = (char) (m%10+48);
    char mh = (char) (m/10+48);
    char hl = (char) (h%10+48);
    char hh = (char) (h/10+48);
    // uptime end

    //UTC start
	char usl = (char) (utc.tm_sec%10+48);
	char ush = (char) (utc.tm_sec/10+48);
	char uml = (char) (utc.tm_min%10+48);
	char umh = (char) (utc.tm_min/10+48);
	char uhl = (char) (utc.tm_hour%10+48);
	char uhh = (char) (utc.tm_hour/10+48);

	//char dl = (char) (utc.tm_mday%10+48);
	//char dh = (char) (utc.tm_mday/10+48);
	//char monl = (char) (utc.tm_mon%10+48);
	//char monh = (char) (utc.tm_mon/10+48);

	char yl1 = (char) (utc.tm_year%10+48);
	utc.tm_year = utc.tm_year/10;
	char yl2 = (char) (utc.tm_year%10+48);
	utc.tm_year = utc.tm_year/10;
	char yh1 = (char) (utc.tm_year%10+48);
	utc.tm_year = utc.tm_year/10;
	char yh2 = (char) (utc.tm_year%10+48);
    //UTC end


    //uptime set start

	pchar_xy(sl , GREEN, 79, 24);
	pchar_xy(sh , GREEN, 78, 24);
	pchar_xy(':', GREEN, 77, 24);
	pchar_xy(ml , GREEN, 76, 24);
	pchar_xy(mh , GREEN, 75, 24);
	pchar_xy(':', GREEN, 74, 24);
	pchar_xy(hl , GREEN, 73, 24);
	pchar_xy(hh , GREEN, 72, 24);

	pchar_xy(' ' , GREEN, 71, 24);
	pchar_xy('e' , GREEN, 70, 24);
	pchar_xy('m' , GREEN, 69, 24);
	pchar_xy('i' , GREEN, 68, 24);
	pchar_xy('T' , GREEN, 67, 24);
	//pchar_xy(' ' , GREEN, 65, 24);
	pchar_xy('p' , GREEN, 66, 24);
	pchar_xy('U' , GREEN, 65, 24);
   //pnum_xy(pages_used, 10, 60);
    //uptime set end


    pchar_xy('|' , GREEN, 63, 24);
    // UTC set start
    pchar_xy(yl1, GREEN, 61, 24);
    pchar_xy(yl2, GREEN, 60, 24);
    pchar_xy(yh1, GREEN, 59, 24);
    pchar_xy(yh2, GREEN, 58, 24);
    pchar_xy(' ', GREEN, 57, 24);
    pchar_xy('C', GREEN, 56, 24);
    pchar_xy('T', GREEN, 55, 24);
    pchar_xy('U', GREEN, 54, 24);
    pchar_xy(' ', GREEN, 53, 24);
    pchar_xy(usl, GREEN, 52, 24);
    pchar_xy(ush, GREEN, 51, 24);
    pchar_xy(':', GREEN, 50, 24);
    pchar_xy(uml, GREEN, 49, 24);
    pchar_xy(umh, GREEN, 48, 24);
    pchar_xy(':', GREEN, 47, 24);
    pchar_xy(uhl, GREEN, 46, 24);
    pchar_xy(uhh, GREEN, 45, 24);

    pchar_xy(' ', GREEN, 44, 24);
    pchar_xy(monthname[utc.tm_mon-1][2], GREEN, 43, 24);
	pchar_xy(monthname[utc.tm_mon-1][1] , GREEN, 42, 24);
	pchar_xy(monthname[utc.tm_mon-1][0] , GREEN, 41, 24);


    //kprintf("month name: %d\n", monthname[utc.tm_mon-1]);
	pchar_xy(' ' , GREEN, 40, 24);
	pchar_xy(weekdayname[utc.tm_wday-1][2] , GREEN, 39, 24);
	pchar_xy(weekdayname[utc.tm_wday-1][1] , GREEN, 38, 24);
	pchar_xy(weekdayname[utc.tm_wday-1][0] , GREEN, 37, 24);
    //UTC set end


	char time_buff[9] = {0};
	time_buff[0] = hh;
	time_buff[1] = hl;
	time_buff[2] = ':';
	time_buff[3] = mh;
	time_buff[4] = ml;
	time_buff[5] = ':';
	time_buff[6] = sl;
	time_buff[7] = sh;
	time_buff[8] = '\0';
	set_system_uptime(time_buff);

    if(sched == 1) {
         schedule();
       }

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


        upd_term_buf(c);


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
    uint64_t fault_addr = read_cr2();

    klog(IMP, "Page Fault! -- ID: %d, Error: %d, Address: %p\n", regs->int_id, error, fault_addr);

    uint64_t p_write_err = error & PF_W;
    uint64_t p_prot_err  = error & PF_P;
    //uint64_t p_user_err  = error & PF_U;
    uint64_t p_rsvd_err  = error & PF_R;
    //uint64_t p_insf_err  = error & PF_I;


    if(p_prot_err & !p_write_err) {
        klog(ERR, "Page Fault at addr: %p\n", fault_addr);
        kpanic("Read permission error");
    }

    if(p_rsvd_err) {
        klog(ERR, "Page Fault at addr: %p\n", fault_addr);
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
        klog(IMP, "Growing Stack! -- Addr: %p\n", fault_addr);
        uint64_t page_addr = (uint64_t) kmalloc(PAGE_SIZE);
        page_addr = page_addr - KERNAL_BASE_ADDRESS; //TODO: wirte va_to_pa();
        uint64_t falign_addr = align_down(fault_addr);
        map_proc(page_addr, falign_addr);
        tlb_flush(curr_task->pml4);
        return;

        //TODO: Grow stack
        //TODO: Grow heap
        //TODO: Stack Overflow!
        //TODO: Segmention Fault!
        //kprintf("TODO: Handle growing stack, growing heap, stack overflow, SEGV\n");
        //TODO: this should be for seq fault only
        //kpanic("TODO: Handle growing stack, growing heap, stack overflow, SEGV");
    }

    if(p_prot_err && p_write_err) {
        //TODO: Handle COW
        /*
        klog(IMP, "Copy On Write! -- Addr: %p\n", fault_addr);
        uint64_t falign_addr = align_down(fault_addr);
        map_proc(falign_addr, falign_addr);
        tlb_flush(curr_task->pml4);
        */

        klog(IMP, "Copy On Write! -- Addr: %p\n", fault_addr);
        uint64_t page_addr = (uint64_t) kmalloc(PAGE_SIZE);
        uint64_t falign_addr = align_down(fault_addr);
        memcpy((void *) page_addr, (void *) falign_addr, PAGE_SIZE);
        page_addr = page_addr - KERNAL_BASE_ADDRESS;
        map_proc(page_addr, falign_addr);
        tlb_flush(curr_task->pml4);
        return;
    }

    //TODO: return for stack and heap growing.
    uint64_t page_addr = (uint64_t) kmalloc(PAGE_SIZE);
    page_addr = page_addr - KERNAL_BASE_ADDRESS; //TODO: wirte va_to_pa();
    uint64_t falign_addr = align_down(fault_addr);
    map_proc(page_addr, falign_addr);
    tlb_flush(curr_task->pml4);

    if(vma->file == NULL)
        return;

    //Copy file contents.
    uint64_t src, dst;
    int size;

    klog(INFO, "Copying file contents to %p\n", vma->vm_start);
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

    __asm__ __volatile__("sti;");

    init_proc("bin/sbush", 1);

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
    kpanic("-- Unknown Interrupt Fired --");
}

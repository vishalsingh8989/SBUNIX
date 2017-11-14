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
#include<dirent.h>

extern void _isr128(void);

#define OFFS 0xffffffff00000000

//void syscall_handler(uint64_t s_num, uint64_t arg1, uint64_t arg2, uint64_t arg3)
uint64_t syscall_handler(cpu_regs* regs)
{
	uint64_t result = 0;
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
            break;
        case __NR_getcwd:
        		kprintf("Executing __NR_getcwd syscall\n");
        		result = sys_getcwd((char *) arg1, (uint64_t) arg2);
			break;
        case __NR_chdir:
        		kprintf("Executing __NR_chdir syscall\n");
        		result  = sys_chdir((const char *)arg1);
        		break;
        case __NR_open:
        		kprintf("Executing __NR_open syscall\n");
        		result = sys_open((const char *) arg1, (uint32_t) arg2);
        		kprintf("Result : %d\n", result);
        		break;
        case __NR_getdents:
    			kprintf("Executing __NR_getdents syscall\n");
    			result = sys_getdents((uint32_t)arg1,(struct dirent*)arg2, (uint32_t)arg3);
    			struct dirent* temp = (struct dirent*)arg2;
    			kprintf("Result copied : %s\n", temp->d_name);
    			break;

        default:
        		kprintf("In default syscall\n");
            break;
    }



    kprintf("Exit syscall handler, syscall no: %d\n", s_num);
    return 0;
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

void int_handler_129(){
            kprintf("Interrupt 129 triggered\n");
}

void int_handler_130(){
            kprintf("Interrupt 130 triggered\n");
}

void int_handler_131(){
            kprintf("Interrupt 131 triggered\n");
}

void int_handler_132(){
            kprintf("Interrupt 132 triggered\n");
}

void int_handler_133(){
            kprintf("Interrupt 133 triggered\n");
}

void int_handler_134(){
            kprintf("Interrupt 134 triggered\n");
}

void int_handler_135(){
            kprintf("Interrupt 135 triggered\n");
}

void int_handler_136(){
            kprintf("Interrupt 136 triggered\n");
}

void int_handler_137(){
            kprintf("Interrupt 137 triggered\n");
}

void int_handler_138(){
            kprintf("Interrupt 138 triggered\n");
}

void int_handler_139(){
            kprintf("Interrupt 139 triggered\n");
}

void int_handler_140(){
            kprintf("Interrupt 140 triggered\n");
}

void int_handler_141(){
            kprintf("Interrupt 141 triggered\n");
}

void int_handler_142(){
            kprintf("Interrupt 142 triggered\n");
}

void int_handler_143(){
            kprintf("Interrupt 143 triggered\n");
}

void int_handler_144(){
            kprintf("Interrupt 144 triggered\n");
}

void int_handler_145(){
            kprintf("Interrupt 145 triggered\n");
}

void int_handler_146(){
            kprintf("Interrupt 146 triggered\n");
}

void int_handler_147(){
            kprintf("Interrupt 147 triggered\n");
}

void int_handler_148(){
            kprintf("Interrupt 148 triggered\n");
}

void int_handler_149(){
            kprintf("Interrupt 149 triggered\n");
}

void int_handler_150(){
            kprintf("Interrupt 150 triggered\n");
}

void int_handler_151(){
            kprintf("Interrupt 151 triggered\n");
}

void int_handler_152(){
            kprintf("Interrupt 152 triggered\n");
}

void int_handler_153(){
            kprintf("Interrupt 153 triggered\n");
}

void int_handler_154(){
            kprintf("Interrupt 154 triggered\n");
}

void int_handler_155(){
            kprintf("Interrupt 155 triggered\n");
}

void int_handler_156(){
            kprintf("Interrupt 156 triggered\n");
}

void int_handler_157(){
            kprintf("Interrupt 157 triggered\n");
}

void int_handler_158(){
            kprintf("Interrupt 158 triggered\n");
}

void int_handler_159(){
            kprintf("Interrupt 159 triggered\n");
}

void int_handler_160(){
            kprintf("Interrupt 160 triggered\n");
}

void int_handler_161(){
            kprintf("Interrupt 161 triggered\n");
}

void int_handler_162(){
            kprintf("Interrupt 162 triggered\n");
}

void int_handler_163(){
            kprintf("Interrupt 163 triggered\n");
}

void int_handler_164(){
            kprintf("Interrupt 164 triggered\n");
}

void int_handler_165(){
            kprintf("Interrupt 165 triggered\n");
}

void int_handler_166(){
            kprintf("Interrupt 166 triggered\n");
}

void int_handler_167(){
            kprintf("Interrupt 167 triggered\n");
}

void int_handler_168(){
            kprintf("Interrupt 168 triggered\n");
}

void int_handler_169(){
            kprintf("Interrupt 169 triggered\n");
}

void int_handler_170(){
            kprintf("Interrupt 170 triggered\n");
}

void int_handler_171(){
            kprintf("Interrupt 171 triggered\n");
}

void int_handler_172(){
            kprintf("Interrupt 172 triggered\n");
}

void int_handler_173(){
            kprintf("Interrupt 173 triggered\n");
}

void int_handler_174(){
            kprintf("Interrupt 174 triggered\n");
}

void int_handler_175(){
            kprintf("Interrupt 175 triggered\n");
}

void int_handler_176(){
            kprintf("Interrupt 176 triggered\n");
}

void int_handler_177(){
            kprintf("Interrupt 177 triggered\n");
}

void int_handler_178(){
            kprintf("Interrupt 178 triggered\n");
}

void int_handler_179(){
            kprintf("Interrupt 179 triggered\n");
}

void int_handler_180(){
            kprintf("Interrupt 180 triggered\n");
}

void int_handler_181(){
            kprintf("Interrupt 181 triggered\n");
}

void int_handler_182(){
            kprintf("Interrupt 182 triggered\n");
}

void int_handler_183(){
            kprintf("Interrupt 183 triggered\n");
}

void int_handler_184(){
            kprintf("Interrupt 184 triggered\n");
}

void int_handler_185(){
            kprintf("Interrupt 185 triggered\n");
}

void int_handler_186(){
            kprintf("Interrupt 186 triggered\n");
}

void int_handler_187(){
            kprintf("Interrupt 187 triggered\n");
}

void int_handler_188(){
            kprintf("Interrupt 188 triggered\n");
}

void int_handler_189(){
            kprintf("Interrupt 189 triggered\n");
}

void int_handler_190(){
            kprintf("Interrupt 190 triggered\n");
}

void int_handler_191(){
            kprintf("Interrupt 191 triggered\n");
}

void int_handler_192(){
            kprintf("Interrupt 192 triggered\n");
}

void int_handler_193(){
            kprintf("Interrupt 193 triggered\n");
}

void int_handler_194(){
            kprintf("Interrupt 194 triggered\n");
}

void int_handler_195(){
            kprintf("Interrupt 195 triggered\n");
}

void int_handler_196(){
            kprintf("Interrupt 196 triggered\n");
}

void int_handler_197(){
            kprintf("Interrupt 197 triggered\n");
}

void int_handler_198(){
            kprintf("Interrupt 198 triggered\n");
}

void int_handler_199(){
            kprintf("Interrupt 199 triggered\n");
}



#include <sys/defs.h>
#include <sys/vmm.h>
#include <sys/mm_types.h>
#include <sys/utils.h>
#include <sys/process.h>
#include <sys/kprintf.h>
#include <sys/elf64.h>
#include <sys/string.h>
#include <sys/gdt.h>
#include <sys/asm_utils.h>

uint64_t g_pid;
struct mm_struct *kern_mm;

uint64_t kern_stack;
uint64_t user_stack;

void print_task_list()
{

    if(curr_task == NULL) {
        kprintf("No tasks in the queue currently");
        return;
    }

    int i = 0;
    kprintf("Tasks in queue are: \n");
    kprintf("%d : %s\n", i++, curr_task->pcmd_name);
    task_struct_t * temp =  curr_task;
    while(temp->next_task != curr_task) {
        kprintf("%d : %s\n", i++, temp->pcmd_name);
        temp = temp->next_task;
    }
}

pid_t get_pid() {
    if(g_pid == MAX_PID)
        g_pid = 300;
    return g_pid++;
}

void context_switch(task_struct_t *prev_task, task_struct_t *next_task)
{
    //TODO: does this have to be atomic.
    __asm__ __volatile__ (
        //"pushq %%rax;"
        //"pushq %%rbx;"
	    //"pushq %%rcx;"
	    //"pushq %%rdx;"
	    //"pushq %%rbp;"
	    //"pushq %%rsi;"
	    //"pushq %%rdi;"
	    //"pushq %%r8;"
	    //"pushq %%r9;"
	    //"pushq %%r10;"
	    //"pushq %%r11;"
	    //"pushq %%r12;"
	    //"pushq %%r13;"
	    //"pushq %%r14;"
	    //"pushq %%r15;"
        "movq %%rsp, %0;"
        "movq %1, %%rsp;"
	    //"popq %%r15;"
	    //"popq %%r14;"
	    //"popq %%r13;"
	    //"popq %%r12;"
	    //"popq %%r11;"
	    //"popq %%r10;"
	    //"popq %%r9;"
	    //"popq %%r8;"
	    //"popq %%rdi;"
	    //"popq %%rsi;"
	    //"popq %%rbp;"
	    //"popq %%rdx;"
	    //"popq %%rcx;"
	    //"popq %%rbx;"
	    //"popq %%rax;"
        "retq;"
        ::"r"(&(prev_task->stack_p)),
          "r"(next_task->stack_p)
    );
}

void add_to_queue(task_struct_t *new_task)
{
    print_task_list();

    if(curr_task == NULL) {
        curr_task = new_task;
        curr_task->next_task = curr_task;
        return;
    }

    task_struct_t *temp = curr_task->next_task;
    while(temp->next_task != curr_task) {
        temp = temp->next_task;
        kprintf("temp: %s\n", temp->pcmd_name);
    }
    temp->next_task = new_task;

    new_task->next_task = curr_task;
    curr_task = new_task;

}

//TODO: replace this with better schedular
task_struct_t *get_next_task()
{
    return curr_task->next_task;
}

void schedule()
{

    //TODO: shouldn't this be atomic?
    task_struct_t *prev_task = curr_task;
    curr_task = get_next_task();

    print_task_list();

    if(curr_task != prev_task) {
        write_cr3(curr_task->pml4);
        context_switch(prev_task, curr_task);
    }
}

//TODO: change the name to idle.
void idle_proc()
{
    kprintf("--Inside IDLE Process--\n");

    while(1) {
        //__asm__ __volatile("hlt;");
        //__asm__ __volatile("sti;");
        schedule();
    }
}

void switch_to_userspace(task_struct_t *task)
{
    set_tss_rsp((void *) task->kern_stack);
    kern_stack = task->kern_stack;

    __asm__ __volatile__(
            "sti;"
            "movq %0, %%cr3;"
            "movq $0x23, %%rax;"
            "movq %%rax, %%ds;"
            "movq %%rax, %%es;"
            "movq %%rax, %%fs;"
            "movq %%rax, %%gs;"

            "pushq %%rax;"
            "pushq %1;"
            "pushfq;"
            "popq %%rax;"
            "orq $0x200, %%rax;"
            "pushq %%rax;"
            "pushq $0x2B;"
            "pushq %2;"
            "movq $0x0, %%rdi;"
            "movq $0x0, %%rsi;"
            "iretq;"
            ::"r"(task->pml4), "r"(task->stack_p), "r"(task->rip) : "memory", "rax"
    );
}

task_struct_t *init_proc(const char *name, int type)
{
    task_struct_t *init_task = (task_struct_t *) kmalloc(sizeof(task_struct_t *));
    if(!init_task) {
        kpanic("Not able to allocate task struct for init\n");
        return NULL;
    }
    memset(init_task, 0, sizeof(task_struct_t *));

    if (type == 0) {
        kern_task = (task_struct_t *) kmalloc(sizeof(task_struct_t *));
        if(!kern_task) {
            kpanic("Not able to allocate task struct for init\n");
            return NULL;
        }
        memset(kern_task, 0, sizeof(task_struct_t *));

        kern_mm = (mm_struct_t *) kmalloc(PAGE_SIZE);
        if(!kern_mm) {
            kpanic("Not able to allocate mm struct for init\n");
            return NULL;
        }
    }

    uint64_t * stack = (uint64_t *) kmalloc(PAGE_SIZE);
    if(!stack) {
        kpanic("Not able to allocate stack for init\n");
    }

    init_task->state     = TASK_RUNNABLE;
    init_task->mm        = kern_mm;
    init_task->sleep_t   = 0;
    init_task->pid       = get_pid();
    init_task->ppid      = 0;
    init_task->prev_task = NULL;
    init_task->parent    = NULL;
    init_task->sibling   = NULL;
    init_task->child     = NULL;
    init_task->pml4      = read_cr3();
    strcpy(init_task->cdir_name, "/bin");

    if (type == 0) { //create idle process
        strcpy(init_task->pcmd_name, "idle");
        *(stack + 510) = (uint64_t) &idle_proc;;
        init_task->stack_p   = init_task->kern_stack = (uint64_t) &stack[510];
        add_to_queue(init_task);
    }
    else { //Load user process
        strcpy(init_task->pcmd_name, "bin/init");

        struct page_map_level_4* old_pml4 = (struct page_map_level_4*) read_cr3();
        struct page_map_level_4* new_pml4 = (struct page_map_level_4*) kmalloc(PAGE_SIZE);

        old_pml4 = (struct page_map_level_4 *)((uint64_t) old_pml4 + KERNAL_BASE_ADDRESS); //TODO: use pa_to_va

        new_pml4->pml4e[511] = old_pml4->pml4e[511];

        init_task->pml4 = (uint64_t) new_pml4 - KERNAL_BASE_ADDRESS;

        write_cr3(init_task->pml4);

        init_task->kern_stack = (uint64_t) &stack[510];

        //Load process.
        int ret = load_elf(init_task, name);
        if(ret == 0)
            kprintf("Loading Exe Sucessfull\n");
        else
            kprintf("Error Loading Exe\n");

        write_cr3((uint64_t)old_pml4 - KERNAL_BASE_ADDRESS);

        add_to_queue(init_task);

        switch_to_userspace(init_task);
    }

    return init_task;
}

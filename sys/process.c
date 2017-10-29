#include <sys/defs.h>
#include <sys/vmm.h>
#include <sys/mm_types.h>
#include <sys/utils.h>
#include <sys/process.h>
#include <sys/kprintf.h>

uint64_t g_pid;

task_struct_t *curr_task;
task_struct_t *init_task;

pid_t get_pid() {
    if(g_pid == MAX_PID)
        g_pid = 300;
    return g_pid++;
}

void context_switch(task_struct_t *prev_task, task_struct_t *next_task)
{
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

void schedule()
{

    task_struct_t *prev_task = curr_task;
    curr_task = curr_task->next_task;

    //struct mm_struct *curr_mm, *prev_mm;
    //curr_mm = curr_task->mm;
    //prev_mm = prev_task->mm;

    //if(prev_mm != curr_mm) 
    //    write_cr3(curr_mm->pml4);
   
    if(curr_task != prev_task)
        context_switch(prev_task, curr_task);
}

void init_entry() 
{
    kprintf("Inside Init!!\n");
    while(1) {
        schedule();
    }
}

task_struct_t *init_proc(const char *name)
{
    task_struct_t *init_task = (task_struct_t *) kmalloc(sizeof(task_struct_t *));
    if(!init_task) {
        kpanic("Not able to allocate task struct for init\n");
        return NULL;
    }
    memset(init_task, 0, sizeof(task_struct_t *));

    task_struct_t *kernel_task = (task_struct_t *) kmalloc(sizeof(task_struct_t *));
    if(!kernel_task) {
        kpanic("Not able to allocate task struct for init\n");
        return NULL;
    }
    memset(kernel_task, 0, sizeof(task_struct_t *));

    uint64_t * stack = (uint64_t *) kmalloc(PAGE_SIZE);
    if(!stack) {
        kpanic("Not able to allocate stack for init\n");
        return NULL;
    }

    *(stack + 510) = (uint64_t) &init_entry;

    init_task->state     = TASK_RUNNABLE;
    init_task->pid       = 0;
    init_task->stack_p   = (uint64_t) &stack[510];
    init_task->mm        = NULL; //TODO:
    init_task->sleep_t   = 0;
    init_task->pid       = get_pid();
    init_task->next_task = init_task;
    init_task->prev_task = NULL;
    init_task->parent    = NULL;
    init_task->sibling   = NULL;
    init_task->child     = NULL;
    strcpy(init_task->pcmd_name, "init");
    strcpy(init_task->cdir_name, "/bin");


    curr_task = kernel_task;
    kernel_task->next_task = init_task;

    schedule();

    return init_task;
}

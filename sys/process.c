#include <sys/defs.h>
#include <sys/vmm.h>
#include <sys/mm_types.h>
#include <sys/utils.h>
#include <sys/process.h>
#include <sys/kprintf.h>
#include <sys/elf64.h>
#include <sys/string.h>

uint64_t g_pid;

task_struct_t *curr_task;
task_struct_t *init_task;
task_struct_t *kern_task;

struct mm_struct *kern_mm;

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

void add_to_queue(task_struct_t *task)
{
    task_struct_t *temp;
    temp = curr_task->next_task;
    curr_task->next_task = task;
    task->next_task = temp;
}

task_struct_t *get_next_task()
{
    return curr_task->next_task;
}

void schedule()
{

    //TODO: shouldn't this be atomic? 
    task_struct_t *prev_task = curr_task;
    //curr_task = curr_task->next_task;
    curr_task = get_next_task();

    //struct mm_struct *curr_mm, *prev_mm;
    //curr_mm = curr_task->mm;
    //prev_mm = prev_task->mm;

    //if(prev_mm != curr_mm) 
    //    write_cr3(curr_mm->pml4);
   
    if(curr_task != prev_task)
        context_switch(prev_task, curr_task);
}

//TODO: this should come from main.
void test_entry() 
{
    kprintf("Inside test process with pid: %d!!\n", curr_task->pid);
    schedule();
}

void init_entry() 
{
    kprintf("Inside Init process!!\n");
    task_struct_t *temp_task = (task_struct_t *) kmalloc(sizeof(task_struct_t *));
    int ret = load_elf(temp_task, "bin/ls");

    if(ret == 0) 
        kprintf("loading exe sucessfull\n");
    else
        kprintf("error loading exe\n");

    while(1) {
        schedule();
    }
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
            kpanic("Not able to allocate stack for init\n");
            return NULL;
        }
    }

    uint64_t * stack = (uint64_t *) kmalloc(PAGE_SIZE);
    if(!stack) {
        kpanic("Not able to allocate stack for init\n");
        return NULL;
    }

    init_task->state     = TASK_RUNNABLE;
    init_task->pid       = 0;
    init_task->mm        = kern_mm;
    init_task->sleep_t   = 0;
    init_task->pid       = get_pid();
    init_task->prev_task = NULL;
    init_task->parent    = NULL;
    init_task->sibling   = NULL;
    init_task->child     = NULL;
    strcpy(init_task->pcmd_name, "init");
    strcpy(init_task->cdir_name, "/bin");

    if (type == 0) {
        *(stack + 510) = (uint64_t) &init_entry;
        init_task->stack_p   = (uint64_t) &stack[510];
        init_task->next_task = init_task;
        curr_task = kern_task;
        kern_task->next_task = init_task;
        //For testing.
        //init_proc("test_proc_1", 1);
        //init_proc("test_proc_2", 1);
        schedule(); //TODO: remove this by taking tasks from schedular only.
    }
    else {
        *(stack + 510) = (uint64_t) &test_entry;
        init_task->stack_p   = (uint64_t) &stack[510];
        add_to_queue(init_task);
        //task_struct_t *temp;
        //temp = curr_task->next_task;
        //curr_task->next_task = init_task;
        //init_task->next_task = temp;
    }

    return init_task;
}

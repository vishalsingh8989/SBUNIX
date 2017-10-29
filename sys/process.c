#include <sys/defs.h>
#include <sys/vmm.h>
#include <sys/mm_types.h>
#include <sys/utils.h>
#include <sys/process.h>
#include <sys/kprintf.h>

uint64_t g_pid;

task_struct_t *curr_task;
task_struct_t *prev_task;

pid_t get_pid() {
    if(g_pid == MAX_PID)
        g_pid = 300;
    return g_pid++;
}

void init_entry() 
{
    kprintf("Inside Init!!\n");
}

task_struct_t *init_proc(const char *name)
{
    task_struct_t *proc = (task_struct_t *) kmalloc(sizeof(task_struct_t *));
    if(!proc) {
        kpanic("Not able to allocate task struct for init\n");
        return NULL;
    }
    memset(proc, 0, sizeof(task_struct_t *));

    uint64_t * stack = kmalloc(PAGE_SIZE);
    if(!stack) {
        kpanic("Not able to allocate stack for init\n");
        return NULL;
    }

    *(stack + 510) = (uint64_t) &init_entry;

    proc->state   = TASK_RUNNABLE;
    proc->pid     = 0;
    proc->stack_p = (uint64_t) (stack + 510);
    proc->mm      = NULL; //TODO:
    proc->sleep_t = 0;
    proc->pid     = get_pid();
    strcpy(proc->pcmd_name, "init");
    strcpy(proc->cdir_name, "/bin");

    return proc;
}

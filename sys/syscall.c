#include <sys/defs.h>
#include <sys/vmm.h>
#include <sys/process.h>
#include <sys/kprintf.h>

void sys_exit()
{
    //TODO: cleanup

    task_struct_t* next_task = curr_task->next_task;

    kprintf("Context switch from %s to %s\n", curr_task->pcmd_name, next_task->pcmd_name);
    schedule();
    //context_switch(next_task, curr_task);
}

int sys_execve(char *fname, char **argv, char **envp)
{
   //TODO:
   return 0;
}

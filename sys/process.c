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
#include <sys/time.h>
#include<logger.h>

uint64_t g_pid = 1500;
struct mm_struct *kern_mm;

uint64_t kern_stack;
uint64_t user_stack;

void print_task_list()
{

    if(curr_task == NULL) {
        klog(IMP, "Tasks in queue are: \n");
        klog(IMP, "No tasks in the queue!\n");
        return;
    }

    int i = 0;
    klog(INFO, "Tasks in queue are: \n");
    klog(INFO, "%d : %s\n", i++, curr_task->pcmd_name);
    task_struct_t * temp =  curr_task->next_task;
    kprintf("PID      TIME    CMD\n");
    while(temp != curr_task) {
        kprintf("%d   %s  %s\n", temp->pid,temp->start_time, temp->pcmd_name);
        temp = temp->next_task;
        if(temp->pid == 0){
            break;
        }
        sleep(100);
    }
    kprintf("%d   %s  %s\n", temp->pid, temp->start_time, temp->pcmd_name);
}

pid_t get_pid() {
    if(g_pid == 1000+MAX_PID)
        g_pid = 1000;
    return g_pid++;
}

void context_switch(task_struct_t *prev_task, task_struct_t *next_task)
{

    //print_task_list();
    set_tss_rsp((void *) next_task->kern_stack);
    kern_stack = next_task->kern_stack;

    klog(IMP, "In task %s with rip: %p, pid: %d, pml4: %p, rsp; %p, Switching to %s with rip: %p, pid: %d, pml4: %p, rsp: %p\n",
              prev_task->pcmd_name, prev_task->rip, prev_task->pid, prev_task->pml4, prev_task->stack_p,
              next_task->pcmd_name, next_task->rip, next_task->pid, next_task->pml4, next_task->stack_p);

    __asm__ __volatile__ (
      "cli;"
      "movq $1f, %0;"
      :"=g"(prev_task->rip)
    );

    __asm__ __volatile__ (
      "cli;"
      "movq %%rsp, (%1);"
      //"movq $1f, %0;"
      "movq %2, %%rsp;"
      //"movq %4, %%cr3;"
      "pushq %3;"
      "retq;"
      "1:\t"
      :"=g"(prev_task->rip)
      :"r"(&(prev_task->kern_stack)), "r"(next_task->kern_stack), "r"(next_task->rip), "r"(next_task->pml4)
    );
}

void add_to_queue(task_struct_t *new_task)
{
    if(curr_task == NULL) {
        curr_task = new_task;
        curr_task->next_task = curr_task->prev_task = new_task;
        //print_task_list();
        return;
    }

    task_struct_t *curr_next = curr_task->next_task;

    curr_task->next_task = new_task;
    new_task->prev_task  = curr_task;
    new_task->next_task  = curr_next;
    curr_next->prev_task = new_task;

    //print_task_list();
}

/*
void add_to_zombie_queue(task_struct_t *task)
{
    if(zombie_task == NULL) {
        task->next_task = NULL;
        task->prev_task = NULL;
        zombie_task = task;
    }
    else {
        task->next_task = zombie_task;
        task->prev_task = NULL;
        zombie_task = task;
    }
}
*/

void remove_from_queue(task_struct_t *task)
{
     task->state = TASK_ZOMBIE;
     klog(IMP, "Adding %s to zombie queue\n", curr_task->pcmd_name);


     task_struct_t *prev_task = task->prev_task;
     task_struct_t *next_task = task->next_task;

     task->prev_task = NULL;
     task->next_task = zombie_task;
     zombie_task = task; //TODO: used add_to_zombie_queue
     //add_to_zombie_queue(curr_task);

     prev_task->next_task = next_task;
     next_task->prev_task = prev_task;

     if(task->parent != NULL) {
       prev_task = task;
       curr_task = task->parent;
     }
     else {
       prev_task = task;
       curr_task = next_task;
     }

     //print_task_list();
     /*
     kprintf("Removing T: %s:%d\n", task->pcmd_name, task->pid);
     kprintf("NT: %s:%d, PT: %s:%d, CT: %s:%d\n", next_task->pcmd_name, next_task->pid,
                                                  prev_task->pcmd_name, prev_task->pid,
                                                  curr_task->pcmd_name, curr_task->pid);
     */

     if(curr_task != prev_task) {
        set_tss_rsp((void *) curr_task->kern_stack);
        kern_stack = curr_task->kern_stack;
        write_cr3(curr_task->pml4);
        context_switch(prev_task, curr_task);
     }
}

void reap_zombies()
{
    if(zombie_task == NULL) {
      return;
    }
    else {
      task_struct_t *temp = zombie_task;
      zombie_task = zombie_task->next_task;
      klog(IMP, "Reaper is freeing memory of %s\n", temp->pcmd_name);
      delete_task(temp);
      reap_zombies();
    }

}

//TODO: replace this with better schedular
task_struct_t *get_next_task()
{
    if(curr_task->next_task == NULL && curr_task->prev_task == NULL) {
      print_task_list();
      kpanic("Both next and prev tasks are NULL!!");
    }
    else if (curr_task->next_task == NULL) {
      print_task_list();
      kpanic("Next task is NULL!!");
    }

    return curr_task->next_task;
}

void schedule()
{
    //TODO: shouldn't this be atomic?
    task_struct_t *prev_task = curr_task;
    curr_task = get_next_task();

    //print_task_list();

    if(curr_task != prev_task) {
        write_cr3(curr_task->pml4);
        context_switch(prev_task, curr_task);
    }
}

void idle_proc()
{

    zombie_task = NULL;

    while(1) {
        //klog(IMP, "--Inside IDLE Process--\n");
        if (zombie_task != NULL) {
           reap_zombies(); //Not safe.
        }
        schedule();
    }
}

void switch_to_userspace(task_struct_t *task)
{
    klog(IMP, "In task %s with rip: %p, pid: %d, pml4: %p, rsp: %p Switching to userspace!\n",
              task->pcmd_name, task->rip, task->pid, task->pml4, task->stack_p);

    curr_task = task;

    set_tss_rsp((void *) task->kern_stack);
    kern_stack = task->kern_stack;

    __asm__ __volatile__(
            "cli;"
            "movq %0, %%cr3;"
            "movq $0x23, %%rax;"

            "pushq %%rax;"
            "pushq %1;"
            "pushfq;"
            "popq %%rax;"
            "orq $0x200, %%rax;"
            "pushq %%rax;"
            "pushq $0x2B;"
            "pushq %2;"
            "movq $0x0, %%rax;"
            "movq $0x0, %%rbx;"
            "movq $0x0, %%rcx;"
            "movq $0x0, %%rdx;"
            "movq $0x0, %%rbp;"
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

    get_system_uptime(init_task->start_time);
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
        strcpy(init_task->pcmd_name, "idle_proc");
        *(stack + 510) = (uint64_t) &idle_proc;
        init_task->stack_p = init_task->kern_stack = (uint64_t) &stack[510];
        init_task->rip = (uint64_t) &idle_proc;
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
            klog(INFO, "Loading %s was sucessfull\n", init_task->pcmd_name);
        else
            klog(ERR, "Error loading %s\n", init_task->pcmd_name);

        write_cr3((uint64_t)old_pml4 - KERNAL_BASE_ADDRESS);

        add_to_queue(init_task);

        switch_to_userspace(init_task);
    }

    return init_task;
}

void delete_task(task_struct_t *task)
{
    //print_task_list();

    //Remove MM amd VMA recursively.
    klog(INFO, "Freeing MM and VMA's for task %s, Curr Task: %s\n", task->pcmd_name, curr_task->pcmd_name);
    if(task->mm != NULL) {
        vm_area_struct_t *vma = task->mm->mmap;
        while(vma != NULL) {
          vm_area_struct_t *temp = vma;
          vma = vma->vm_next;
          kfree((uint64_t*) temp);
        }
        kfree((uint64_t *) task->mm);
    }

    //Remove stack.
    klog(INFO, "Freeing kernal STACK for task %s, Curr Task: %s\n", task->pcmd_name, curr_task->pcmd_name);
    if(task->kern_stack != 0) kfree((uint64_t *) align_down(task->kern_stack));

    //Remove page tables.
    klog(INFO, "Freeing page tables for task %s, Curr Task: %s\n", task->pcmd_name, curr_task->pcmd_name);
    if(task->pml4 != 0) delete_ptables(task->pml4); //This throws error, Need to debug this.

    //Remove task_struct.
    klog(INFO, "Freeing task structure for task %s, Curr Task: %s\n", task->pcmd_name, curr_task->pcmd_name);
    if(task != 0) kfree((uint64_t *) task);
}

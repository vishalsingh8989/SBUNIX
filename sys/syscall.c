#include <sys/defs.h>
#include <sys/vmm.h>
#include <sys/process.h>
#include <sys/kprintf.h>
#include <sys/string.h>
#include <sys/utils.h>
#include <sys/asm_utils.h>
#include <sys/fs.h>
#include <sys/elf64.h>

extern void fork_return(void);

void sys_exit()
{
    //TODO: cleanup

    task_struct_t* next_task = curr_task->next_task;

    kprintf("Context switch from %s to %s\n", curr_task->pcmd_name, next_task->pcmd_name);
    schedule();
}

uint64_t sys_fork()
{
    task_struct_t *child_task;

    uint64_t *stack = (uint64_t *) kmalloc(PAGE_SIZE);
    if(!stack) {
        kpanic("Not able to allocate stack!!");
    }

    child_task = (task_struct_t *) kmalloc(PAGE_SIZE);
    child_task->pml4 = (uint64_t) kmalloc(PAGE_SIZE) - KERNAL_BASE_ADDRESS;
    child_task->mm = (mm_struct_t *) kmalloc(PAGE_SIZE);
    child_task->stack_p = child_task->kern_stack = (uint64_t) &stack[510];

    child_task->state     = TASK_RUNNABLE;
    child_task->pid       = get_pid();
    child_task->ppid      = curr_task->pid;
    child_task->sibling   = NULL;
    strcpy(child_task->pcmd_name, curr_task->pcmd_name);
    prev_task = curr_task;

    //Set up virtual memory.
    setup_child_ptables(child_task->pml4);

    //queue the child after parent task. TODO: add function "add_child_to_queue"
    child_task->prev_task = curr_task;
    child_task->next_task = curr_task->next_task;
    curr_task->child      = child_task;
    curr_task->next_task  = child_task;

    write_cr3(child_task->pml4);

    //Deep copy
    uint64_t * chld_stack = (uint64_t *) child_task->kern_stack;
    uint64_t * curr_stack = (uint64_t *) curr_task->kern_stack;
    memcpy(chld_stack-510, curr_stack-510, 4080);
    memcpy(chld_stack, curr_stack, 4080);
    memcpy(child_task->fd, curr_task->fd, sizeof(fd_t) * MAX_FILES);
    memcpy(child_task->mm, curr_task->mm, sizeof(mm_struct_t));
    if(curr_task->mm->mmap != NULL) {
        vm_area_struct_t *curr_vma = curr_task->mm->mmap;
        vm_area_struct_t *chld_vma = (vm_area_struct_t *) kmalloc(PAGE_SIZE);
        memcpy(chld_vma, curr_vma, sizeof(vm_area_struct_t));
        child_task->mm->mmap = chld_vma;
        while(curr_vma->vm_next != NULL) {
            curr_vma = curr_vma->vm_next;
            chld_vma->vm_next = (vm_area_struct_t *) kmalloc(PAGE_SIZE);
            chld_vma = chld_vma->vm_next;
            memcpy(chld_vma, curr_vma, sizeof(vm_area_struct_t));
        }
        chld_vma->vm_next = NULL;
    }
    else {
       child_task->mm->mmap = NULL;
    }

    write_cr3(curr_task->pml4);
    tlb_flush(curr_task->pml4);

    volatile uint64_t stack_loc;
    volatile uint64_t rip_loc;

    //TODO: make these are functions in asm utils.h
    __asm__ __volatile__("movq $2f, %0;" "2:\t" : "=g"(rip_loc));
    __asm__ __volatile__("movq %%rsp, %0" : "=r"(stack_loc));

    /*
    if(curr_task == prev_task) {
      //stack[510] = rip_loc;
      //child_task->stack_p = child_task->kern_stack = (uint64_t) &stack[510];
      //child_task->kern_stack = child_task->kern_stack - (curr_task->kern_stack - stack_loc);
      //child_task->stack_p = child_task->kern_stack;
      //uint64_t * temp = (uint64_t *) child_task->kern_stack;
      //temp[0] = rip_loc;
      //kprintf("In Parent!\n");
      //schedule();

      kprintf("In Parent!\n");
      child_task->kern_stack = child_task->kern_stack - 16 - 56 - 8;
      *(uint64_t *) child_task->kern_stack = rip_loc;
      child_task->stack_p = child_task->kern_stack;
      schedule();
    }
    else {
      kprintf("In Child!\n");
      outb(0x20, 0x20);
      return 0;
    }
    */

    //child_task->kern_stack = child_task->kern_stack - (curr_task->kern_stack - stack_loc);
    //child_task->kern_stack = child_task->kern_stack - 16 - 128 - 8;

    child_task->kern_stack = child_task->kern_stack - 16 - 64 - 8;
    *(uint64_t *) child_task->kern_stack = (uint64_t) fork_return;
    child_task->stack_p = child_task->kern_stack;
    schedule();

    return child_task->pid;
}

uint64_t sys_execve(char *fname, char **argv, char **envp)
{
    //TODO: change the prints to include process name.
    task_struct_t *new_task = (task_struct_t *) kmalloc(sizeof(task_struct_t *));
    if(!new_task) {
        kpanic("Not able to allocate task struct for new task\n");
        return -1;
    }
    memset(new_task, 0, sizeof(task_struct_t *));

    uint64_t * stack = (uint64_t *) kmalloc(PAGE_SIZE);
    if(!stack) {
        kpanic("Not able to allocate stack for init\n");
    }

    mm_struct_t *mm = (mm_struct_t *) kmalloc(PAGE_SIZE);
    if(!mm) {
        kpanic("Not able to allocate mm_struct for init\n");
    }

    char args[5][50]; //TODO: can this be less restrictive. Can use better version of kmalloc()
    int arg_cnt = 1;
    strcpy(args[0], fname);

    for(int i = 0; i < 5; i++) {
        //args[i][0] = '\0';
        if(argv[i+1] != NULL) {
            strcpy(args[i], argv[i+1]);
            arg_cnt++;
        }
    }

    new_task->state     = TASK_RUNNABLE;
    new_task->mm        = mm;
    new_task->sleep_t   = 0;
    new_task->pid       = get_pid();
    new_task->ppid      = -1;
    new_task->prev_task = NULL;
    new_task->parent    = NULL;
    new_task->sibling   = NULL;
    new_task->child     = NULL;
    strcpy(new_task->cdir_name, "/bin");
    strcpy(new_task->pcmd_name, fname);

    //TODO: setup user address space.
    struct page_map_level_4* old_pml4 = (struct page_map_level_4*) read_cr3();
    struct page_map_level_4* new_pml4 = (struct page_map_level_4*) kmalloc(PAGE_SIZE);

    old_pml4 = (struct page_map_level_4 *)((uint64_t) old_pml4 + KERNAL_BASE_ADDRESS); //TODO: use pa_to_va

    new_pml4->pml4e[511] = old_pml4->pml4e[511];

    new_task->pml4 = (uint64_t) new_pml4 - KERNAL_BASE_ADDRESS;

    write_cr3(new_task->pml4);

    new_task->kern_stack = (uint64_t) &stack[510];

    //Load process.
    int ret = load_elf(new_task, args[0]);
    //TODO: print the name of the exec
    if(ret == 0)
        kprintf("Loading Exe Sucessfull\n");
    else
        kprintf("Error Loading Exe\n");

    //TODO: copy arguments.TODO: do for envp too.
    uint64_t args_user = (uint64_t) kmalloc(PAGE_SIZE);
    uint64_t falign_addr = align_down(STACK_TOP);
    uint64_t page_addr = args_user - KERNAL_BASE_ADDRESS; //TODO: wirte va_to_pa();
    map_proc(page_addr, falign_addr);

    args_user = STACK_TOP + 0x1000 - 0x10 - sizeof(args);
    memcpy((void *) args_user, (void *) args, sizeof(args));
    for(int i = arg_cnt; i > 0; i--)
       *(uint64_t *)(args_user - 8*i) = args_user + (arg_cnt-i)*64;
    *(uint64_t *)(args_user - 8*(arg_cnt+1)) = arg_cnt-1;

    args_user = args_user - 8*(arg_cnt+1);
    new_task->stack_p = args_user;

    write_cr3((uint64_t)old_pml4 - KERNAL_BASE_ADDRESS);

    new_task->next_task = curr_task;
    curr_task = new_task;

    switch_to_userspace(new_task);

    return -1;
}
uint64_t sys_read(uint64_t fd, uint64_t addr, uint64_t size)
{
    if(fd == STDIN) {
        char *buf = (char *) addr;
        buf = "Hello";
        kprintf(buf);
    }
    while(1);

    return 1;
}

uint64_t sys_write(uint64_t fd, uint64_t addr, uint64_t size)
{
    if(fd == STDOUT || fd == STDERR) {
        //kprintf((char *) addr);
        char *sym = (char *) addr;
        pchar(*sym);
    }
    return 1;
}

uint64_t sys_waitpid(uint64_t pid, uint64_t status, uint64_t options)
{
    curr_task->state = TASK_WAITING;

    return 0;
}

uint64_t sys_getdents(uint64_t fd, char *dir, uint64_t size)
{
    //TODO:
    return 0;
}

uint64_t sys_dup2(uint64_t old_fd, uint64_t new_fd)
{
    //TODO:
    return 0;
}

uint64_t sys_pipe(uint64_t* fds)
{
    //TODO:
    return 0;
}

uint64_t sys_getcwd(char *buf, uint64_t size)
{
    //TODO:
    return 0;
}

uint64_t sys_access(char * pathname, uint64_t mode)
{
    //TODO:
    return 0;
}

uint64_t sys_chdir(char * pathname)
{
    //TODO:
    return 0;
}

uint64_t sys_open(char * pathname, uint64_t flags)
{
    //TODO:
    return 0;
}

uint64_t sys_close(uint64_t fd)
{
    //TODO:
    return 0;
}

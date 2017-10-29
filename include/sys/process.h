#ifndef _PROCESS_H
#define _PROCESS_H

#include <sys/defs.h>
#include <sys/fs.h>
#include <sys/mm_types.h>

#define MAX_NAME      150
#define MAX_FILES      50
#define MAX_PROCESS    50
#define MAX_PID      (4 * 1024 * 1024)

enum state {
    TASK_RUNNING  = 1,
    TASK_RUNNABLE = 2,
    TASK_BLOCKED  = 3,
    TASK_SLEEPING = 4,
    TASK_WAITING  = 5,
    TASK_ZOMBIE   = 6
};

struct task_struct {
    int pid;
    int state;
    int exit_code;
    
    struct mm_struct *mm;
    struct file_d *fd[MAX_FILES];

    struct task_struct *next_task, *prev_task;
    struct task_struct *next_run, *prev_run;
    struct task_struct *parent, *sibling, *child;

    uint64_t stack_p;
    uint64_t sleep_t;

    char pcmd_name[MAX_NAME];
    char cdir_name[MAX_NAME];
};

typedef struct task_struct task_struct_t;

#endif

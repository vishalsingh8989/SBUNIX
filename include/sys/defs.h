#ifndef _DEFS_H
#define _DEFS_H

#define NULL ((void*)0)

#define O_RDONLY	0x0000
#define O_WRONLY	0x0001
#define O_RDWR		0x0002
#define O_ACCMODE	0x0003

#define R_OK  4               /* Test for read permission.  */
#define W_OK  2               /* Test for write permission.  */
#define X_OK  1               /* Test for execute permission.  */
#define F_OK  0               /* Test for existence.  */

#define O_DIRECTORY	0100000	/* must be a directory */

#define __NR_read        0
#define __NR_write       1
#define __NR_open        2
#define __NR_close       3
#define __NR_pipe       22
#define __NR_dup2       33
#define __NR_fork       57
#define __NR_execve     59
#define __NR_exit       60
#define __NR_wait4      61
#define __NR_getcwd     79
#define __NR_chdir      80
#define __NR_getdents   78
#define __NR_symlinkat  266 //symlink for . and ..
typedef unsigned long  uint64_t;
typedef          long   int64_t;
typedef unsigned int   uint32_t;
typedef          int    int32_t;
typedef unsigned short uint16_t;
typedef          short  int16_t;
typedef unsigned char   uint8_t;
typedef          char    int8_t;

typedef uint64_t size_t;
typedef int64_t ssize_t;

typedef uint64_t off_t;

typedef uint32_t pid_t;

typedef int bool;

typedef struct env_var{
    char *key;
    char *value;
    struct env_var *next;
    struct env_var *prev;
} env_var;


typedef struct  {
    size_t size;
    struct block_info *next;
    struct block_info *prev;
    bool  state; // 0 = not free/ not available  and 1 = free/available
} block_info;

#define PAGESIZE 4096
#endif

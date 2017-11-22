#ifndef _FS_H
#define _FS_H

#include <sys/defs.h>

#define STDIN  0
#define STDOUT 1
#define STDERR 2

//TODO: define appropriate types.

typedef struct file_node {
    //struct file_operations *f_op;

    uint64_t    f_count;
    uint64_t    f_flags;
    off_t       f_pos;

    //uint64_t    f_size;
    //uint64_t    f_pgoff;
    //uint64_t    f_start;
    //uint64_t    f_bss_size;

    void *private_data;
} file_node_t;

typedef struct file_d {
    file_node_t* fnode;
} fd_t;

#endif

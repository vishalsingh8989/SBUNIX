#ifdef  _FS_H
#define _FS_H

#include <sys/defs.h>

//TODO: define appropriate types.
struct file_operations {
    uint64_t (*llseek) (struct file *, off_t, int);
    ssize_t (*read) (struct file *, char *, size_t, off_t *);
    ssize_t (*writed) (struct file *, const char *, size_t, off_t *);
}

typedef struct file_node {
    struct file_operations *f_op;

    uint64_t    f_count;
    uint64_t    f_flags;
    off_t       f_pos;

    //uint64_t    f_size;
    //uint64_t    f_pgoff;
    //uint64_t    f_start;
    //uint64_t    f_bss_size;

    void *private_data;
} file_node_t;

#endif

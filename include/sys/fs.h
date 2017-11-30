#ifndef _FS_H
#define _FS_H

#include <sys/defs.h>

#define STDIN  0
#define STDOUT 1
#define STDERR 2

//TODO: define appropriate types.

typedef struct  {
	uint32_t fmode;
	uint32_t fseek;
	unsigned short flags;
	unsigned short fsize;
	unsigned long freada, framax, fraend, fralen, frawin;
	struct file *fnext, *fprev;
	int fowner;         /* pid or -pgrp where SIGIO should be sent */
	//struct inode * f_inode;
	//struct file_operations * f_op;
	//unsigned long f_version;
	void *private_data;  /* needed for tty driver, and maybe others */
} file_node_t;

typedef struct file_d {
    file_node_t* fnode;
} fd_t;

#endif

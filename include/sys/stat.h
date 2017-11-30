#ifndef _STAT_H
#define _STAT_H

#include <sys/defs.h>
typedef struct stat {
	uint32_t			st_mode;
	uint32_t			st_ino;
	uint32_t			st_dev;
	uint32_t			st_rdev;
	uint32_t			st_nlink;
	uint32_t			st_uid;
	uint32_t			st_gid;
	uint32_t			st_size;
	//struct timespec	st_atim;
	//struct timespec	st_mtim;
	//struct timespec st_ctim;
	uint32_t		st_blksize;
	uint32_t		st_blocks;
}fstat_t;

#endif

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

#define MSR_EFER 0xC0000080 

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

#endif

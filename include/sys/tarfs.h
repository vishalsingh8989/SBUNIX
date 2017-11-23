#ifndef _TARFS_H
#define _TARFS_H

#include <sys/defs.h>

extern char _binary_tarfs_start;
extern char _binary_tarfs_end;

struct posix_header_ustar {
  char name[100];
  char mode[8];
  char uid[8];
  char gid[8];
  char size[12];
  char mtime[12];
  char checksum[8];
  char typeflag[1];
  char linkname[100];
  char magic[6];
  char version[2];
  char uname[32];
  char gname[32];
  char devmajor[8];
  char devminor[8];
  char prefix[155];
  char pad[12];
};

#define	 OPEN_FILE_LIMIT		512

#define TMAGIC   "ustar"        /* ustar and a null */
#define TMAGLEN  6
#define TVERSION "00"           /* 00 and no null */
#define TVERSLEN 2

/* Values used in typeflag field.  */
#define REGTYPE  '0'            /* regular file */
#define AREGTYPE '\0'           /* regular file */
#define LNKTYPE  '1'            /* link */
#define SYMTYPE  '2'            /* reserved */
#define CHRTYPE  '3'            /* character special */
#define BLKTYPE  '4'            /* block special */
#define DIRTYPE  '5'            /* directory */
#define FIFOTYPE '6'            /* FIFO special */
#define CONTTYPE '7'            /* reserved */

//https://www.gnu.org/software/tar/manual/html_node/Standard.html
struct tarfs_fd {
    char name[100];
    //char match_source[20];
    int type;
    int mode;
    uint64_t size;
    uint64_t fd_index;
    uint64_t offset;
    void *data;
};

typedef struct posix_header_ustar posix_header_ustar;

struct tarfs_fd tarfs_fds[OPEN_FILE_LIMIT];

void init_tarfs();
void *get_bin_info(const char *fname);
int syscall_open(const char *fname ,  int flag);
uint32_t get_index_by_name(const char* fname);
uint32_t get_child(uint32_t fd_idx , uint32_t child_fidx);

#endif

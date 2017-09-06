#ifndef _DIRENT_H
#define _DIRENT_H

#define NAME_MAX 255

struct dirent {
   long  inode;
   long  offset;
   unsigned short reclen;
   char d_name[NAME_MAX+1];
};

//struct DIR {
//   struct dirent ent;
//};

typedef struct DIR DIR;

DIR *opendir(const char *name);
struct dirent *readdir(DIR *dirp);
int closedir(DIR *dirp);
int getdents(int fd, char *dir, int size);

#endif

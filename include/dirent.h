#ifndef _DIRENT_H
#define _DIRENT_H

#define NAME_MAX 512

struct dirent {
	int type;
	int size;
	long  inode;
	long  offset;
	unsigned short len;
	char f_owner[30];
	char d_name[NAME_MAX+1];
};

//struct DIR {
//   struct dirent ent;
//};

typedef struct DIR DIR;

DIR *opendir(const char *name);
struct dirent *readdir(DIR *dirp);
int closedir(DIR *dirp);
int getdents(int fd, struct dirent *dir, int size);

#endif

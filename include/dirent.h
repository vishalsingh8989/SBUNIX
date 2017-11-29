#ifndef _DIRENT_H
#define _DIRENT_H

#define NAME_MAX 512

//ERROR CODES
#define ENOTDIR -1


struct dirent {
	int type;
	int size;
	long  inode;
	long  offset;
	unsigned short len;
	char fowner[30];
	char d_name[NAME_MAX+1];
};

typedef struct dir {
    int dfd;
    struct dirent *ptr_dir;
} DIR;



DIR *opendir(char *name);
struct dirent *readdir(DIR *dirp);
int closedir(DIR *dirp);
int getdents(int fd, struct dirent *dir, int size);

#endif

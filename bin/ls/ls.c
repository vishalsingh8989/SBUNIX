#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/defs.h>
#include <sys/stat.h>
#include <sys/user.h>

char *file_type = "-lscbdfc";
int main(int argc, char* argv[], char* envp[]) {

//	printf("************In ls argv*************\n");
//
//
//	for(int i=0 ;i < argc ; i++){
//		printf("%s at %p\n", argv[i], &argv[i]);
//
//	}
//	printf("************In ls argv************* %d \n", argc);



	char buff[NAME_MAX+1];
	memset(buff, '\0', NAME_MAX+1);
	if(argc == 1){
		getcwd(buff, NAME_MAX+1);
	}else if(argc == 2){
		strcpy(buff, argv[1]);
	}else{
		printf("Multiple directories not supported\n");
	}
	int fidx = open(buff, 0);
	struct dirent dir_buff;
	struct dirent* ptr_dir = &dir_buff;
	ptr_dir->offset=fidx;
	memset(dir_buff.d_name, '\0', sizeof(dir_buff.d_name));

	//fstat_t curr_dir_stat;
	//fstat_t par_dir_stat;
	//stat(fidx, &curr_dir_stat);
	//stat(fidx, &curr_dir_stat);
	//printf("size : %d\n", curr_dir_stat.st_size);
	//printf("drwx--x--x    %4dKB     %s  %s\n", file_type[dir_buff.type], dir_buff.size/1024,dir_buff.f_owner, ptr_dir->d_name);

	uint32_t size = 0;
	while(getdents(fidx, ptr_dir, 100) !=-1 && fidx != -1){
		printf("%crwx--x--x    %4dKB     %s  %s\n", file_type[dir_buff.type], dir_buff.size/1024,dir_buff.f_owner, ptr_dir->d_name);
		size = size + dir_buff.size;
		memset(dir_buff.d_name, '\0', sizeof(dir_buff.d_name));

	}
	if(fidx != -1){
	printf("drwx--x--x    %4dKB     %s  %s\n", size/1024,"admin", ".");
	printf("drwx--x--x    %4dKB     %s  %s\n", 0,"admin", "..");
	}else{
		printf("ls: %s: No such file or directory,\n", argv[1]);
	}



}

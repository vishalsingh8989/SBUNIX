#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/defs.h>
#include <sys/user.h>

char *file_type = "-lscbdfc";
int main(int argc, char* argv[], char* envp[]) {

	printf("************In ls argv*************\n");


	for(int i=0 ;i < argc ; i++){
		printf("%s at %p\n", argv[i], &argv[i]);

	}
	printf("************In ls argv************* %d \n", argc);



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

	printf("\n");
	while(getdents(fidx, ptr_dir, 100) !=-1){
		printf("%crwx--x--x    %4dKB     %s  ", file_type[dir_buff.type], dir_buff.size/1024,dir_buff.f_owner);
		printf("%s\n",ptr_dir->d_name);

		memset(dir_buff.d_name, '\0', sizeof(dir_buff.d_name));

	}

}

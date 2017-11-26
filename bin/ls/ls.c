#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/defs.h>

char *file_type = "-lscbdfc";
int main(int argc, char* argv[], char* envp[]) {

	printf("************In ls argv*************\n");

	for(int i=0 ;i < argc ; i++){
		puts(argv[i]);
		puts("\n");
	}
	printf("************In ls argv************* %d\n", argc);



	char buff[NAME_MAX+1];
	memset(buff, '\0', NAME_MAX+1);
	if(argc == 1){
		getcwd(buff, NAME_MAX+1);
	}else if(argc == 2){
		strcpy(buff, argv[1]);
	}else{
		puts("Multiple directories not supported\n");
	}
	//puts(buff);
	int fidx = open(buff, 0);
	struct dirent dir_buff ;
	struct dirent* ptr_dir = &dir_buff;
	ptr_dir->offset=fidx;
	memset(dir_buff.d_name, '\0', sizeof(dir_buff.d_name));
	while(getdents(fidx, ptr_dir, 100) !=-1){

		printf("%crwx--x--x    %5dKB ", file_type[dir_buff.type], dir_buff.size/1024);
		printf("%s\n",ptr_dir->d_name);

		//puts("\n");
		memset(dir_buff.d_name, '\0', sizeof(dir_buff.d_name));

	}

}

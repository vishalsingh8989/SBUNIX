#include <sys/tarfs.h>
#include <sys/kprintf.h>
#include <sys/defs.h>
#include <sys/utils.h>
#include <sys/elf64.h>
#include <sys/syscall.h>
#include <sys/utils.h>
#include <sys/string.h>
#include <sys/vmm.h>
#include <sys/kprintf.h>
#include <sys/user.h>
#include <sys/env.h>
#include <dirent.h>
#include<logger.h>


extern char PWD[MAX_NAME+1];
extern char PS1[MAX_NAME+1];
extern char HOME[MAX_NAME+1];
extern char USER[MAX_NAME+1];
extern char PATH[MAX_NAME+1];

void test_tarfs_init(int upper){

	for(int iterator = 0 ;iterator < upper ; iterator++ ){
		klog(BOOTLOG,"Name :  %s, size : %p ,  data  : %p\n" ,tarfs_fds[iterator].size, tarfs_fds[iterator].data);

	}
}

void *align_tarfs(void *p_val, uint64_t size)
{
    char *p_char_val = (char *) p_val;
    p_char_val = p_char_val - 1 - (((uint64_t) p_char_val - 1) % size) + size;
    return p_char_val;
}

void init_tarfs(){
	klog(BOOTLOG,"Tarfs Initialize  : %p - %p\n", &_binary_tarfs_start , &_binary_tarfs_end);
	klog(BOOTLOG,"Set root  : / \n");
	strcpy(PWD, "/");
	posix_header_ustar *iterator = (posix_header_ustar *) &_binary_tarfs_start;

	// store root
	*tarfs_fds[0].name = '/'; //string
	tarfs_fds[0].size = 0;
	tarfs_fds[0].offset = 0;
	tarfs_fds[0].data = 0;
	tarfs_fds[0].type = DIRTYPE;

	uint64_t fd_index = 1;



	file_node_t* fd_node = (file_node_t *)kmalloc(PAGE_SIZE);

	for(iterator =(posix_header_ustar *) &_binary_tarfs_start; iterator < ( posix_header_ustar *)&_binary_tarfs_end ;){
		uint64_t name_length = strlen(iterator->name);
		if(name_length){
			uint64_t size = otod(atoi(iterator->size));
			char tempname[NAME_MAX+1];
			memset(tempname, '\0', sizeof(tempname));
			strcpy(tempname,"/");
			strconcat(tempname, iterator->name);
			klog(BOOTLOG,"name=%s Size=%p\n", iterator->name, size);
			memset(tarfs_fds[fd_index].name, '\0', sizeof(tarfs_fds[fd_index].name));
			strcpy(tarfs_fds[fd_index].name, tempname);
			tarfs_fds[fd_index].size = size;
			tarfs_fds[fd_index].offset = 0;// read offset
			tarfs_fds[fd_index].mode = 0; // for flags

			fd_node->fowner = admin_uid;
			tarfs_fds[fd_index].fnode = fd_node;
			fd_node++;
			if (size) {//file if size not 0
				tarfs_fds[fd_index].type = REGTYPE;
				tarfs_fds[fd_index].data =  (void *) ((uint64_t) iterator + sizeof(posix_header_ustar));
				size  = (uint64_t)align_tarfs((void*) size ,  sizeof(posix_header_ustar)); // store in 4k
				iterator = (posix_header_ustar *) ((uint64_t) iterator + size);
				tarfs_fds[fd_index].size = size;
			}else{
				tarfs_fds[fd_index].data = 0; // dir have 0 data.//TODO add child data size
				tarfs_fds[fd_index].type = DIRTYPE;
			}
			fd_index++;
	}
		iterator++;

	}

	//testing


	strcpy(tarfs_fds[fd_index].name, "/bin/sbin/");
	tarfs_fds[fd_index].size = 0;
	tarfs_fds[fd_index].offset = 0;
	tarfs_fds[fd_index].data = 0;
	tarfs_fds[fd_index].type = DIRTYPE;


	strcpy(tarfs_fds[fd_index+1].name, "/bin/config/");
	tarfs_fds[fd_index+1].size = 0;
	tarfs_fds[fd_index+1].offset = 0;
	tarfs_fds[fd_index+1].data = 0;
	tarfs_fds[fd_index+1].type = DIRTYPE;


	strcpy(tarfs_fds[fd_index+2].name, "/bin/config/ifconfig");
	tarfs_fds[fd_index+2].size = 18883;
	tarfs_fds[fd_index+2].offset = 0;
	tarfs_fds[fd_index+2].data = 0;
	tarfs_fds[fd_index+2].type = REGTYPE;

	strcpy(tarfs_fds[fd_index+3].name, "/etc/sbin/config/ifconfig");
	tarfs_fds[fd_index+3].size = 30270;
	tarfs_fds[fd_index+3].offset = 0;
	tarfs_fds[fd_index+3].data = 0;
	tarfs_fds[fd_index+3].type = DIRTYPE;

	fd_index = fd_index+4;


	// if(BOOTLOG)
	// 	test_tarfs_init(fd_index);


	klog(BOOTLOG,"Tarfs Initialize :  Successful.\n");



}

uint32_t get_index_by_name(const char* fname){
	uint64_t idx = 0;
	for(idx = 0 ;idx <  OPEN_FILE_LIMIT ; idx++ ){
		if(-1 != strstr(tarfs_fds[idx].name, (char*)fname)){
			klog(INFO,"Found : %s at idx %d\n",fname,  idx);
			return idx;
		}
	}
	//TODO. not found then search bin_tarfs_start to bin_tarfs_end and store in tarfs_fd.
	return -1;
}

void* get_bin_info(const char *fname){


	uint64_t idx =  get_index_by_name(fname);

	if(idx !=  -1){
		return &tarfs_fds[idx];
	}
	return NULL;

}


int syscall_open(const char *fname ,  int flag){
	//TODO respect flags
	uint64_t idx = get_index_by_name(fname);
	if(idx != -1){
		tarfs_fds[idx].offset = 0;
		tarfs_fds[idx].mode = flag; // TODO
	}
	return idx;
}

int syscall_close(uint64_t fd){
	if(fd <  OPEN_FILE_LIMIT  && tarfs_fds[fd].name){
		tarfs_fds[fd].offset = 0;//reset
		tarfs_fds[fd].mode = 0;

	}
	return 0;
}

uint32_t get_child(uint32_t fd_idx , uint32_t child_fidx){

	char name[MAX_NAME+1] ;
	char tempname[MAX_NAME+1] ;
	memset(name, '\0', sizeof(name));
	memset(tempname, '\0', sizeof(tempname));

	strcpy(name,tarfs_fds[fd_idx].name);

	for (uint32_t idx = child_fidx+1; idx <  100 ;idx ++){
		strcpy(tempname,tarfs_fds[idx].name);
		int position = dir_match(name, tempname);
		int clevel = file_level(tempname);
		int plevel = file_level(name);
		//int test = dir_match(tarfs_fds[child_fidx].name, tempname);
		if(position ==1 && ((plevel +1 == clevel && tarfs_fds[idx].type == DIRTYPE ) || (plevel == clevel && tarfs_fds[idx].type == REGTYPE)  )    ){
			//klog(INFO,"name : %s , position :%d\n",tempname, position );
			return idx;
		}
	}
	//klog(INFO,"Name is : %s\n", name);
	return -1;
}

int file_level(char *fname){
	int len = strlen(fname);
	int level = 0;
	for(int i = 0 ; i <  len ; i++){
		if(fname[i] == '/'){
			level++;
		}
	}
	return level;
}
int tarfs_size(int fidx){
	return tarfs_fds[fidx].size;
}

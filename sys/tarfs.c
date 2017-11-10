#include<sys/tarfs.h>
#include<sys/kprintf.h>
#include<sys/defs.h>
#include<sys/utils.h>


struct tarfs_fd tarfs_fds[OPEN_FILE_LIMIT];

void sleep(int s){
	for(int i = 0 ;i< s*900; i ++){
		for(int k = 0 ;  k <s*900;k++  );
	}
}

void test_tarfs_init(int upper){
	kprintf("Start tarfs test..\n");
	for(int iterator = 0 ;iterator < upper ; iterator++ ){
		kprintf("Name :  %s, size : %p ,  data  : %p\n" ,tarfs_fds[iterator].name,  tarfs_fds[iterator].size, tarfs_fds[iterator].data);
	}
	kprintf("End tarfs test..\n");
}

void *align_up(void *p_val, uint64_t size)
{
    char *p_char_val = (char *) p_val;
    p_char_val = p_char_val - 1 - (((uint64_t) p_char_val - 1) % size) + size;
    return p_char_val;
}

void init_tarfs(){
	kprintf("Init tarfs : %p - %p\n", &_binary_tarfs_start , &_binary_tarfs_end);


	posix_header_ustar *iterator = (posix_header_ustar *) &_binary_tarfs_start;

	// store root
	*tarfs_fds[0].name = '/'; //string
	tarfs_fds[0].size = 0;
	tarfs_fds[0].offset = 0;
	tarfs_fds[0].data = 0;
	tarfs_fds[0].type = DIRTYPE;

	uint64_t fd_index = 1;

	for(iterator =(posix_header_ustar *) &_binary_tarfs_start; iterator < ( posix_header_ustar *)&_binary_tarfs_end ;)
	{
		uint64_t name_length = strlen(iterator->name);
		if(name_length){
			uint64_t size = otoi(atoi(iterator->size));
			kprintf("name=%s size=%p\n", iterator->name, size);
			strcopy(tarfs_fds[fd_index].name, iterator->name);
			tarfs_fds[fd_index].size = size;
			tarfs_fds[fd_index].offset = 0;

			if (size) {
				tarfs_fds[fd_index].type = REGTYPE;
				tarfs_fds[fd_index].data =  (void *) ((uint64_t) iterator + sizeof(posix_header_ustar));
				size  = (uint64_t) align_up((void *) size, sizeof(posix_header_ustar)); // store in 4k
				iterator = (posix_header_ustar *) ((uint64_t) iterator + size);
			}else{
				tarfs_fds[fd_index].data = 0;
				tarfs_fds[fd_index].type = DIRTYPE;
			}
			fd_index++;
		}
		iterator++;

	}

	test_tarfs_init(fd_index);
	get_bin_addr("bin/ls");


}

void *get_bin_addr(const char *fname){
	uint64_t idx = 0;
	for(idx = 0 ;idx <  OPEN_FILE_LIMIT ; idx++ ){
		if(-1 != strstr(tarfs_fds[idx].name, (char*)fname)){
			kprintf("Found : %s at idx %d\n",fname,  idx);
		}
	}
	//TODO
	return NULL;



}




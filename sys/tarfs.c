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


	//struct posix_header_ustar *iterator ;
	for(iterator =(posix_header_ustar *) &_binary_tarfs_start; iterator < ( posix_header_ustar *)&_binary_tarfs_end ;)
	{
		uint64_t name_length = strlen(iterator->name);
		if(name_length){
			uint64_t size = otoi_kernel(atoi_kernel(iterator->size));
			kprintf("name=%s size=%p\n", iterator->name, size);
			strcopy(tarfs_fds[fd_index].name, iterator->name);
			tarfs_fds[fd_index].size = size;
			tarfs_fds[fd_index].offset = 0;

			if (size) {
				tarfs_fds[fd_index].type = REGTYPE;
				tarfs_fds[fd_index].data =  (void *) ((uint64_t) iterator + sizeof(posix_header_ustar));
				size  = (uint64_t) align_up((void *) size, sizeof(posix_header_ustar));
				iterator = (posix_header_ustar *) ((uint64_t) iterator + size);
			}else{
				tarfs_fds[fd_index].data = 0;
				tarfs_fds[fd_index].type = DIRTYPE;
			}
		}
		iterator++;
		fd_index++;

	}




//	struct posix_header_ustar *iteratorator = (struct posix_header_ustar *)&_binary_tarfs_start ;
//
//
//	kprintf("Val end %p\n", (struct posix_header_ustar *)&_binary_tarfs_end);
//	for(iteratorator =(struct posix_header_ustar *) &_binary_tarfs_start; iteratorator < (struct posix_header_ustar *)&_binary_tarfs_end ;){
//		uint64_t name_length = strlen(iteratorator->name);
//
//
//
//		if(name_length){
//			uint64_t fsize = otoi(atoi((char*)iteratorator->size));
//			kprintf("Name : %s, size %d\n", iteratorator->name , fsize);
//			sleep(20);
//			*tarfs_fds[idx].name = '/';
//
//			tarfs_fds[idx].offset = 0;
//			tarfs_fds[idx].data = 0;
//
//
//			if (fsize){
//
//				//iteratorator = (struct posix_header_ustar *)(iteratorator  + fsize);
//				tarfs_fds[idx].type = 0;
//				tarfs_fds[idx].size = 0;//alignPage (uint64_t addr);
//			}else{
//				tarfs_fds[idx].type = 5;
//				tarfs_fds[idx].size = 0;
//			}
//			idx++;
//		}
//		iteratorator++;
//		//kprintf("iteratorator :  %p ,  %p\n",iteratorator, (struct posix_header_ustar *)&_binary_tarfs_end);
//
//	}

//	while(strlen(iteratorator->name)){
//		uint64_t fsize = otoi(atoi((char*)iteratorator->size));
//		kprintf("%p , Name : %s, size %d\n", iteratorator,iteratorator->name , fsize);
//
//		strcopy(tarfs_fds[idx].name , iteratorator->name);
//		tarfs_fds[idx].size = fsize;
//		tarfs_fds[idx].offset = 0;
//		tarfs_fds[idx].data = 0;
//		tarfs_fds[idx].type = 5;
//		if (fsize){
//			iteratorator = (struct posix_header_ustar *)(iteratorator  + fsize);
//		}
//
//		iteratorator++;
//	}

}

#include<sys/tarfs.h>
#include<sys/kprintf.h>
#include<sys/defs.h>
#include<sys/utils.h>
#include<sys/elf64.h>
#include<sys/syscall.h>


struct tarfs_fd tarfs_fds[OPEN_FILE_LIMIT];

void sleep(int s){
	for(int i = 0 ;i< s*900; i ++){
		for(int k = 0 ;  k <s*900;k++  );
	}
}

void test_tarfs_init(int upper){

	for(int iterator = 0 ;iterator < upper ; iterator++ ){
		kprintf("Name :  %s, size : %p ,  data  : %p\n" ,tarfs_fds[iterator].name,  tarfs_fds[iterator].size, tarfs_fds[iterator].data);
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

	for(iterator =(posix_header_ustar *) &_binary_tarfs_start; iterator < ( posix_header_ustar *)&_binary_tarfs_end ;)
	{
		uint64_t name_length = strlen(iterator->name);
		if(name_length){
			uint64_t size = otoi(atoi(iterator->size));
			kprintf("name=%s size=%p\n", iterator->name, size);
			strcopy(tarfs_fds[fd_index].name, iterator->name);
			tarfs_fds[fd_index].size = size;
			tarfs_fds[fd_index].offset = 0;// read offset
			tarfs_fds[fd_index].mode = 0; // for flags

			if (size) {//file if size not 0
				tarfs_fds[fd_index].type = REGTYPE;
				tarfs_fds[fd_index].data =  (void *) ((uint64_t) iterator + sizeof(posix_header_ustar));
				size  = (uint64_t) align_up((void *) size, sizeof(posix_header_ustar)); // store in 4k
				iterator = (posix_header_ustar *) ((uint64_t) iterator + size);
			}else{
				tarfs_fds[fd_index].data = 0; // dir have 0 data.
				tarfs_fds[fd_index].type = DIRTYPE;
			}
			fd_index++;
		}
		iterator++;

	}

	kprintf("tarfs test start..........");
	test_tarfs_init(fd_index);
	struct tarfs_fd *bin_tarfs = (struct tarfs_fd*) get_bin_info("bin/ls");
	kprintf("Found ls: %s at idx %p\n",bin_tarfs->name,  bin_tarfs->data);
	int idx = get_index("cat");
	kprintf("Cat index  : %d\n", idx);
	idx  = syscall_open("sbush", 0);
	kprintf("Sbush index  : %d\n", idx);

	Elf64_Ehdr *e_hdr = (Elf64_Ehdr *) (bin_tarfs->data);
	kprintf("Elf64_Ehdr data  : %p\n", e_hdr);
	Elf64_Phdr *p_hdr = (Elf64_Phdr *) ((uint64_t)e_hdr + e_hdr->e_phoff);
	kprintf("Elf64_Phdr p_hdr  : %p\n", p_hdr);
	kprintf("********************************\n");

//	typedef struct {
//	  Elf64_Word    p_type;
//	  Elf64_Word    p_flags;
//	  Elf64_Off     p_offset;
//	  Elf64_Addr    p_vaddr;
//	  Elf64_Addr    p_paddr;
//	  Elf64_Xword   p_filesz;
//	  Elf64_Xword   p_memsz;
//	  Elf64_Xword   p_align;
//	} Elf64_Phdr
	kprintf("Elf64_Phdr p_type :  %d\n",p_hdr->p_type);
	kprintf("Elf64_Phdr p_flags :  %d\n",p_hdr->p_flags);
	kprintf("Elf64_Phdr p_offset :  %d\n",p_hdr->p_offset);
	kprintf("Elf64_Phdr p_vaddr :  %p\n",p_hdr->p_vaddr);
	kprintf("Elf64_Phdr p_paddr :  %p\n",p_hdr->p_paddr);
	kprintf("Elf64_Phdr p_filesz :  %p\n",p_hdr->p_filesz);
	kprintf("Elf64_Phdr p_memsz :  %p\n",p_hdr->p_memsz);
	kprintf("Elf64_Phdr p_align :  %p\n",p_hdr->p_align);


	kprintf("********************************\n");
	kprintf("tarfs test end ..........");
	kprintf("Open call\n");
	//sleep();


	//char   buf[2048+1];


	//getcwd(buf, (size_t) 2048+1);
	uint64_t out;
	out = syscall_0(48);
	kprintf("Open call res : %d \n", out);


}

int get_index(const char* fname){
	uint64_t idx = 0;
	for(idx = 0 ;idx <  OPEN_FILE_LIMIT ; idx++ ){
		if(-1 != strstr(tarfs_fds[idx].name, (char*)fname)){
			kprintf("Found : %s at idx %d\n",fname,  idx);
			return idx;
		}
	}
	//TODO. not found then search bin_tarfs_start to bin_tarfs_end and store in tarfs_fd.
	return -1;
}

void* get_bin_info(const char *fname){


	uint64_t idx =  get_index(fname);

	if(idx !=  -1){
		return &tarfs_fds[idx];
	}
	return NULL;
//	for(idx = 0 ;idx <  OPEN_FILE_LIMIT ; idx++ ){
//		if(-1 != strstr(tarfs_fds[idx].name, (char*)fname)){
//			kprintf("Found : %s at idx %d\n",fname,  idx);
//			return
//		}
//	}
	//TODO. not found then search bin_tarfs_start to bin_tarfs_end and store in tarfs_fd.


}

//int sys_call_open(const char* fname){
//	if(!fname) return -1;
//
//
//
//
//
//}

int syscall_open(const char *fname ,  int flag){
	//TODO respect flags
	uint64_t idx = get_index(fname);
	if(idx != -1){
		tarfs_fds[idx].offset = 0;
		tarfs_fds[idx].mode = flag; // TODO
	}
	return idx;
//	for(idx = 0 ;idx <  OPEN_FILE_LIMIT ; idx++ ){
//		if(-1 != strstr(tarfs_fds[idx].name, (char*)fname)){
//			kprintf("Found : %s at idx %d\n",fname,  idx);
//			return &tarfs_fds[idx];
//		}
//	}
}

int syscall_close(uint64_t fd){
	if(fd <  OPEN_FILE_LIMIT  && tarfs_fds[fd].name){
		tarfs_fds[fd].offset = 0;//reset
		tarfs_fds[fd].mode = 0;

	}
	return 0;

}




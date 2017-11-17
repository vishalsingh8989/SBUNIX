#include <sys/defs.h>
#include <sys/vmm.h>
#include <sys/process.h>
#include <sys/kprintf.h>
#include <sys/tarfs.h>
#include <sys/utils.h>
#include <dirent.h>

char cwd[500];



void sys_exit(){
    //TODO: cleanup

    task_struct_t* next_task = curr_task->next_task;

    kprintf("Context switch from %s to %s\n", curr_task->pcmd_name, next_task->pcmd_name);

    //context_switch(next_task, curr_task);
    schedule();
}

int sys_execve(char *fname, char **argv, char **envp){
   //TODO:
   return 0;
}


uint64_t sys_open(const char *fname, uint32_t flags){
	kprintf("syscall : sys_open()\n");
	int fidx = get_index_by_name(fname);
	if(fidx == -1){
	}else{
		kprintf("fname : %s ,  flags : %p, found at idx : %d\n", fname, flags, fidx);
	}
	return fidx;
}
uint64_t sys_getcwd(char *buf, uint64_t size){
	kprintf("Inside sys_getcwd\n");
	//strcopy(buf ,cwd);
	//kprintf("cwd: %s\n", cwd);
	kprintf("buf from getcwd : %s ,  size : %d\n", buf, size);
	return 0;
}

uint64_t sys_chdir(const char *dirname){
	int len = strlen(cwd);
//	TODO . handle absolute and relative paths.
	strcopy(cwd, dirname);
	//cwd = (char *)dirname;	cwd
	kprintf("Change dir :  %s, len: %d \n",cwd,len );
	return 0;
}


uint64_t sys_getdents(uint32_t fd_index, struct dirent* dir, uint32_t count){
	static uint32_t offset;
	if(offset == 0){
		offset = fd_index;
	}
	char buf[512] = {0};
	strcopy(buf ,cwd);

	uint32_t child_fidx = fd_index;
	child_fidx = get_child(fd_index, &child_fidx );

	if(child_fidx == -1){
		offset = 0;
		return -1;
	}


	strcopy(dir->d_name, tarfs_fds[child_fidx].name);// copy name
	kprintf("data copied to dir struct :  %s\n", dir->d_name);
	return child_fidx;

//
//	while ((child_fidx = get_child(idx, &child_fidx) )!=-1){
//		kprintf("name : %s\n",tarfs_fds[child_fidx].name , child_fidx);
//
//
//	}

}

int sys_fork(){
	kprintf("Inside sys_fork\n");

	return 19;
}

#include <sys/tarfs.h>
#include <sys/defs.h>
#include <sys/utils.h>
#include <sys/elf64.h>
#include <sys/syscall.h>
#include <sys/utils.h>
#include <sys/string.h>
#include <sys/vmm.h>
#include <sys/kprintf.h>
#include <sys/user.h>
#include <dirent.h>
#include<logger.h>


uint64_t curr_uid;

void init_users(){
	klog(BOOTLOG, "Intialize users.\n");
	users[0] = "jvishal";
	users[1] = "aahangir";
	users[2] = "admin";
	pchar_xy('U' , GREEN, 0, 24);
	pchar_xy('s' , GREEN, 1, 24);
	pchar_xy('e' , GREEN, 2, 24);
	pchar_xy('r' , GREEN, 3, 24);
	pchar_xy(':' , GREEN, 4, 24);


}

void set_uid(uint64_t user_id){
	klog(INFO,"Inside  set_uid :  %d ,  %s \n", user_id, users[user_id]);
	curr_uid =  user_id;
	for(int idx = 0 ; idx < strlen(users[user_id]); idx++)
		pchar_xy(users[user_id][idx] , RED, 6 + idx, 24);
}

uint64_t get_uid(){
	return curr_uid;
}

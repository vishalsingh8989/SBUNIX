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

extern string users[5];
void init_users(){
	users[0] = "admin";
	users[1] = "user1";
	//strcpy(users[0] , "admin");
	//strcpy(users[1] , "user1");
	//strcpy(users[2] , "user2");
}

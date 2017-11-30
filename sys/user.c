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

extern string users[5];
void init_users(){
	users[0] = "admin";
	users[1] = "user1";

	pchar_xy('U' , GREEN, 0, 24);
	pchar_xy('s' , GREEN, 1, 24);
	pchar_xy('e' , GREEN, 2, 24);
	pchar_xy('r' , GREEN, 3, 24);
	pchar_xy(':' , GREEN, 4, 24);
	for(int idx = 0 ; idx < strlen(users[0]); idx++){
		pchar_xy(users[0][idx] , GREEN, idx + 5, 24);
	}


//strcpy(users[0] , "admin");
	//strcpy(users[1] , "user1");
	//strcpy(users[2] , "user2");
}

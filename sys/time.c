#include<sys/time.h>
#include<sys/utils.h>
#include<sys/string.h>

char system_uptime[15];
void init_time(){
	strcpy(system_uptime, "00:00:00");
}
void set_system_uptime(char* timestr){

	strcpy(system_uptime, timestr);
}

void get_system_uptime(char *buff){
	strcpy(buff, system_uptime);
}

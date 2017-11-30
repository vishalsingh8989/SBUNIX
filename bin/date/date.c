#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>





char weekdayname[7][4] = {
    "Mon",
    "Tue",
	"Wed",
	"Thu",
	"Fri",
	"Sat",
	"Sun"
};

char monthname[13][5] = {
    "Jan",
    "Feb",
	"Mar",
	"Apr",
	"May",
	"Jun",
	"July",
	"Aug",
	"Oct",
	"Sept",
	"Nov",
	"Dec"
};


int main(int argc, char* argv[], char* envp[]) {

	struct tm tm_time;
	gettime(&tm_time);

	//printf("%d\n", tm_time.tm_wday);
	printf("%s, %s %d  %d :%d :%d,  UTC %d   \n",weekdayname[tm_time.tm_wday],monthname[tm_time.tm_mon-1],tm_time.tm_mday,tm_time.tm_hour,tm_time.tm_min,tm_time.tm_sec, tm_time.tm_year);


	return 0;
}

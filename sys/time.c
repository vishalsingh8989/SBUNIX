#include<sys/time.h>
#include<sys/utils.h>
#include<sys/string.h>
#include<sys/asm_utils.h>
#include<sys/kprintf.h>

#define CURRENT_YEAR        2017
char system_uptime[15];



int century_val = 0x00;

int second;
int minute;
int hour;
int day;
int weekday;
int month;
int year;


int updating() {
      outb(CMOS_ADD, 0x0A);
      return (inb(CMOS_DATA) & 0x80);
}

unsigned char get_RTC_values(int reg) {
      outb(CMOS_ADD, reg);
      return inb(CMOS_DATA);
}

void read_rtc(struct tm* utc) {
      unsigned char century = 0;

      unsigned char bcdmode;

      while (updating());
      second = get_RTC_values(0x00);
      minute = get_RTC_values(0x02);
      hour = get_RTC_values(0x04);
      day = get_RTC_values(0x07);
      weekday = get_RTC_values(0x06);
      month = get_RTC_values(0x08);
      year = get_RTC_values(0x09);
      if(century_val != 0) {
            century = get_RTC_values(century_val);
      }


      bcdmode = get_RTC_values(0x0B);


      if (!(bcdmode & 0x04)) {

            second = (second & 0x0F) + ((second / 16) * 10);
            minute = (minute & 0x0F) + ((minute / 16) * 10);
            hour = ( (hour & 0x0F) + (((hour & 0x70) / 16) * 10) ) | (hour & 0x80);
            day = (day & 0x0F) + ((day / 16) * 10);
            month = (month & 0x0F) + ((month / 16) * 10);
            year = (year & 0x0F) + ((year / 16) * 10);

            if(century_val != 0)
            {
                  century = (century & 0x0F) + ((century / 16) * 10);
            }
      }

      if (!(bcdmode & 0x02) && (hour & 0x80)) {
            hour = ((hour & 0x7F) + 12) % 24;
      }



      if(century_val != 0) {
            year += century * 100;
      } else {
            year += (CURRENT_YEAR / 100) * 100;
            if(year < CURRENT_YEAR) year += 100;
      }



		utc->tm_sec  = second;  //seconds;  // [0,61]
		utc->tm_min  = minute;//  minutes;  // [0,59]
		utc->tm_hour = hour;// hour;     // [0,23]
		utc->tm_mday = day;// day of month [1,31]
		utc->tm_mon  = month;// month of year [0,11]
		utc->tm_year = year;// years since 1900
		utc->tm_wday = weekday;// day of week [0,6] (Sunday = 0)
		//utc->tm_yday ;// day of year [0,365]
		//utc->tm_isdst;// daylight savings flag}
		//
		//kprintf("month : %d\n", month);

}

void init_time(){
	strcpy(system_uptime, "00:00:00");
}
void set_system_uptime(char* timestr){

	strcpy(system_uptime, timestr);
}

void get_system_uptime(char *buff){
	strcpy(buff, system_uptime);
}

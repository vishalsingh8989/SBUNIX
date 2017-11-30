#include<sys/time.h>
#include<sys/utils.h>
#include<sys/string.h>
#include<sys/asm_utils.h>
#include<sys/kprintf.h>

#define CURRENT_YEAR        2017
char system_uptime[15];


                      // Change this each year!

int century_register = 0x00;                                // Set by ACPI table parsing code if possible

int second;
int minute;
int hour;
int day;
int weekday;
int month;
int year;



int get_update_in_progress_flag() {
      outb(CMOS_ADD, 0x0A);
      return (inb(CMOS_DATA) & 0x80);
}

unsigned char get_RTC_register(int reg) {
      outb(CMOS_ADD, reg);
      return inb(CMOS_DATA);
}

void read_rtc(struct tm* utc) {
      unsigned char century = 0;
//      unsigned char last_second;
//      unsigned char last_minute;
//      unsigned char last_hour;
//      unsigned char last_day;
//      unsigned char last_month;
//      unsigned char last_year;
//      unsigned char last_century;
      unsigned char registerB;

      // Note: This uses the "read registers until you get the same values twice in a row" technique
      //       to avoid getting dodgy/inconsistent values due to RTC updates

      while (get_update_in_progress_flag());                // Make sure an update isn't in progress
      second = get_RTC_register(0x00);
      minute = get_RTC_register(0x02);
      hour = get_RTC_register(0x04);
      day = get_RTC_register(0x07);
      weekday = get_RTC_register(0x06);
      month = get_RTC_register(0x08);
      year = get_RTC_register(0x09);
      if(century_register != 0) {
            century = get_RTC_register(century_register);
      }

//      m
//      } while( (last_second != second) || (last_minute != minute) || (last_hour != hour) ||
//               (last_day != day) || (last_month != month) || (last_year != year) ||
//               (last_century != century) );

      registerB = get_RTC_register(0x0B);

      // Convert BCD to binary values if necessary

      if (!(registerB & 0x04)) {
            second = (second & 0x0F) + ((second / 16) * 10);
            minute = (minute & 0x0F) + ((minute / 16) * 10);
            hour = ( (hour & 0x0F) + (((hour & 0x70) / 16) * 10) ) | (hour & 0x80);
            day = (day & 0x0F) + ((day / 16) * 10);
            month = (month & 0x0F) + ((month / 16) * 10);
            year = (year & 0x0F) + ((year / 16) * 10);
            if(century_register != 0) {
                  century = (century & 0x0F) + ((century / 16) * 10);
            }
      }

      // Convert 12 hour clock to 24 hour clock if necessary

      if (!(registerB & 0x02) && (hour & 0x80)) {
            hour = ((hour & 0x7F) + 12) % 24;
      }

      // Calculate the full (4-digit) year

      if(century_register != 0) {
            year += century * 100;
      } else {
            year += (CURRENT_YEAR / 100) * 100;
            if(year < CURRENT_YEAR) year += 100;
      }


      //
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
		//kprintf("yearrtc : %d\n", year);

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

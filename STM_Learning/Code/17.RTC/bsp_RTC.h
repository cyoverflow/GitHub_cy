#ifndef __BSP_RTC_H__
#define __BSP_RTC_H__

#include <stm32f10x.h>
#include <stdio.h>

#define leapyear(a)			(((a % 4) && (a%100 != 0)) || (a % 400 == 0))
#define days_in_year(i) (leapyear(i) ? 366 : 365)

int RTC_Configuration(void);
void RTC_TimeDisplay(u32 TimeVal);
void Set_time(void);

struct rtc_tm
{
	unsigned int year;
	unsigned int month;
	unsigned int day;
	unsigned int hour;
	unsigned int minute;
	unsigned int second;
};



#endif

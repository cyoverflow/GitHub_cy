#include "bsp_RTC.h"

unsigned int days_in_month[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

//设定一个返回值，如果为1，表示已经设置过时间，无需重新设置， 如果为0，表示需要设置时间
int RTC_Configuration(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP | RCC_APB1Periph_PWR, ENABLE);
	PWR_BackupAccessCmd(ENABLE);
	
	if(BKP_ReadBackupRegister(BKP_DR1) != 0xABCD)
	{
		BKP_DeInit();
		RCC_LSEConfig(RCC_LSE_ON);
		while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) != SET);
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
		RCC_RTCCLKCmd(ENABLE);
		RTC_WaitForLastTask();
		RTC_WaitForSynchro();
		RTC_SetPrescaler(32767);
		RTC_WaitForLastTask();
		
		BKP_WriteBackupRegister(BKP_DR1, 0xABCD);
		return 0;
	}
	else
	{
		return 1;
	}
}

void RTC_TimeDisplay(u32 TimeVal)
{
	struct rtc_tm tm;
	unsigned int days, hms;
	unsigned int year_now;
	int month_now;
	
	days = TimeVal / 86400;
	hms = TimeVal % 86400;
	
	tm.hour = hms / 3600;
	tm.minute = hms % 3600 / 60;
	tm.second = hms % 3600 % 60;
	
	for(year_now = 1970; days >= days_in_year(year_now); year_now++)
	{
		days -= days_in_year(year_now);
	}
	tm.year = year_now;
	
	if(leapyear(year_now))
	{
		days_in_month[2] = 29;
	} else
	{
		days_in_month[2] = 28;
	}
	
	for(month_now = 1; days >= days_in_month[month_now]; month_now++)
	{
		days -= days_in_month[month_now];
	}
	tm.month = month_now;
	
	tm.day = days + 1;
	
	printf("%d/%d/%d, %d:%d:%d\n", tm.year, tm.month, tm.day, tm.hour, tm.minute, tm.second);
}

void Set_time(void)
{
	RTC_SetCounter(0x63D74648);
}





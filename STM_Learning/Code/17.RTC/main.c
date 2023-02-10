#include "Led_Key.h"
#include "bsp_exti.h"
#include "bsp_SysTick.h"
#include "bsp_IWDG.h"
#include "bsp_WWDG.h"
#include "bsp_UART.h"
#include "bsp_DMA.h"
#include "bsp_ADC.h"
#include "bsp_tim2.h"
#include "bsp_RTC.h"

int main(void)
{
	unsigned int flag;
	
	SysTick_Configuration();

	flag = RTC_Configuration();
	
	UART1_Configuration();
	
	if(flag == 0)//表示没有配置过RTC，需要配置时间
	{
		Set_time();
	}
	
	while(1)
	{
		RTC_TimeDisplay(RTC_GetCounter());
		Delay_us(1000000);
	} 
}

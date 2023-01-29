#include "bsp_IWDG.h"

//使能IWDG，在40KHz HSI下， 约等于每秒产生一次IWDG复位
void IWDG_Configuration(void)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);//使能对寄存器IWDG_PR和IWDG_RLR的写操作
	IWDG_SetPrescaler(IWDG_Prescaler_64);//设置IWDG预分频值
	IWDG_SetReload(625);//设置IWDG重装载值
	IWDG_ReloadCounter();//按照IWDG重装载寄存器的值重装载IWDG计数器
	IWDG_Enable();//使能IWDG
}

void IWDG_FeedDog(void)
{
	IWDG_ReloadCounter();
}




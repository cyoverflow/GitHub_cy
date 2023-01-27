#include "bsp_SysTick.h"

unsigned int TimingDelay;

void SysTick_Configuration(void)
{
	while(SysTick_Config(72));
	
	SysTick->CTRL &= ~(1<<0);  //暂时失能定时器，到使用前再进行使能操作
	
}

void Delay_us(unsigned int n)
{
	TimingDelay = n;
	
	SysTick->CTRL |= (1<<0);//使能定时器
	while(TimingDelay);
	SysTick->CTRL &= ~(1<<0);
}

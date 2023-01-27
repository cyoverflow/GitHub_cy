#include "Led_Key.h"
#include "bsp_exti.h"
#include "bsp_SysTick.h"

int main(void)
{
	SysTick_Configuration();
	
	LED_Configuration();//LED灯的初始化

	while(1)
	{
		GPIO_WriteBit(GPIOA, GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4, Bit_SET);
		//delay(0xFFFFFF);
		Delay_us(100000);
		GPIO_WriteBit(GPIOA, GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4, Bit_RESET);
		//delay(0xFFFFFF);
		Delay_us(100000);
	} 
	
	
}

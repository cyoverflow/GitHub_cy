#include "Led_Key.h"
#include "bsp_exti.h"
#include "bsp_SysTick.h"
#include "bsp_IWDG.h"
#include "bsp_WWDG.h"
#include "bsp_UART.h"

int main(void)
{
	SysTick_Configuration();
	
	LED_Configuration();
	
	PA5_EXTI5_Configuration();//通过PA5外部中断去唤醒待机模式
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);//使能PWR时钟
	
	while(1)
	{
		for(int i = 0; i < 10; i++)
		{
			GPIO_WriteBit(GPIOA, GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4, Bit_SET);
			Delay_us(100000);
			GPIO_WriteBit(GPIOA, GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4, Bit_RESET);
			Delay_us(100000);
		}
	} 
}

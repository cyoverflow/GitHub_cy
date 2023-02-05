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
	
	PA0_EXTI0_Configuration();//通过PA0外部中断去唤醒睡眠模式
	
	while(1)
	{
		for(int i = 0; i < 10; i++)
		{
			GPIO_WriteBit(GPIOA, GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4, Bit_SET);
			Delay_us(100000);
			GPIO_WriteBit(GPIOA, GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4, Bit_RESET);
			Delay_us(100000);
		}
		//进入停止模式
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);//使能PWR时钟
		PWR_EnterSTOPMode(PWR_Regulator_ON, PWR_STOPEntry_WFI);
	} 
}

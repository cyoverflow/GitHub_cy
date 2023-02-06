#include "Led_Key.h"
#include "bsp_exti.h"
#include "bsp_SysTick.h"
#include "bsp_IWDG.h"
#include "bsp_WWDG.h"
#include "bsp_UART.h"
#include "bsp_DMA.h"

int main(void)
{
	SysTick_Configuration();
	
	LED_Configuration();
	
	UART1_Configuration();
	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);//使能USART的DMA发送请求
	
	DMA_Configuration();//初始化DMA
	
	for(int i = 0; i < 2048; i++)
	{
		arr[i] = 'A';
	}
	while(1)
	{
		GPIO_WriteBit(GPIOA, GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4, Bit_SET);
		Delay_us(1000000);
		GPIO_WriteBit(GPIOA, GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4, Bit_RESET);
		Delay_us(1000000);
	} 
}

#include "Led_Key.h"

int main(void)
{
	int i;
	
	LED_Configuration();//LED灯的初始化
	

	while(1)
	{
		for(i = 0; i < 10000; i++)  //LED逐渐变亮
		{
			GPIO_WriteBit(GPIOA, GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4, Bit_RESET);
			delay(10000 - i);
			GPIO_WriteBit(GPIOA, GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4, Bit_SET);
			delay(i);
		}
		
		for(i = 0; i < 10000; i++)  //LED逐渐变暗
		{
			GPIO_WriteBit(GPIOA, GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4, Bit_RESET);
			delay(i);
			GPIO_WriteBit(GPIOA, GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4, Bit_SET);
			delay(10000 - i);
		}
	} 
}

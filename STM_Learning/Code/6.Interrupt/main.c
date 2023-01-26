#include "Led_Key.h"
#include "bsp_exti.h"

int main(void)
{
	LED_Configuration();//LED灯的初始化
	PA0_EXTI0_Configuration();

	while(1)
	{
		
	} 
	
	
}

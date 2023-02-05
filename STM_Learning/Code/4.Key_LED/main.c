#include "Led_Key.h"

int main(void)
{
	LED_Configuration();//LED灯的初始化
	Key_Configuration();//按键的初始化
	

	while(1){
		if(Key_One_Scan(GPIOA, GPIO_Pin_0) == KEY_PRESSED) {
			//如果按下按钮,则对LED状态进行切换
			GPIOA->ODR ^= GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
		} 
	}
}

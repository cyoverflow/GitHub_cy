#include "Led_Key.h"



void LED_Configuration(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能GPIOA外设时钟

	GPIO_InitTypeDef GPIO_Initstructure;
	GPIO_Initstructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Initstructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_Initstructure);//将PA1~PA4配置为通用推挽输出,工作频率50MHz
}

// PA0 -> KEY1  input  
void Key_Configuration(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能GPIOA外设时钟

	GPIO_InitTypeDef GPIO_Initstructure;
	GPIO_Initstructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_5;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Initstructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输入 使用模拟的方式为电路模拟接上一个上拉电阻  即按键没有按下去的时候持续输入高电平
	GPIO_Init(GPIOA, &GPIO_Initstructure);//将PA1~PA4配置为通用上拉输入,工作频率50MHz
}

// Key_Scan -> press = 0
int Key_One_Scan (GPIO_TypeDef* GPIOx, unsigned int Pin) {
	
	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0)
	{
		//延时并进行再次确认,达到软件消抖
		delay(1000);
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0)
		{
			while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0);
			return KEY_PRESSED;
		}
	}
	
	return 1;
}


void delay(unsigned long time) {
	while(time--) {
	}
}


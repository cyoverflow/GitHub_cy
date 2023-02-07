#include <stm32f10x.h>

//#define RCC_APB2ENR (*(volatile unsigned long *)0x40021018)
//#define GPIOA_CRL 	(*(volatile unsigned long *)0x40010800)
//#define GPIO_ODR		(*(volatile unsigned long *)0x4001080C)

void delay(unsigned long);

int main(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能GPIOA外设时钟

	GPIO_InitTypeDef GPIO_Initstructure;
	GPIO_Initstructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Initstructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_Initstructure);//将PA1~PA4配置为通用推挽输出,工作频率50MHz
	
	while(1){
		//GPIO_SetBits(GPIOA, GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4);
		GPIO_WriteBit(GPIOA, GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4, Bit_SET);//PA1~PA4输出高电平,点亮四个灯
		delay(0xFFFFFF);
		//GPIO_ResetBits(GPIOA, GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4);
		GPIO_WriteBit(GPIOA, GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4, Bit_RESET);//PA1~PA4输出低电平,熄灭四个灯
		delay(0xFFFFFF);
	}
}

void delay(unsigned long time) {
	while(time--) {
	}
}

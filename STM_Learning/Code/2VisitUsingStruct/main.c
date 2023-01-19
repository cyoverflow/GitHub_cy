#include "main.h"

//#define RCC_APB2ENR (*(volatile unsigned long *)0x40021018)
//#define GPIOA_CRL 	(*(volatile unsigned long *)0x40010800)
//#define GPIO_ODR		(*(volatile unsigned long *)0x4001080C)

void delay(unsigned long);

int main(void)
{
	//RCC_APB2ENR = 0x4002 1000 + 0x18
	//RCC_APB2ENR bit[2] = 1 使能GPIOA外设时钟
	//RCC_APB2ENR |= (1<<2);
	
	RCC->APB2ENR |= (1<<2);
	
	//GPIOA_CRL &= 0xFFF0000F;GPIOA_CRL = 0x4001 0800 + 0x00,  利用与运算清除[19:4]
	//GPIOA_CRL |= 0x00033330;//利用或运算将[19:4]置为0011 0011 0011 0011 将PA1~PA4配置为通用推挽输出,工作频率50MHz
	GPIOA->CRL &= 0xFFF0000F;
	GPIOA->CRL |= 0x00033330;
	
	//GPIO_ODR = 0x40010800 + 0x0C = 0x4001080C
	//GPIO_ODR &= ~((1<<1) | (1<<2) | (1<<3) | (1<<4));//PA1~PA4输出低电平,LED全灭  或写成 &= 0xFFFFFFE1 
	GPIOA->ODR &= ~((1<<1) | (1<<2) | (1<<3) | (1<<4));
	
	while(1){
		//点亮LED1~LED4  PA1~PA4输出高电平
		GPIOA->ODR |= (1<<1) | (1<<2) | (1<<3) | (1<<4);//点亮四个灯
		delay(0xFFFF);
		//熄灭LED1~LED4  PA1~PA4输出低电平
		GPIOA->ODR &= ~((1<<1) | (1<<2) | (1<<3) | (1<<4));//熄灭四个灯
		delay(0xFFFF);
	}
}

void delay(unsigned long time) {
	while(time--) {
	}
}
#include "bsp_exti.h"

void PA0_EXTI0_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_Initstructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO,ENABLE);//使能GPIOA外设时钟和功能复用IO时钟
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;//外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; //响应优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能NVIC来管理外部中断0(EXTI0)
	
	NVIC_Init(&NVIC_InitStructure);//初始化外设NVIC寄存器
	
	//将PA0配置成外部中断的方式
	GPIO_Initstructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Initstructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输入 使用模拟的方式为电路模拟接上一个上拉电阻  即按键没有按下去的时候持续输入高电平
	GPIO_Init(GPIOA, &GPIO_Initstructure);//将PA1~PA4配置为通用上拉输入,工作频率50MHz
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);//选择GPIO管脚用作外部中断线路

	EXTI_InitStructure.EXTI_Line = EXTI_Line0; //外部线路
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; //被使能线路的模式:中断
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发: 高电平变为低电平时触发
	EXTI_InitStructure.EXTI_LineCmd = ENABLE; //线路的新状态
	EXTI_Init(&EXTI_InitStructure);//初始化外设EXTI寄存器
}

void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0) == SET)
	{
		GPIOA->ODR ^= GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
		EXTI_ClearITPendingBit(EXTI_Line0);//相应业务完成后要清除对应的中断挂起位
	}
}

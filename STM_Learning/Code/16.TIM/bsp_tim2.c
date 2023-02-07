#include "bsp_tim2.h"

unsigned int times;

void TIM2_Configuration()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	//初始化NVIC
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	//初始化TIM2
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Period = 9999;//自动重装载寄存器周期的值 9999 + 1 = 10000  100us x 10000 = 1s 
	TIM_TimeBaseStructure.TIM_Prescaler = 7199;//预分频值               7199 + 1 = 7200   100us
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;//时钟分割			意义不大
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//计数器模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);//清除TIM2的更新标志位
	
	TIM_ITConfig(TIM2, TIM_FLAG_Update, ENABLE);//中断使能
	TIM_Cmd(TIM2, ENABLE);//使能
	
	//暂时关闭TIM2时钟,全部初始化结束后正式使用前再开启
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, DISABLE);
}

void TIM2_IRQHandler()
{
	if(TIM_GetITStatus(TIM2, TIM_FLAG_Update) == SET)
	{
		times++;
		if(times % 2 == 1)
		{
			printf("LED shined %dtimes.\n",times/2 + 1);
		}
		GPIOA->ODR ^= GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
		TIM_ClearFlag(TIM2, TIM_FLAG_Update);//清除中断标志位
	}
}


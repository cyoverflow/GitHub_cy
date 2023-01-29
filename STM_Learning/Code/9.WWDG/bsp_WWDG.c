#include "bsp_WWDG.h"

unsigned char WWDG_CNT = 0x7F;

void WWDG_NVIC_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel = WWDG_IRQn;//外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; //响应优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能NVIC来管理外部中断0(EXTI0)
	
	NVIC_Init(&NVIC_InitStructure);//初始化外设NVIC寄存器
}

void WWDG_Configuration(unsigned char tr, unsigned char wr, unsigned int pr)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);//使能WWDG时钟
	
	WWDG_SetPrescaler(pr);//设置预分频值
	WWDG_SetWindowValue(wr);//设置上窗口值(0x40与0x7F之间)
	WWDG_CNT &= tr;//初始值(0x40~0x7F区间的值,与操作避免超过范围)
	WWDG_Enable(WWDG_CNT);//设置WWDG计数器值,并使能WWDG
	
	WWDG_NVIC_Init();
	
	WWDG_ClearFlag();//清除早期唤醒中断标志位
	WWDG_EnableIT();//使能WWDG早期唤醒中断（EWI）
}

void WWDG_IRQHandler(void)
{
	WWDG_SetCounter(WWDG_CNT);
	WWDG_ClearFlag();
}

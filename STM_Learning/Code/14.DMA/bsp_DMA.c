#include "bsp_DMA.h"

volatile unsigned char arr[2048];

void DMA_Configuration(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);//使能DMA外设时钟
	
	DMA_InitStructure.DMA_PeripheralBaseAddr = USART_DR; //外设地址,此处为UART发送缓冲区地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)arr;//内存地址,此处为数组的地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;	 //将外设作为DMA数据传输的终点
	DMA_InitStructure.DMA_BufferSize = 2048;						 //DMA缓存大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设寄存器地址是否递增
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;					//内存寄存器地址是否递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//数据的单位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;									//DMA是否循环
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;						//DMA通道优先级
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;										//DMA是否从内存到内存传输
	
	DMA_Init(DMA1_Channel4, &DMA_InitStructure);
	DMA_Cmd(DMA1_Channel4, ENABLE);
	
}





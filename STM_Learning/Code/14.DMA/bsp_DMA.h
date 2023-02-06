#ifndef __BSP_DMA_H__
#define __BSP_DMA_H__

#include <stm32f10x.h>

#define USART_DR 0x40013804

extern volatile unsigned char arr[2048];
void DMA_Configuration(void);

#endif

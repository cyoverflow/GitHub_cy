#ifndef __BSP_WWDG_H__
#define __BSP_WWDG_H__

#include <stm32f10x.h>

void WWDG_Configuration(unsigned char tr, unsigned char wr, unsigned int pr);
void WWDG_IRQHandler(void);

#endif

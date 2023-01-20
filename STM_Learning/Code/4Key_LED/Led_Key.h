#ifndef __LED_KEY_H__
#define __LED_KEY_H__

#include <stm32f10x.h>

#define KEY_PRESSED 0

void delay(unsigned long);
void LED_Configuration(void);
void Key_Configuration(void);
int Key_One_Scan (GPIO_TypeDef*, unsigned int);


#endif

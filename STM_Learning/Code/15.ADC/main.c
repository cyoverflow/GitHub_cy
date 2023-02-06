#include "Led_Key.h"
#include "bsp_exti.h"
#include "bsp_SysTick.h"
#include "bsp_IWDG.h"
#include "bsp_WWDG.h"
#include "bsp_UART.h"
#include "bsp_DMA.h"
#include "bsp_ADC.h"

int main(void)
{
	float temperature;
	
	SysTick_Configuration();

	UART1_Configuration();
	ADC_Configuration();
	
	
	while(1)
	{
		//温度(°C) = {(V25 - VSENSE) / Avg_Slope} + 25
		temperature = (V25 - ADC_ConvertVal) / AVG_SLOPE + 25;
		Delay_us(1000000);
		printf("The temperature now is:%.1f.\n",temperature);
	} 
}

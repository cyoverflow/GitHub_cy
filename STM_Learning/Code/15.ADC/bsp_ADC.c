#include "bsp_ADC.h"

volatile uint16_t ADC_ConvertVal;

//对DMA,ADC进行初始化
void ADC_Configuration(void)
{
	//使能外设时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	
	//配置DMA
	DMA_InitTypeDef DMA_InitStructure;
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC_DR; //外设地址,此处为ADC_DR地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADC_ConvertVal;//内存地址,此处为变量的地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;	 //将外设作为DMA数据传输的来源
	DMA_InitStructure.DMA_BufferSize = 1;						 //DMA缓存大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设寄存器地址是否递增
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;					//内存寄存器地址是否递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//数据的单位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;									//DMA是否循环
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;						//DMA通道优先级
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;										//DMA是否从内存到内存传输
	
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	DMA_Cmd(DMA1_Channel1, ENABLE);
	
	//配置ADC
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1, &ADC_InitStructure);
	
	//总转换时间 = 采样时间 + 12.5个周期  现在ADCCLK=9MHz,采样时间为55.5周期  1/9 x 68周期 = 7.56微秒
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);//设置ADC时钟频率
	ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 1, ADC_SampleTime_55Cycles5);//设置转化顺序和采样时间
	
	ADC_TempSensorVrefintCmd(ENABLE);//使能温度传感器
	
	ADC_DMACmd(ADC1, ENABLE);//使能ADC的DMA
	ADC_Cmd(ADC1, ENABLE);//使能ADC
	
	ADC_ResetCalibration(ADC1);//重置ADC的校准寄存器
	while(ADC_GetResetCalibrationStatus(ADC1) != RESET);//等待校准寄存器复位
	ADC_StartCalibration(ADC1);//对ADC进行校准
	while(ADC_GetCalibrationStatus(ADC2) != RESET);//等待校准完成
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);//使能ADC的软件转换启动功能
}

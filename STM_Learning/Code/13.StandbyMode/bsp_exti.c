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

void PA5_EXTI5_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_Initstructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO,ENABLE);//使能GPIOA外设时钟和功能复用IO时钟
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;//外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; //响应优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能NVIC来管理外部中断0(EXTI0)
	
	NVIC_Init(&NVIC_InitStructure);//初始化外设NVIC寄存器
	
	//将PA5配置成外部中断的方式
	GPIO_Initstructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Initstructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输入 使用模拟的方式为电路模拟接上一个上拉电阻  即按键没有按下去的时候持续输入高电平
	GPIO_Init(GPIOA, &GPIO_Initstructure);//将PA1~PA4配置为通用上拉输入,工作频率50MHz
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource5);//选择GPIO管脚用作外部中断线路

	EXTI_InitStructure.EXTI_Line = EXTI_Line5; //外部线路
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; //被使能线路的模式:中断
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发: 高电平变为低电平时触发
	EXTI_InitStructure.EXTI_LineCmd = ENABLE; //线路的新状态
	EXTI_Init(&EXTI_InitStructure);//初始化外设EXTI寄存器
}

static void Set_SYSCLOCK_TO_72(void)
{
  __IO uint32_t StartUpCounter = 0, HSEStatus = 0;
  
  /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration ---------------------------*/    
  /* Enable HSE */    
  RCC->CR |= ((uint32_t)RCC_CR_HSEON);
 
  /* Wait till HSE is ready and if Time out is reached exit */
  do
  {
    HSEStatus = RCC->CR & RCC_CR_HSERDY;
    StartUpCounter++;  
  } while((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

  if ((RCC->CR & RCC_CR_HSERDY) != RESET)
  {
    HSEStatus = (uint32_t)0x01;
  }
  else
  {
    HSEStatus = (uint32_t)0x00;
  }  

  if (HSEStatus == (uint32_t)0x01)
  {
    /* Enable Prefetch Buffer */
    FLASH->ACR |= FLASH_ACR_PRFTBE;

    /* Flash 2 wait state */
    FLASH->ACR &= (uint32_t)((uint32_t)~FLASH_ACR_LATENCY);
    FLASH->ACR |= (uint32_t)FLASH_ACR_LATENCY_2;    

 
    /* HCLK = SYSCLK */
    RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;
      
    /* PCLK2 = HCLK */
    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV1;
    
    /* PCLK1 = HCLK */
    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV2;

#ifdef STM32F10X_CL
    /* Configure PLLs ------------------------------------------------------*/
    /* PLL2 configuration: PLL2CLK = (HSE / 5) * 8 = 40 MHz */
    /* PREDIV1 configuration: PREDIV1CLK = PLL2 / 5 = 8 MHz */
        
    RCC->CFGR2 &= (uint32_t)~(RCC_CFGR2_PREDIV2 | RCC_CFGR2_PLL2MUL |
                              RCC_CFGR2_PREDIV1 | RCC_CFGR2_PREDIV1SRC);
    RCC->CFGR2 |= (uint32_t)(RCC_CFGR2_PREDIV2_DIV5 | RCC_CFGR2_PLL2MUL8 |
                             RCC_CFGR2_PREDIV1SRC_PLL2 | RCC_CFGR2_PREDIV1_DIV5);
  
    /* Enable PLL2 */
    RCC->CR |= RCC_CR_PLL2ON;
    /* Wait till PLL2 is ready */
    while((RCC->CR & RCC_CR_PLL2RDY) == 0)
    {
    }
    
   
    /* PLL configuration: PLLCLK = PREDIV1 * 9 = 72 MHz */ 
    RCC->CFGR &= (uint32_t)~(RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLSRC | RCC_CFGR_PLLMULL);
    RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLXTPRE_PREDIV1 | RCC_CFGR_PLLSRC_PREDIV1 | 
                            RCC_CFGR_PLLMULL9); 
#else    
    /*  PLL configuration: PLLCLK = HSE * 9 = 72 MHz */
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE |
                                        RCC_CFGR_PLLMULL));
    RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC_HSE | RCC_CFGR_PLLMULL9);
#endif /* STM32F10X_CL */

    /* Enable PLL */
    RCC->CR |= RCC_CR_PLLON;

    /* Wait till PLL is ready */
    while((RCC->CR & RCC_CR_PLLRDY) == 0)
    {
    }
    
    /* Select PLL as system clock source */
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
    RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;    

    /* Wait till PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)0x08)
    {
    }
  }
  else
  { /* If HSE fails to start-up, the application will have wrong clock 
         configuration. User can add here some code to deal with this error */
  }
}

void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0) == SET)
	{
		//GPIOA->ODR ^= GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
		Set_SYSCLOCK_TO_72();
		EXTI_ClearITPendingBit(EXTI_Line0);//相应业务完成后要清除对应的中断挂起位
	}
}

void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line5) == SET)
	{
		PWR_WakeUpPinCmd(ENABLE);//启用WKUP管脚功能
		PWR_EnterSTANDBYMode();
		EXTI_ClearITPendingBit(EXTI_Line5);//相应业务完成后要清除对应的中断挂起位
	}
}

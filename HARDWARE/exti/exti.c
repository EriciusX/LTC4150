#include "exti.h"

/*******************************************************************************
*函数的原型：void EXTI7_Init(void)
*函数的功能：INT脚中断初始化
*函数的参数：None
*函数返回值：None
*函数的说明：
*函数编写者: 豪猪
*函数编写日期：2021/7/21
*函数的版本号：V1.0
********************************************************************************/
void EXTI7_Init(void)
{
	
 	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	battery_init();
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    //GPIOB.7 中断线以及中断初始化配置
	
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource7);

  	EXTI_InitStructure.EXTI_Line=EXTI_Line7;
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		//下降沿触发
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	    					//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
		
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//使能按键所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//抢占优先级1
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//子优先级1
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure); 
	
}


/*******************************************************************************
*函数的原型：void EXTI9_5_IRQHandler(void)
*函数的功能：INT脚中断函数
*函数的参数：None
*函数返回值：None
*函数的说明：
*函数编写者: 豪猪
*函数编写日期：2021/7/21
*函数的版本号：V1.0
********************************************************************************/
void EXTI9_5_IRQHandler(void)
{
	if(INT_in == 0)
	{
		myISR();
	}
	EXTI_ClearITPendingBit(EXTI_Line7); //清除中断标志位  
}


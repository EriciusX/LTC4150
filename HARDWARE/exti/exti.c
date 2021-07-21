#include "exti.h"

/*******************************************************************************
*������ԭ�ͣ�void EXTI7_Init(void)
*�����Ĺ��ܣ�INT���жϳ�ʼ��
*�����Ĳ�����None
*��������ֵ��None
*������˵����
*������д��: ����
*������д���ڣ�2021/7/21
*�����İ汾�ţ�V1.0
********************************************************************************/
void EXTI7_Init(void)
{
	
 	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	battery_init();
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    //GPIOB.7 �ж����Լ��жϳ�ʼ������
	
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource7);

  	EXTI_InitStructure.EXTI_Line=EXTI_Line7;
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		//�½��ش���
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	    					//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
		
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//��ռ���ȼ�1
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//�����ȼ�1
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure); 
	
}


/*******************************************************************************
*������ԭ�ͣ�void EXTI9_5_IRQHandler(void)
*�����Ĺ��ܣ�INT���жϺ���
*�����Ĳ�����None
*��������ֵ��None
*������˵����
*������д��: ����
*������д���ڣ�2021/7/21
*�����İ汾�ţ�V1.0
********************************************************************************/
void EXTI9_5_IRQHandler(void)
{
	if(INT_in == 0)
	{
		myISR();
	}
	EXTI_ClearITPendingBit(EXTI_Line7); //����жϱ�־λ  
}


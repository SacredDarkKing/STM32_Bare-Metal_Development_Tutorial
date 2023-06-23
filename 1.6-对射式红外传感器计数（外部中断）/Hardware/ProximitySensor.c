#include "stm32f10x.h"

uint16_t proximitySensor_Count = 0;

void ProximitySensor_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	//开启APB2的AFIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	//开启EXTI和NVIC的时钟，但是因为EXTI不需要开启，NVIC则是默认开启的，所以这里不需要手动开启
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	//配置EXTI线路
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource12);
	
	//配置EXIT线路
	EXTI_InitTypeDef EXTI_InitStruct;
	EXTI_InitStruct.EXTI_Line = EXTI_Line12;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStruct);
	
	//配置NVIC
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置分组规则
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStruct);
	
}
uint16_t get_proximitySensor_Count(void)
{
	return proximitySensor_Count;
}


void EXTI15_10_IRQHandler(void)
{

	if (EXTI_GetITStatus(EXTI_Line12) == SET)
	{
		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12) == 0)
			proximitySensor_Count++;
		EXTI_ClearITPendingBit(EXTI_Line12);
	}
}


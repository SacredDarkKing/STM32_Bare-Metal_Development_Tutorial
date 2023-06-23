#include "stm32f10x.h"                  // Device header

void LED_Init(void) 
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	GPIO_SetBits(GPIOA, GPIO_Pin_2 | GPIO_Pin_3);
}

void LED2_On()
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_2);
}

void LED2_Off()
{
	GPIO_SetBits(GPIOA, GPIO_Pin_2);
}

void LED2_Turn()
{
	if (GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_2) == 0)
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_2);
	} else 
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_2);
	}
	
}

void LED3_On()
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_3);
}

void LED3_Off()
{
	GPIO_SetBits(GPIOA, GPIO_Pin_3);
}

void LED3_Turn()
{
	if (GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_3) == 0)
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_3);
	} else 
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_3);
	}
	
}

#include "stm32f10x.h"                  // Device header
#include "Delay.h"

int main(void)
{
	//1. 使用RCC开启GPIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	//2. 使用GPIO_init函数初始化GPIO
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	//3. 使用输出或者输入函数控制GPIO口
	
	int portValue = 0x0001;
	
	while (1)
	{
		GPIO_Write(GPIOA, ~portValue);
		portValue <<= 1;
		Delay_ms(500);
		if (portValue == 0x0080)
		{
			portValue = 0x0001;
		}
	}
}

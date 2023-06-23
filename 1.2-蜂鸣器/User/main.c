#include "stm32f10x.h"                  // Device header
#include "Delay.h"

int main(void)
{
	//1. 使用RCC开启GPIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	//2. 使用GPIO_init函数初始化GPIO
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	//3. 使用输出或者输入函数控制GPIO口
	
	//GPIO_ResetBits(GPIOB, GPIO_Pin_12);
	
	while (1)
	{
		GPIO_Write(GPIOB, ~0x1000);
		Delay_ms(50);
		
		GPIO_Write(GPIOB, 0x1000);
		Delay_ms(50);
		
		GPIO_Write(GPIOB, ~0x1000);
		Delay_ms(50);
		
		GPIO_Write(GPIOB, 0x1000);
		Delay_ms(3000);

	}
}

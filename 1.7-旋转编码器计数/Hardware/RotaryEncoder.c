#include "stm32f10x.h"
#include "Delay.h"

int16_t rotaryEncoder_Count = 0;

void RotaryEncoder_Init(void)
{
    // 开启时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    // 初始化引脚，上拉输入
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    // 配置GPIO与EXTI线路关联
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource15);

    // 配置EXTI
    EXTI_InitTypeDef EXTI_InitStruct;
    EXTI_InitStruct.EXTI_Line = EXTI_Line14 | EXTI_Line15;
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_Init(&EXTI_InitStruct);

    // 配置NVIC
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStruct);
}

int16_t get_RotaryEncoder_Count()
{
    return rotaryEncoder_Count;
}

void EXTI15_10_IRQHandler(void)
{
	
	//旋转编码器的技术不稳定，原因未知
	
    if (EXTI_GetITStatus(EXTI_Line14) == SET)
    {
        if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14) == 0 && GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15) == 1)
        {
            rotaryEncoder_Count++;
            Delay_ms(20);
        }
        EXTI_ClearITPendingBit(EXTI_Line14);
    }

    if (EXTI_GetITStatus(EXTI_Line15) == SET)
    {
        if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14) == 1 && GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15) == 0)
        {
            rotaryEncoder_Count--;
            Delay_ms(20);
        }
        EXTI_ClearITPendingBit(EXTI_Line15);
    }
}

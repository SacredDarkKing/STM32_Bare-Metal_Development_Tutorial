#include "stm32f10x.h"

void PWM_Init(void)
{
    // RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	// RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    // GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE);
    // GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//

	// GPIO_InitTypeDef GPIO_InitStruct;
	// GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	// GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15;
	// GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	// GPIO_Init(GPIOA, &GPIO_InitStruct);

    // TIM_InternalClockConfig(TIM2);

    // TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    // TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    // TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    // TIM_TimeBaseInitStruct.TIM_Period = 100 - 1;
    // TIM_TimeBaseInitStruct.TIM_Prescaler = 720 - 1;
    // TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
    // TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);

    // TIM_OCInitTypeDef TIM_OCInitStruct;
    // TIM_OCStructInit(&TIM_OCInitStruct);
    // TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;//OC模式
    // TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;//OC极性，high就是不反转，low就是反转
    // TIM_OCInitStruct.TIM_Pulse = 50;//CCR寄存器的值
    // TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;//使能
    // TIM_OC1Init(TIM2, &TIM_OCInitStruct);
    
    // TIM_Cmd(TIM2, ENABLE);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    TIM_InternalClockConfig(TIM2);

    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStruct.TIM_Period = 100 - 1;
    TIM_TimeBaseInitStruct.TIM_Prescaler = 720 - 1;//频率1000hz
    TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);

    TIM_OCInitTypeDef TIM_OCInitStruct;
    TIM_OCStructInit(&TIM_OCInitStruct);
    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStruct.TIM_Pulse = 50;
    TIM_OC1Init(TIM2, &TIM_OCInitStruct);

    TIM_Cmd(TIM2, ENABLE);

}

void PWM_SetCompare1(uint16_t pulse) 
{
    TIM_SetCompare1(TIM2, pulse);
}

void PWM_SetFrequence(uint16_t freq)
{
    TIM_PrescalerConfig(TIM2, 720000 / freq - 1, TIM_PSCReloadMode_Immediate);
}


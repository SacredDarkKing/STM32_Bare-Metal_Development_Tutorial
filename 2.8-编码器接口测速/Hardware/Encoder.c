#include "stm32f10x.h"

void Encoder_Init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);


    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);


    TIM_ICInitTypeDef TIM_ICInitStruct;
    TIM_ICStructInit(&TIM_ICInitStruct);
    TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;//通道参数是不能按位或（"|"）组合的
    TIM_ICInitStruct.TIM_ICFilter = 0xf;
    TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;//此处的rising和平时的含义不同，这里rising代表的是不反转，falling是反转
    TIM_ICInit(TIM3, &TIM_ICInitStruct);

    TIM_ICInitStruct.TIM_Channel = TIM_Channel_2;//通道参数是不能按位或（"|"）组合的
    TIM_ICInitStruct.TIM_ICFilter = 0xf;
    TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM_ICInit(TIM3, &TIM_ICInitStruct);

    TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);

    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;//在编码器接口的方式中，计数模式没有作用
    TIM_TimeBaseInitStruct.TIM_Period = 65536 - 1;
    TIM_TimeBaseInitStruct.TIM_Prescaler = 0;//不需要分屏
    TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);

    TIM_Cmd(TIM3, ENABLE);

}

int16_t Encoder_GetCNT(void)
{
    return TIM_GetCounter(TIM3);
}

#include "stm32f10x.h"
#include "PWM.h"

void IC_Init(void)
{

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    TIM_InternalClockConfig(TIM3);

    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStruct.TIM_CounterMode  = TIM_CounterMode_Up;
    TIM_TimeBaseInitStruct.TIM_Period = 65536 - 1;
    TIM_TimeBaseInitStruct.TIM_Prescaler = 72;
    TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);


    TIM_ICInitTypeDef TIM_ICInitStruct;
    TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;
    TIM_ICInitStruct.TIM_ICFilter = 0xF;
    TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInit(TIM3, &TIM_ICInitStruct);

    //设置从模式自动重置cnt
    TIM_SelectInputTrigger(TIM3, TIM_TS_TI1FP1);//选择从模式的触发源

    TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset);//选择从模式

    TIM_Cmd(TIM3, ENABLE);
}

uint16_t IC_GetFreqence(void)
{
    return 1000000 / (TIM_GetCapture1(TIM3) + 1);
}

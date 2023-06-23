#include "stm32f10x.h"

uint16_t Timer_Cnt = 0;

void Timer_init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);


    TIM_InternalClockConfig(TIM2);

    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//滤波器的采样频率
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;//计数器的计数模式
    TIM_TimeBaseInitStruct.TIM_Period = 10000 - 1;//周期，也就是ACC自动重装器的值
    TIM_TimeBaseInitStruct.TIM_Prescaler = 7200 - 1;//预分频值
    TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;//重复计数器的值，高级定时器的属性，如果用的不是高级定时器，随便给个值就好

    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
    //因为预分频器有一个缓冲寄存器，需要在下一格更新事件时才会更改预分频器的值
    //所以在TIM_TimeBaseInit中，调用了一次更新事件，而更新时间又会将更新标志置位1
    //解决办法就是，在初始化完时基单元后，并且在初始化NVIC前，手动将这个标志位置为0，就可以避免一上电就产生更新的问题
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    

    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStruct);

    TIM_Cmd(TIM2, ENABLE);

}

uint16_t get_Timer_Cnt()
{
    return Timer_Cnt;
}


void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
    {
        Timer_Cnt++;
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}


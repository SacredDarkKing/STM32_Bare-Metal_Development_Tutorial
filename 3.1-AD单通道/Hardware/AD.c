#include "stm32f10x.h"

void AD_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

    RCC_ADCCLKConfig(RCC_PCLK2_Div6);//设置adc的分频，这里是6分频，这样adc的频率就是72/6=12了

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;//当使用adc输入中，为了防止外部对gpio引脚造成干扰，gpio引脚会失效，必须定义为AIN模式才可以ADC输入
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;//pa2是adc1和2的2号通道
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    //初始化adc
    ADC_InitTypeDef ADC_InitStruct;
    ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;//adc寄存器的数据对其，左对齐或者右对齐
    ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//外部触发的方式，此示例使用软件触发，这里选择none
    ADC_InitStruct.ADC_Mode = ADC_Mode_Independent ;//adc工作模式，独立模式或者是双adc模式，双adc模式又有许多选择
    ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;//连续转换模式：连续转换/单词转换，指的是一次抓换结束后，是否自动进行下一次转换，还是手动执行下一次转换
    //连续转换模式下，只需要第一次开启转换，之后，不需要在开启转换
    //单词模式则每次都需要开启转换
    ADC_InitStruct.ADC_ScanConvMode = DISABLE;//扫描模式：单通道/多通道，指的是规则组转换的通道数目，数目是ADC_NbrOfChannel，扫描就是规则组中依次都进行转换，非扫描就是只转换第一项
    ADC_InitStruct.ADC_NbrOfChannel = 1;//指定扫描模式转换的通道数目
    ADC_Init(ADC1, &ADC_InitStruct);

    //规则通道配置，这个函数用来配置 规则组，配置规则表中的Rank位置位ADC_Channel通道、采样周期位ADC_SampleTime
    //注意：规则表一共可以存放16项，一个ADC_RegularChannelConfig函数只能配置规则表中的 一项，如果需要配置更多项，就继续使用该函数进行配置
    //一个通道只能测量一个输入
    //adc_channel需要参照引脚定义表，看看自己使用的是哪个通道
    ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 1, ADC_SampleTime_1Cycles5);

    ADC_Cmd(ADC1, ENABLE);//此处需要开启adc才能进行校准

    //校准过程：复位搅浑-等待复位校准完成-校准-等待校准完成
    ADC_ResetCalibration(ADC1);
    while (ADC_GetResetCalibrationStatus(ADC1) == SET);//返回CR2_RSTCAL_Set标志位，set是正在校准，reset是校准完成，使用while等待校准
    ADC_StartCalibration(ADC1);
    while (ADC_GetCalibrationStatus(ADC1) == SET);

}

uint16_t AD_GetValue(void)
{
    //此处配置的是单通道，单词转换的模式，所以需要软件触发转换
    //开始转换一次
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);

    //等待转换完成
    while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);//EOC是0的时候是未完成
    return ADC_GetConversionValue(ADC1);//返沪ADC_DR的值，之后并清空EOC标志位，所以这里不需要手动清除标志位
}

#include "stm32f10x.h"

uint16_t MyDMA_Size;

void MyDMA_Init(uint32_t addr1, uint32_t addr2, uint16_t size)
{

    MyDMA_Size = size;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    DMA_InitTypeDef DMA_InitStruct;
    DMA_StructInit(&DMA_InitStruct);
    DMA_InitStruct.DMA_PeripheralBaseAddr = addr1;
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
    DMA_InitStruct.DMA_MemoryBaseAddr = addr2;
    DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStruct.DMA_BufferSize = size;//转运次数
    DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;//方向
    DMA_InitStruct.DMA_M2M = DMA_M2M_Enable;//m2m就是软件触发 否则就是硬件出发
    DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;//是否自动重装
    DMA_InitStruct.DMA_Priority = DMA_Priority_Medium;//优先级
    DMA_Init(DMA1_Channel1, &DMA_InitStruct);//这里第一个参数需要注意：要指定dma和它的通道，硬件触发需要对应规定的通道，而软件触发不需要

}

void MyDMA_Transfer(void)
{
    // 转运函数

    // 关闭 DMA1 通道1，停止数据传输，因为转运前必须先失能dma
    DMA_Cmd(DMA1_Channel1, DISABLE);

    // 设置 DMA1 通道1 的当前数据传输计数器为 MyDMA_Size，重新设置传输数据的大小
    DMA_SetCurrDataCounter(DMA1_Channel1, MyDMA_Size);

    // 使能 DMA1 通道1，启动数据传输
    DMA_Cmd(DMA1_Channel1, ENABLE);

    // 等待 DMA1 通道1 的传输完成标志被置位，即数据传输完成
    while (DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);

    // 清除 DMA1 通道1 的传输完成标志位，重置标志位以便下一次传输，该标志需要手动清除
    DMA_ClearFlag(DMA1_FLAG_TC1);
}


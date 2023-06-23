#include "stm32f10x.h"
#include <stdio.h>
#include <stdarg.h>

// 用来接收发送或者接收的数据包，固定长度为4字节
uint8_t Serial_RxPacket[4];
uint8_t Serial_TxPacket[4];
uint8_t Serial_RxFlag;

void Serial_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct); // gpioa9是发送口，配置输出模式

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
    GPIO_Init(GPIOA, &GPIO_InitStruct); // gpioa10是接收口，配置输入模式

    USART_InitTypeDef USART_InitStruct;
    USART_InitStruct.USART_BaudRate = 9600;                                      // 波特率，这里直接写，stm32会自动计算分配系数
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 硬件流控制，保证硬件能够正确的接收数据
    // 例如有时候因为硬件繁忙，导致输入不能接收，而发送端一直在发送，所以这部分数据就浪费了，如果开启硬件流控制，就会有一个握手的过程，表示硬件准备好了可以接收
    USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; // usart的模式，输入或者输出，如果又要输入又要输出，就用或符号链接
    USART_InitStruct.USART_Parity = USART_Parity_No;             // 校验位，奇校验、偶校验
    USART_InitStruct.USART_StopBits = USART_StopBits_1;          // 停止位长度
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;     // 数据字长
    USART_Init(USART1, &USART_InitStruct);

    // 接收有两种形式，一种是查询、一种是中断，如果是查询，则初始化就是到此为止
    // 如何查询是否收到数据：判断标志为RXNE是否为1，1说明收到数据

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStruct);

    USART_Cmd(USART1, ENABLE);
}

void Serial_SendByte(uint8_t msg)
{
    USART_SendData(USART1, msg);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
        ;
}

void Serial_SendArray(uint8_t *array, uint16_t len)
{
    for (int i = 0; i < len; i++)
    {
        Serial_SendByte(array[i]);
    }
}

void Serial_SendString(char *string)
{
    for (int i = 0; string[i] != 0; i++)
    {
        Serial_SendByte(string[i]);
    }
}

void Serial_SendNumber(uint32_t number)
{
    // 此处递归是为了先发送最高位
    if (number == 0)
        return;
    int8_t n = number % 10;
    Serial_SendNumber(number / 10);
    Serial_SendByte('0' + n);
}

int fputc(int ch, FILE *f) // printf重定向，缺点：只有一个串口可以使用重定向
{
    Serial_SendByte(ch);
    return ch;
}

void Serial_Printf(char *format, ...) // 使用sprintf，解决了上一个函数的缺点，将输出结果存储在一个string中，再由串口发送出去，此处是c语言的可变参数知识点
{
    char string[100];
    va_list arg;
    va_start(arg, format);
    vsprintf(string, format, arg);
    va_end(arg);
    Serial_SendString(string);
}

void Serial_SendPacket(void)
{

    // 发送hex格式的数据包，固定长度为4字节，包头是ff，包尾是fe
    Serial_SendByte(0xff);
    Serial_SendArray(Serial_TxPacket, 4);
    Serial_SendByte(0xfe);
}

uint8_t Serial_GetRxFlag(void)
{
    uint8_t flag = Serial_RxFlag == 1 ? 1 : 0;
    Serial_RxFlag = 0;
    return flag;
}

void USART1_IRQHandler(void)
{
    static uint8_t RxState = 0; // 状态机状态位
    static uint8_t RxIdx = 0;   // rx数据包的数组下标
    if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
    {
        uint8_t Serial_RxByte = USART_ReceiveData(USART1);
        if (RxState == 0) // 0代表包头
        {
            if (Serial_RxByte == 0xff)
            {
                RxState = 1;
            }
        }
        else if (RxState == 1) // 1代表数据位
        {
            Serial_RxPacket[RxIdx++] = Serial_RxByte;
            if (RxIdx == 4)
            {
                RxIdx = 0;
                RxState = 2;
            }
        }
        else if (RxState == 2) // 2代表包尾
        {
            if (Serial_RxByte == 0xfe)
            {
                RxState = 0;
                Serial_RxFlag = 1;
            }
        }
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }
}

#include "stm32f10x.h"
#define MySPI_CLK_Pin GPIO_Pin_5
#define MySPI_MISO_Pin GPIO_Pin_6
#define MySPI_MOSI_Pin GPIO_Pin_7

/*
    初始化一个从机
    该函数需要一个cs引脚的参数，表示从机的cs线

    接线：
    CS:     函数参数指定
    CLK:    GPIOA5
    MISO:   GPIOA6
    MOSI:   GPIOA7

    交换字节模式：模式0
    模式0：
    CLK空闲为低电平
    在CS下降沿时移出第一个bit
    在CLK第一个边沿输入采样移入数据，第二个边沿移出下一位
*/
void MySPI_Init(uint16_t CSPin)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;                      // 设置推挽输出模式，因为SPI通信没有上拉的设计，所有的信号都是手动拉高或者拉低
    GPIO_InitStruct.GPIO_Pin = CSPin | MySPI_CLK_Pin | MySPI_MOSI_Pin; // pin6是输入需要单独设置模式
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU; // 输入引脚设置浮空挥着上拉，这里选择上拉代表默认高电平
    GPIO_InitStruct.GPIO_Pin = MySPI_MISO_Pin;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_SetBits(GPIOA, CSPin);           // 设置片选线的默认电平：高电平
    GPIO_ResetBits(GPIOA, MySPI_CLK_Pin); // 设置CLK的默认电平：低电平
}

// 控制与CS线的电平
void MySPI_WriteCS(uint16_t CSPin, uint8_t BitVal)
{
    GPIO_WriteBit(GPIOA, CSPin, (BitAction)BitVal);
}

// 开启与一个从机的通信
void MySPI_Start(uint16_t CSPin)
{
    MySPI_WriteCS(CSPin, 0);
}

// 停止与一个从机的通信
void MySPI_Stop(uint16_t CSPin)
{
    MySPI_WriteCS(CSPin, 1);
}

// 设置时钟
void MySPI_WriteCLK(uint8_t BitVal)
{
    GPIO_WriteBit(GPIOA, MySPI_CLK_Pin, (BitAction)BitVal);
}

// 发送一位
void MySPI_WriteMOSI(uint8_t BitVal)
{
    GPIO_WriteBit(GPIOA, MySPI_MOSI_Pin, (BitAction)BitVal);
}

// 读取一位
uint8_t MySPI_ReadMISO(void)
{
    return GPIO_ReadInputDataBit(GPIOA, MySPI_MISO_Pin);
}

uint8_t MySPI_SwapByte(uint8_t ByteSend)
{
    for (int8_t i = 0; i < 8; i++)
    {
        // 移出有两种情况：
        // 1.cs开始时，需要移出一位，但是此时的clk为空闲状态，没有触发边沿
        // 2.cs已经选中，clk第二个边沿移出（偶数边沿）
        // 已知cs选中后，紧跟着发送字节的时序，所以此时不能改变clk的电平，直接将第一个比特发送出去，满足条件1
        // 条件2是需要第二个边沿发送，而前一个边沿刚好是接收一个比特（clk上升沿，也就是高电平），所以在循环中，执行完接收比特后，将clk置为低电平，在下一格循环开始时，满足条件2

        // 此处代码符合SPI通信的交换移位模型，将发送数据最高位移出，最低位传入接收数据
        MySPI_WriteMOSI(ByteSend & 0x80); // 移出最高位
        ByteSend <<= 1;                   // 左移一位
        MySPI_WriteCLK(1);
        ByteSend |= MySPI_ReadMISO(); // 最低位接收一个比特
        MySPI_WriteCLK(0);
    }
    return ByteSend;
}

#include "stm32f10x.h"
#include "Delay.h"

void MyI2C_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_InitStruc;
    GPIO_InitStruc.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStruc.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_InitStruc.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStruc);

    GPIO_SetBits(GPIOB, GPIO_Pin_10 | GPIO_Pin_11); // 默认高电平
}

// 软件模拟I2C需要定义六个时序基本单元：开始、结束、发送应答、接收应答、

// 封装scl和sda的电平修改操作，这样的好处是六个基本时序单元调用这些操作，如果以后需要修改pin口，可以只需要修改这些函数中的值即可
void MyI2C_WriteSCL(uint8_t BitValue)
{
    GPIO_WriteBit(GPIOB, GPIO_Pin_10, (BitAction)BitValue);
    Delay_us(10); // 延时是为了保证I2C从机的频率能够跟得上
}

void MyI2C_WriteSDA(uint8_t BitValue)
{
    GPIO_WriteBit(GPIOB, GPIO_Pin_11, (BitAction)BitValue);
    Delay_us(10);
}

uint8_t MyI2C_ReadSDA(void)
{
    uint8_t BitValue = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11);
    Delay_us(10);
    return BitValue;
}

void MyI2C_Start(void)
{
    // 由于需要兼容重复起始条件，所以需要先拉高scl和sda，确保之后的起始条件能够被下拉电平
    MyI2C_WriteSCL(0);
    MyI2C_WriteSDA(1); // 注意此处的sda先拉高，因为在重复起始条件前防止被识别为终止条件
    MyI2C_WriteSCL(1);
    MyI2C_WriteSDA(0);
}

void MyI2C_Stop(void)
{
    MyI2C_WriteSCL(0);
    MyI2C_WriteSDA(0);
    MyI2C_WriteSCL(1);
    MyI2C_WriteSDA(1);
}

void MyI2C_SendByte(uint8_t Byte)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        MyI2C_WriteSCL(0);
        MyI2C_WriteSDA((0x80 >> i) & Byte);
        MyI2C_WriteSCL(1);
    }
}

uint8_t MyI2C_RecieveByte(void)
{
    uint8_t Byte = 0;
    MyI2C_WriteSCL(0);
    // 接收：主机需要释放sda
    MyI2C_WriteSDA(1);

    for (uint8_t i = 0; i < 8; i++)
    {
        MyI2C_WriteSCL(0); // 让从机写数据
        MyI2C_WriteSCL(1); // 进行行读取，需要释放scl
        Byte = Byte | (MyI2C_ReadSDA() << (7 - i));
    }

    return Byte;
}

void MyI2C_SendAck(uint8_t Bit)
{
    MyI2C_WriteSCL(0);
    MyI2C_WriteSDA(Bit);
    MyI2C_WriteSCL(1);
}

uint8_t MyI2C_RecieveAck(void)
{
    MyI2C_WriteSCL(0); // 让从机写数据
    // 接收：主机需要释放sda
    MyI2C_WriteSDA(1);
    MyI2C_WriteSCL(1); // 进行行读取，需要释放scl
    uint8_t Bit = MyI2C_ReadSDA();

    return Bit;
}

#include "stm32f10x.h"
#include "MPU6050_Reg.h"
#include "MPU6050.h"

#define MPU6050_ADDRESS 0xD0

struct MPU6050_DataStruct MPU6050_Data;

void MPU6050_Init(void)
{
    // MyI2C_Init();
    // 初始化硬件i2c
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD; // 硬件i2c需要设置为复用开漏模式
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    I2C_InitTypeDef I2C_InitStruct;
    I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
    // i2c的频率，100KHz及以下是标准模式，100KHz以上是快速模式
    I2C_InitStruct.I2C_ClockSpeed = 400000;
    // i2c信号占空比，但是此处不是指 高电平时间/总时间，而是低电平时间/高电平时间
    // 在i2c实际的波形中，由于是开漏模式，所以下降沿的速度通常要比上升沿快很多
    // 并且因为弱上拉导致了上升沿的波形较为平滑，转换时间较长
    // 在scl低电平期间，sda需要不断的翻转电平来输出数据，所以低电平的时间需要长一点，否则可能在上升沿还没结束就翻转为低电平，导致数据错误
    // 而scl高电平，是用来去读数据，一般速度较快，不需要翻转sda的电平，时间可以相对短一点
    I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;                               // 是否发送应答，之后如果需要结束通信，需要更改应答
    I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; // 这个参数是stm32作为从机，可以响应多少位的地址
    I2C_InitStruct.I2C_OwnAddress1 = 0x00;                                 // stm32自身的从机地址，只有stm32作为从机才会用得到
    I2C_Init(I2C2, &I2C_InitStruct);

    I2C_Cmd(I2C2, ENABLE);

    // 配置MPU6050
    // 电源管理1寄存器
    /*
    MPU6050_PWR_MGMT_1 寄存器是 MPU6050 的电源管理寄存器 1，它包含了不同位的配置选项。以下是每个位的表示和作用：
    位 7 (DEVICE_RESET): 设备复位位。当该位被设置为 1 时，会将 MPU6050 的寄存器和电路复位到初始状态。复位完成后，该位会自动清零。
    位 6 (SLEEP): 睡眠模式位。当该位被设置为 1 时，MPU6050 进入睡眠模式，以降低功耗。在睡眠模式下，大部分功能都被禁用，只有唤醒功能和部分传感器会保持工作。
    位 5 (CYCLE): 循环唤醒位。当该位被设置为 1 时，在睡眠模式下，MPU6050 将周期性地唤醒，进行数据采集和处理，然后再次进入睡眠。该功能可用于定时采集数据。
    位 4 (TEMP_DIS): 温度传感器禁用位。当该位被设置为 1 时，禁用 MPU6050 内部的温度传感器，以降低功耗。在某些情况下，如果不需要温度数据，可以禁用该传感器以节省能量。
    位 3-0 (CLKSEL[3:0]): 时钟源选择位。这 4 位用于选择 MPU6050 的时钟源。以下是一些常见的时钟源选项：
    0000: 内部振荡器 (默认配置)
    0001: PLL X轴振荡器
    0010: PLL Y轴振荡器
    0011: PLL Z轴振荡器
    0100: PLL 外部 32.768kHz 晶振
    0101-0111: 保留
    1000: 外部引脚提供的时钟源
    1001-1111: 保留
    */
    MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x01);
    /*
    MPU6050_PWR_MGMT_2 寄存器是 MPU6050 的电源管理寄存器 2，它包含了不同位的配置选项。以下是每个位的表示和作用：
    位 7:5 (RESERVED): 保留位，应始终设置为 0，保留供将来使用。
    位 4 (LP_WAKE_CTRL[2]): 低功耗唤醒控制位。这个位用于设置低功耗唤醒模式的唤醒频率。以下是一些常见的选项：
    000: 唤醒频率为 1.25 Hz
    001: 唤醒频率为 2.5 Hz
    010: 唤醒频率为 5 Hz
    011: 唤醒频率为 10 Hz
    100: 唤醒频率为 20 Hz
    101: 唤醒频率为 40 Hz
    110: 唤醒频率为 80 Hz
    111: 唤醒频率为 160 Hz
    位 3:0 (STBY_XA, STBY_YA, STBY_ZA, STBY_XG, STBY_YG, STBY_ZG): 加速度计和陀螺仪的待机模式位。每一位对应于相应轴上的传感器。当位被设置为 1 时，对应轴上的传感器进入待机模式以降低功耗。当位被设置为 0 时，对应轴上的传感器处于正常工作模式。
    */
    MPU6050_WriteReg(MPU6050_PWR_MGMT_2, 0x00);
    /*
    MPU6050_WriteReg(MPU6050_SMPLRT_DIV, 0x09);这行代码将 MPU6050 的采样率分频器（Sample Rate Divider）设置为 9。具体的采样率计算公式可以在 MPU6050 的文档中找到，根据采样率和陀螺仪的输出频率进行配置。
    MPU6050_WriteReg(MPU6050_CONFIG, 0x06);这行代码配置了 MPU6050 的低通滤波器的截止频率和陀螺仪的工作模式。具体配置值 0x06 表示启用低通滤波器，并设置截止频率为 5 Hz。
    MPU6050_WriteReg(MPU6050_GYRO_CONFIG, 0x18);这行代码配置了 MPU6050 的陀螺仪的全量程范围和自检功能。具体配置值 0x18 表示陀螺仪的全量程范围为 ±2000 °/s。
    MPU6050_WriteReg(MPU6050_ACCEL_CONFIG, 0x18);这行代码配置了 MPU6050 的加速度计的全量程范围和自检功能。具体配置值 0x18 表示加速度计的全量程范围为 ±16 g。
    */
    MPU6050_WriteReg(MPU6050_SMPLRT_DIV, 0x09);
    MPU6050_WriteReg(MPU6050_CONFIG, 0x06);
    MPU6050_WriteReg(MPU6050_GYRO_CONFIG, 0x18);
    MPU6050_WriteReg(MPU6050_ACCEL_CONFIG, 0x18);
}

// 此函数是对I2C_CheckEvent函数的封装，目的是等待某个事件的产生，并且具有超时处理
void MPU6050_WaitEvent(uint32_t I2C_EVENT)
{
    uint16_t TimeOut = 10000;
    while (I2C_CheckEvent(I2C2, I2C_EVENT) == ERROR)
    {
        TimeOut--;
        if (TimeOut == 0)
        {
            break;
        }
    }
}

void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data)
{
    // MyI2C_Start();
    // MyI2C_SendByte(MPU6050_ADDRESS);
    // MyI2C_RecieveAck();
    // MyI2C_SendByte(RegAddress);
    // MyI2C_RecieveAck();
    // MyI2C_SendByte(Data);
    // MyI2C_RecieveAck();
    // MyI2C_Stop();

    // 硬件的i2c函数中，没有delay延时，但是有某些操作的状态标志位，所以在执行某一步操作时，需要检查上一步是否完成，方法是调用检查标志为函数
    I2C_GenerateSTART(I2C2, ENABLE);
    MPU6050_WaitEvent(I2C_EVENT_MASTER_MODE_SELECT); // ev5

    I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Transmitter);
    // i2c硬件发送一个字节之后会自动接收应答位，不需要手动接收，如果应答为为0，会通过事件或者标志为通知我们
    MPU6050_WaitEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED); // ev6事件

    // 手册中，此时还有一个ev8_1事件，表示需要写入DR，但是我们并不需要等待，而且也没有ev8_1的参数，所以直接写入DR就可以了
    I2C_SendData(I2C2, RegAddress); // 指定从机寄存器地址，写入dr，接着放入移位寄存器发送
    // 此时需要等待ev8事件的到来，ev8表示移位寄存器不为空，数据寄存器空，写DR清除事件
    // 所以在ev8事件来临时，可以写入下一个DR
    MPU6050_WaitEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTING);

    I2C_SendData(I2C2, Data); // 写入DR，发送数据
    // 到此，一个字节的通信到达末尾，速妖终止通信，需要等待ev8_2事件的到来
    MPU6050_WaitEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED);

    I2C_GenerateSTOP(I2C2, ENABLE); // 产生结束条件
}

uint8_t MPU6050_ReadReg(uint8_t RegAddress) // 读寄存器
{
    uint8_t Data = 0;

    // MyI2C_Start();
    // // 开始是指定地址的写操作，讲要读取的地址写入从机的地址寄存器
    // MyI2C_SendByte(MPU6050_ADDRESS);
    // MyI2C_RecieveAck();
    // MyI2C_SendByte(RegAddress);
    // MyI2C_RecieveAck();

    // // 写入完地址寄存器后，开始重复起始条件，执行读当前寄存器操作
    // MyI2C_Start();
    // MyI2C_SendByte(MPU6050_ADDRESS | 0x01); // 重复起始仍然要指定从机，此时需要设置该从机的读操作，所以需要最后一位是1，进行或操作
    // MyI2C_RecieveAck();
    // Data = MyI2C_RecieveByte(); // 接收数据
    // MyI2C_SendAck(1);           // 此处的发送应答，并不是严格的指有没有收到数据，而是表示主机当前是否还需要继续读取下一个地址的数据，如果不想读了，就给一个否定的应答，即1
    // MyI2C_Stop();

    I2C_GenerateSTART(I2C2, ENABLE);
    MPU6050_WaitEvent(I2C_EVENT_MASTER_MODE_SELECT);

    I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Transmitter);
    MPU6050_WaitEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);

    I2C_SendData(I2C2, RegAddress);
    // 注意，此时是在写入从机寄存器地址  和   重复起始条件发送  之间
    // 那么应该使等待 正在发送的事件 还是 发送完毕的事件，如果是正在发送的事件，会不会还没发送完毕就生成起始条件导致数据截断？
    // 此处两者都没问题，如果是正在发送，起始条件会等待发送完毕
    MPU6050_WaitEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED);

    I2C_GenerateSTART(I2C2, ENABLE);
    MPU6050_WaitEvent(I2C_EVENT_MASTER_MODE_SELECT);

    I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Receiver);
    MPU6050_WaitEvent(I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED); // ev6

    // 在ev6事件之后，表示从机地址已经指定完毕，可以开始读取数据了，ev6之后会紧跟着ev6_1
    // 读取第一个字节时会产生ev6_1，但是ev6_1没有对应的标志位，ev6_1表示第一个字节开始接收并且只需要接收一个字节
    // 需要注意，在ev6之后，ev6_1之前需要将ack置0，因为ev6_1时开始接收字节，必须提前将 应答位  和  停止位 设置，否则字节接收完毕之后就迟了
    I2C_AcknowledgeConfig(I2C2, DISABLE);
    I2C_GenerateSTOP(I2C2, ENABLE);
    // 在设置应答位和停止位时，已经处于ev6_1的过程中，也就是意味着正在接收字节，接收的字节会写入dr中
    // 在ev6_1事件结束后，应答位和停止位就会产生作用

    MPU6050_WaitEvent(I2C_EVENT_MASTER_BYTE_RECEIVED); // ev7
    // 此处需要先等待接收完毕的事件，因为接收是被动，需要等待从机发送完毕，并且数据全部写入dr中才可以读取
    // 而之前的发送是主动发送，是先写入dr寄存器，再发送，接收是等待写入完毕，才可以读取，注意顺序
    Data = I2C_ReceiveData(I2C2);

    // 最后需要将ack恢复为1，方便下次使用
    I2C_AcknowledgeConfig(I2C2, ENABLE);

    return Data;
}

struct MPU6050_DataStruct MPU6050_GetData(void)
{
    MPU6050_Data.AccX = (MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H) << 8) | MPU6050_ReadReg(MPU6050_ACCEL_XOUT_L);
    MPU6050_Data.AccY = (MPU6050_ReadReg(MPU6050_ACCEL_YOUT_H) << 8) | MPU6050_ReadReg(MPU6050_ACCEL_YOUT_L);
    MPU6050_Data.AccZ = (MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_H) << 8) | MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_L);
    MPU6050_Data.GyroX = (MPU6050_ReadReg(MPU6050_GYRO_XOUT_H) << 8) | MPU6050_ReadReg(MPU6050_GYRO_XOUT_L);
    MPU6050_Data.GyroY = (MPU6050_ReadReg(MPU6050_GYRO_YOUT_H) << 8) | MPU6050_ReadReg(MPU6050_GYRO_YOUT_L);
    MPU6050_Data.GyroZ = (MPU6050_ReadReg(MPU6050_GYRO_ZOUT_H) << 8) | MPU6050_ReadReg(MPU6050_GYRO_ZOUT_L);
    MPU6050_Data.Temp = (MPU6050_ReadReg(MPU6050_TEMP_OUT_H) << 8) | MPU6050_ReadReg(MPU6050_TEMP_OUT_L);
    return MPU6050_Data;
}

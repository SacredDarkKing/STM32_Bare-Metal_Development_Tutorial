#include "stm32f10x.h"
#include "MPU6050_Reg.h"
#include "MyI2C.h"
#include "MPU6050.h"

#define MPU6050_ADDRESS 0xD0

struct MPU6050_DataStruct MPU6050_Data;

void MPU6050_Init(void)
{
    MyI2C_Init();
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

void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data)
{
    MyI2C_Start();
    MyI2C_SendByte(MPU6050_ADDRESS);
    MyI2C_RecieveAck();
    MyI2C_SendByte(RegAddress);
    MyI2C_RecieveAck();
    MyI2C_SendByte(Data);
    MyI2C_RecieveAck();
    MyI2C_Stop();
}

uint8_t MPU6050_ReadReg(uint8_t RegAddress) // 读寄存器
{
    uint8_t Data = 0;

    MyI2C_Start();
    // 开始是指定地址的写操作，讲要读取的地址写入从机的地址寄存器
    MyI2C_SendByte(MPU6050_ADDRESS);
    MyI2C_RecieveAck();
    MyI2C_SendByte(RegAddress);
    MyI2C_RecieveAck();

    // 写入完地址寄存器后，开始重复起始条件，执行读当前寄存器操作
    MyI2C_Start();
    MyI2C_SendByte(MPU6050_ADDRESS | 0x01); // 重复起始仍然要指定从机，此时需要设置该从机的读操作，所以需要最后一位是1，进行或操作
    MyI2C_RecieveAck();
    Data = MyI2C_RecieveByte(); // 接收数据
    MyI2C_SendAck(1);           // 此处的发送应答，并不是严格的指有没有收到数据，而是表示主机当前是否还需要继续读取下一个地址的数据，如果不想读了，就给一个否定的应答，即1
    MyI2C_Stop();

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

#ifndef __MPU6050_REG_H
#define __MPU6050_REG_H

//mpu6050寄存器的地址宏定义声明
#define MPU6050_SMPLRT_DIV    0x19   // 采样率分频器
#define MPU6050_CONFIG        0x1A   // 配置寄存器
#define MPU6050_GYRO_CONFIG   0x1B   // 陀螺仪配置寄存器
#define MPU6050_ACCEL_CONFIG  0x1C   // 加速度计配置寄存器
#define MPU6050_ACCEL_XOUT_H  0x3B   // 加速度计X轴高位字节
#define MPU6050_ACCEL_XOUT_L  0x3C   // 加速度计X轴低位字节
#define MPU6050_ACCEL_YOUT_H  0x3D   // 加速度计Y轴高位字节
#define MPU6050_ACCEL_YOUT_L  0x3E   // 加速度计Y轴低位字节
#define MPU6050_ACCEL_ZOUT_H  0x3F   // 加速度计Z轴高位字节
#define MPU6050_ACCEL_ZOUT_L  0x40   // 加速度计Z轴低位字节
#define MPU6050_TEMP_OUT_H    0x41   // 温度传感器高位字节
#define MPU6050_TEMP_OUT_L    0x42   // 温度传感器低位字节
#define MPU6050_GYRO_XOUT_H   0x43   // 陀螺仪X轴高位字节
#define MPU6050_GYRO_XOUT_L   0x44   // 陀螺仪X轴低位字节
#define MPU6050_GYRO_YOUT_H   0x45   // 陀螺仪Y轴高位字节
#define MPU6050_GYRO_YOUT_L   0x46   // 陀螺仪Y轴低位字节
#define MPU6050_GYRO_ZOUT_H   0x47   // 陀螺仪Z轴高位字节
#define MPU6050_GYRO_ZOUT_L   0x48   // 陀螺仪Z轴低位字节
#define MPU6050_PWR_MGMT_1    0x6B   // 电源管理1寄存器
#define MPU6050_PWR_MGMT_2    0x6C   // 电源管理2寄存器
#define MPU6050_WHO_AM_i      0x75   // 设备ID寄存器


#endif

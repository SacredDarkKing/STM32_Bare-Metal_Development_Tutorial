#ifndef __MPU6050_H
#define __MPU6050_H

struct MPU6050_DataStruct
{
    int16_t AccX;
    int16_t AccY;
    int16_t AccZ;

    int16_t GyroX;
    int16_t GyroY;
    int16_t GyroZ;

    int16_t Temp;
};


void MPU6050_Init(void);
void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data);
uint8_t MPU6050_ReadReg(uint8_t RegAddress);
struct MPU6050_DataStruct MPU6050_GetData(void);

#endif

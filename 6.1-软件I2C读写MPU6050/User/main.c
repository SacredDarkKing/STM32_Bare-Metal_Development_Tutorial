#include "stm32f10x.h" // Device header
#include "OLED.h"
#include "MPU6050.h"
#include "MPU6050_Reg.h"

int main(void)
{
	OLED_Init();
	MPU6050_Init();

	MPU6050_WriteReg(0x6b, 0x00); // 解除睡眠模式
	uint8_t Id = MPU6050_ReadReg(MPU6050_WHO_AM_i);

	OLED_ShowString(1, 1, " ACC       GYRO ");
	OLED_ShowString(1, 6, "TP: ");
	OLED_ShowHexNum(1, 9, Id, 2);

	struct MPU6050_DataStruct data;
	while (1)
	{
		data = MPU6050_GetData();

		float temperature = (float)data.Temp / 340.0 + 36.53;
		OLED_ShowNum(1, 9, temperature, 2);
		//OLED_ShowString(1, 11, "\xB0");

		OLED_ShowSignedNum(2, 2, data.AccX, 5);
		OLED_ShowSignedNum(2, 10, data.GyroX, 5);

		OLED_ShowSignedNum(3, 2, data.AccY, 5);
		OLED_ShowSignedNum(3, 10, data.GyroY, 5);

		OLED_ShowSignedNum(4, 2, data.AccZ, 5);
		OLED_ShowSignedNum(4, 10, data.GyroZ, 5);
	}
}

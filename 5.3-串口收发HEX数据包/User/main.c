#include "stm32f10x.h" // Device header
#include "OLED.h"
#include "Serial.h"
#include "Key.h"

int main(void)
{

	OLED_Init();
	Serial_Init();
	Key_Init();

	Serial_TxPacket[0] = 1;
	Serial_TxPacket[1] = 2;
	Serial_TxPacket[2] = 3;
	Serial_TxPacket[3] = 4;

	// uint8_t msg = 0;

	OLED_ShowString(1, 1, "TxPacket");
	OLED_ShowString(3, 1, "TxPacket");

	while (1)
	{

		if (Key_GetNum() == 1)
		{
			Serial_SendPacket();
			OLED_ShowHexNum(2, 1, Serial_TxPacket[0], 2);
			OLED_ShowHexNum(2, 4, Serial_TxPacket[1], 2);
			OLED_ShowHexNum(2, 7, Serial_TxPacket[2], 2);
			OLED_ShowHexNum(2, 10, Serial_TxPacket[3], 2);
		}

		if (Serial_GetRxFlag() == 1)
		{
			OLED_ShowHexNum(4, 1, Serial_RxPacket[0], 2);
			OLED_ShowHexNum(4, 4, Serial_RxPacket[1], 2);
			OLED_ShowHexNum(4, 7, Serial_RxPacket[2], 2);
			OLED_ShowHexNum(4, 10, Serial_RxPacket[3], 2);
		}
	}
}

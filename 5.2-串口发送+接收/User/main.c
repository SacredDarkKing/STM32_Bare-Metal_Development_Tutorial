#include "stm32f10x.h" // Device header
#include "OLED.h"
#include "Serial.h"

int main(void)
{

	OLED_Init();
	Serial_Init();
	// Serial_SendByte('X');
	// uint8_t arr[] = {98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108};
	// Serial_SendArray(arr, 11);

	// Serial_SendString("\r\nhello, bitch\r\n");
	// Serial_SendNumber(987654321);
	// printf("number=%d\r\n", 999);
	Serial_Printf("hello, world! num=%d\r\n", 987654321);

	uint8_t msg = 0;

	while (1)
	{
		if (Serial_GetRxFlag())
		{
			msg = Serial_GetRxMsg();
			Serial_SendByte(msg);
			OLED_ShowHexNum(1, 1, msg, 2);
			OLED_ShowChar(2, 1, msg);
		}
	}
}

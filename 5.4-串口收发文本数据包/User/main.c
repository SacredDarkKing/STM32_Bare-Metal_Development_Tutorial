#include "stm32f10x.h" // Device header
#include "OLED.h"
#include "Serial.h"
#include "Key.h"
#include "string.h"
#include "LED.h"

int main(void)
{

	OLED_Init();
	Serial_Init();
	Key_Init();
	LED_Init();
	// uint8_t msg = 0;

	OLED_ShowString(1, 1, "RxPacket");

	while (1)
	{

		if (Key_GetNum() == 1)
		{
			Serial_SendString("9876");
		}

		if (Serial_GetRxFlag() == 1)
		{
			OLED_ShowString(2, 1, "                            ");
			OLED_ShowString(3, 1, "                            ");
			OLED_ShowString(2, 1, Serial_RxPacket);
			if (strcmp(Serial_RxPacket, "LED_ON") == 0)
			{
				LED2_On();
				OLED_ShowString(3, 1, "LED_ON");
				Serial_SendString("LED_ON");
			}
			else if (strcmp(Serial_RxPacket, "LED_OFF") == 0)
			{
				LED2_Off();
				OLED_ShowString(3, 1, "LED_OFF");
				Serial_SendString("LED_OFF");
			}
			else
			{
				OLED_ShowString(3, 1, "CMD_ERROR");
				Serial_SendString("CMD_ERROR");
			}
		}
	}
}

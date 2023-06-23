#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "Key.h"
#include "Buzzer.h"
#include "LightSensor.h"
#include "OLED.h"

uint8_t Key_Num;

int main(void)
{	
	uint8_t Key_Num = 0; 
	LED_Init();
	Key_Init();
	//Buzzer_Init();
	//LightSensor_Init();
	OLED_Init();
	
	OLED_ShowChar(3, 7, 'A' );
	OLED_ShowChar(3, 10, 'B');
	
	OLED_ShowChar(1, 1, 'A');
	OLED_ShowString(1, 3, "Hello, world!");
	OLED_ShowNum(2, 1, 12345, 3);
	OLED_ShowSignedNum(2, 5, -987, 3);
	OLED_ShowHexNum(3, 1, 0xABCD, 4);
	OLED_ShowBinNum(4, 1, 0xABCD, 16);

	
	while (1)
	{
		

		
		
		Key_Num = Key_GetNum();
		if (Key_Num == 1)
		{
			LED2_Turn();
		}

		if (Key_Num == 2)
		{
			LED3_Turn();
		}
		
		
		if (LightSensor_Get() == 0)
		{
			Buzzer12_On();
		} else 
		{
			Buzzer12_Off();
		}
	}
}

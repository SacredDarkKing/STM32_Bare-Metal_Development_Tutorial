#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "Key.h"
#include "Buzzer.h"
#include "LightSensor.h"
#include "OLED.h"
#include "ProximitySensor.h"

uint8_t Key_Num;

int main(void)
{	
	uint8_t Key_Num = 0; 
	LED_Init();
	Key_Init();
	//Buzzer_Init();
	//LightSensor_Init();
	OLED_Init();
	ProximitySensor_Init();
	
	OLED_ShowChar(3, 7, 'A' );
	OLED_ShowChar(3, 10, 'B');

	
	while (1)
	{
		
		OLED_ShowString(1, 1, "Count: ");
		OLED_ShowSignedNum(1, 8, get_proximitySensor_Count(), 4);
		
		
		
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

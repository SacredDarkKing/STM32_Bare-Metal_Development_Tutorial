#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "Key.h"
#include "Buzzer.h"
#include "LightSensor.h"

uint8_t Key_Num;

int main(void)
{	
	uint8_t Key_Num = 0; 
	LED_Init();
	Key_Init();
	Buzzer_Init();
	LightSensor_Init();
	
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

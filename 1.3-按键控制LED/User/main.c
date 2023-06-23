#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "Key.h"

uint8_t Key_Num;

int main(void)
{	
	LED_Init();
	Key_Init();
	
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

	}
}

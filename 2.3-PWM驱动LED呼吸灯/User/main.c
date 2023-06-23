#include "stm32f10x.h" // Device header
#include "PWM.h"
#include "OLED.h"
#include "Delay.h"

int main(void)
{
	OLED_Init();
	PWM_Init();

	while (1)
	{
		for (int i = 1; i <= 100; i++)
		{
			PWM_SetCompare1(i);
			Delay_ms(10);
		}

		for (int i = 1; i <= 99; i++)
		{
			PWM_SetCompare1(100 - i);
			Delay_ms(10);
		}
	}
}

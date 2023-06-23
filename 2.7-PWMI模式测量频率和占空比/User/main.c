#include "stm32f10x.h" // Device header
#include "PWM.h"
#include "OLED.h"
#include "Delay.h"
#include "IC.h"
#include "Key.h"

int main(void)
{
	OLED_Init();
	PWM_Init();
	IC_Init();
	Key_Init();

	uint16_t freq = 1000;
	PWM_SetCompare1(80);

	while (1)
	{
		if (Key_GetNum() == 1)
		{
			freq = freq * 10 % 10001;
			PWM_SetFrequence(freq);
		
		}
		OLED_ShowString(1, 1, "freq: ");
		OLED_ShowNum(1, 7, IC_GetFreqence(), 5);
		
		OLED_ShowString(2, 1, "duty: ");
		OLED_ShowNum(2, 7, IC_GetDuty(), 5);
	}
}

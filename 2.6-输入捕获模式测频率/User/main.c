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
	PWM_SetCompare1(50);

	while (1)
	{
		if (Key_GetNum() == 1)
		{
			freq = freq * 10 % 10001;
			PWM_SetFrequence(freq);
		
		}
		OLED_ShowNum(2, 1, freq, 5);
		OLED_ShowNum(1, 1, IC_GetFreqence(), 5);
		OLED_ShowNum(3, 1, TIM_GetCapture1(TIM3), 8);
	}
}

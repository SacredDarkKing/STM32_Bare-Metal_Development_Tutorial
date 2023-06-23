#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Timer.h"

int main(void)
{	

	OLED_Init();
	Timer_init();

	OLED_ShowChar(3, 7, 'A' );
	OLED_ShowChar(3, 10, 'B');

	
	while (1)
	{
		OLED_ShowString(1, 1, "num: ");
		OLED_ShowNum(1, 6, get_Timer_Cnt(), 6);
		OLED_ShowNum(2, 6, TIM_GetCounter(TIM2), 6);
	}
}

#include "stm32f10x.h" // Device header
#include "PWM.h"
#include "OLED.h"
#include "Delay.h"
#include "Motor.h"
#include "Key.h"

int main(void)
{
	OLED_Init();
	Motor_Init();
	Key_Init();

	int8_t speed[] = {0, 20, 40, 80, 100, -20, -40, -80, -100};
	int8_t idx = 0;

	OLED_ShowString(1, 1, "speed: ");
	OLED_ShowSignedNum(1, 7, speed[idx], 4);

	while (1)
	{
		
		if (Key_GetNum() == 1)
		{
			idx = (idx + 1) % 9;
			Motor_SetSpeed(speed[idx]);
			OLED_ShowSignedNum(1, 7, speed[idx], 4);
		}
		
	}
}

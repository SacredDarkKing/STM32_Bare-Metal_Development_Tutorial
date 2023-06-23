#include "stm32f10x.h" // Device header
#include "PWM.h"
#include "OLED.h"
#include "Delay.h"
#include "Servo.h"
#include "Key.h"

int main(void)
{
	OLED_Init();
	Key_Init();
	Servo_Init();

	OLED_ShowNum(1, 1, 0, 3);

	uint16_t angle = 0;
	while (1)
	{
		if (Key_GetNum() == 1)
		{
			angle = (angle + 30) % 210;
			OLED_ShowNum(1, 1, angle, 3);
			Servo_SetAngle(angle);
		}
	}
}

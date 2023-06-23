#include "stm32f10x.h"
#include "PWM.h"

void Servo_Init(void)
{
    PWM_Init();
}

void Servo_SetAngle(float angle)
{
    uint16_t pulse = 500 + (angle * 500 / 45);
    PWM_SetCompare2(pulse);
}

#include "stm32f10x.h" // Device header
#include "OLED.h"
#include "AD.h"

int main(void)
{
	OLED_Init();
	AD_Init();

	OLED_ShowString(1, 1, "ADVal: ");
	OLED_ShowString(2, 1, "Voltage1: 0.00V");

	while (1)
	{
		
		OLED_ShowNum(1, 8, AD_GetValue(), 6);
		float voltage = (float)AD_GetValue() / 4095 * 3.3;
		OLED_ShowNum(2, 11, voltage, 1);
		OLED_ShowNum(2, 13, (uint16_t)(voltage * 100) % 100, 2);
		
	}
}

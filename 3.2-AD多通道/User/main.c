#include "stm32f10x.h" // Device header
#include "OLED.h"
#include "AD.h"

int main(void)
{
	OLED_Init();
	AD_Init();

	OLED_ShowString(1, 1, "L1: 0.00V");
	OLED_ShowString(2, 1, "H2: 0000");
	OLED_ShowString(3, 1, "R3: 0.00V");
	OLED_ShowString(4, 1, "R4: 0.00V");

	float voltage = 0.0;

	while (1)
	{
		voltage = (float)AD_GetValue(ADC_Channel_2) / 4095 * 3.3;
		OLED_ShowNum(1, 5, voltage, 1);
		OLED_ShowNum(1, 7, (uint16_t)(voltage * 100) % 100, 2);

		voltage = AD_GetValue(ADC_Channel_3);
		OLED_ShowNum(2, 5, voltage, 4);
		//OLED_ShowNum(2, 7, (uint16_t)(voltage * 100) % 100, 2);

		voltage = (float)AD_GetValue(ADC_Channel_4) / 4095 * 3.3;
		OLED_ShowNum(3, 5, voltage, 1);
		OLED_ShowNum(3, 7, (uint16_t)(voltage * 100) % 100, 2);

		voltage = (float)AD_GetValue(ADC_Channel_5) / 4095 * 3.3;
		OLED_ShowNum(4, 5, voltage, 1);
		OLED_ShowNum(4, 7, (uint16_t)(voltage * 100) % 100, 2);
	}
}

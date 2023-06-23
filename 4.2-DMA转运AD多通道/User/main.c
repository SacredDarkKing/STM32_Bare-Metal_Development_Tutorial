#include "stm32f10x.h" // Device header
#include "OLED.h"
#include "AD.h"
#include "MyDMA.h"

int main(void)
{
	OLED_Init();
	AD_Init();

	OLED_ShowString(1, 1, "L1: 0.00V");
	OLED_ShowString(2, 1, "H2: 0000");
	OLED_ShowString(3, 1, "R3: 0.00V");
	OLED_ShowString(4, 1, "R4: 0.00V");

	while (1)
	{
		//AD_GetValue();

		// OLED_ShowNum(1, 5, arr[0], 4);
		// OLED_ShowNum(2, 5, arr[1], 4);
		// OLED_ShowNum(3, 5, arr[2], 4);
		// OLED_ShowNum(4, 5, arr[3], 4);

		float ad1 = (float)arr[0] / 4095 * 3.3;
		OLED_ShowNum(1, 5, ad1, 1);
		OLED_ShowNum(1, 7, (uint16_t)(ad1 * 100) % 100, 2);

		OLED_ShowNum(2, 5, arr[1], 4);
		// OLED_ShowNum(2, 7, (uint16_t)(arr[1] * 100) % 100, 2);

		float ad3 = (float)arr[2] / 4095 * 3.3;
		OLED_ShowNum(3, 5, ad3, 1);
		OLED_ShowNum(3, 7, (uint16_t)(ad3 * 100) % 100, 2);		
		
		float ad4 = (float)arr[3] / 4095 * 3.3;
		OLED_ShowNum(4, 5, ad4, 1);
		OLED_ShowNum(4, 7, (uint16_t)(ad4 * 100) % 100, 2);
	}
}

#include "stm32f10x.h" // Device header
#include "OLED.h"
#include "MyDMA.h"
#include "Delay.h"

uint16_t arr1[] = {0x01, 0x02, 0x03, 0x04};
uint16_t arr2[] = {0, 0, 0, 0};

int main(void)
{
	OLED_Init();

	MyDMA_Init((uint32_t)arr1, (uint32_t)arr2, 4);

	while (1)
	{

		OLED_ShowHexNum(1, 1, (uint32_t)&arr1, 8);

		OLED_ShowHexNum(2, 1, arr1[0], 2);
		OLED_ShowHexNum(2, 4, arr1[1], 2);
		OLED_ShowHexNum(2, 7, arr1[2], 2);
		OLED_ShowHexNum(2, 10, arr1[3], 2);

		Delay_ms(1000);

		MyDMA_Transfer();
		OLED_ShowHexNum(3, 1, (uint32_t)&arr2, 8);

		OLED_ShowHexNum(4, 1, arr2[0], 2);
		OLED_ShowHexNum(4, 4, arr2[1], 2);
		OLED_ShowHexNum(4, 7, arr2[2], 2);
		OLED_ShowHexNum(4, 10, arr2[3], 2);

		Delay_ms(1000);

		arr1[0]++;
		arr1[1]++;
		arr1[2]++;
		arr1[3]++;
	}
}

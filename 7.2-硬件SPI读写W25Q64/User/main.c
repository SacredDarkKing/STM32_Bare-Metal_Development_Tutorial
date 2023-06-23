#include "stm32f10x.h" // Device header
#include "OLED.h"
#include "W25Q64.h"
#include "Delay.h"

int main(void)
{
	OLED_Init();
	W25Q64_Init();

	uint8_t Manufacturer = 0;
	uint16_t ID = 0;
	W25Q64_GetID(&Manufacturer, &ID);

	OLED_ShowString(1, 1, "MID: ");
	OLED_ShowHexNum(1, 6, Manufacturer, 2);
	OLED_ShowString(2, 1, "DID: ");
	OLED_ShowHexNum(2, 6, ID, 4);

	uint8_t DataArr1[] = {0xD4, 0xC3, 0xB2, 0xA1};
	uint8_t DataArr2[4];

	// 写入前记得擦除扇区
	//W25Q64_SectorErase(0xF00000);
	//W25Q64_PageProgram(0xF00000, DataArr1, 4);
	W25Q64_ReadData(0xF00000, 4, DataArr2); // 如果读取超过arr2容量的n个，则超过的n个会从arr1头部开始替换
	// 原因：arr1的地址是2000064C，而arr2是20000648，arr2在arr1前面，由于越界所以越界的会覆盖arr1的数据

	OLED_ShowHexNum(3, 1, DataArr1[0], 2);
	OLED_ShowHexNum(3, 4, DataArr1[1], 2);
	OLED_ShowHexNum(3, 7, DataArr1[2], 2);
	OLED_ShowHexNum(3, 10, DataArr1[3], 2);

	OLED_ShowHexNum(4, 1, DataArr2[0], 2);
	OLED_ShowHexNum(4, 4, DataArr2[1], 2);
	OLED_ShowHexNum(4, 7, DataArr2[2], 2);
	OLED_ShowHexNum(4, 10, DataArr2[3], 2);

	while (1)
	{
	}
}

#ifndef __W25Q64_H
#define __W25Q64_H

void W25Q64_Init(void);
void W25Q64_GetID(uint8_t *Manufacturer, uint16_t *ID);
void W25Q64_PageProgram(uint32_t Address, uint8_t *Data, uint16_t Count);
uint8_t *W25Q64_ReadData(uint32_t Address, uint32_t Count, uint8_t *DataArr);
void W25Q64_SectorErase(uint32_t Address);

#endif

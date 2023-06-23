#ifndef __MYSPI_H
#define __MYSPI_H

void MySPI_Init(uint16_t CSPin);
void MySPI_Start(uint16_t CSPin);
void MySPI_Stop(uint16_t CSPin);
uint8_t MySPI_SwapByte(uint8_t ByteSend);

#endif

#ifndef __SERIAL_H
#define __SERIAL_H
#include <stdio.h>

extern uint8_t Serial_RxPacket[100];

void Serial_Init(void);
void Serial_SendByte(uint8_t msg);
void Serial_SendArray(uint8_t *arr, uint16_t len);
void Serial_SendString(char* string);
void Serial_SendNumber(uint32_t number);
void Serial_Printf(char *format, ...);
uint8_t Serial_GetRxFlag(void);
void USART1_IRQHandler(void);
void Serial_SendPacket(void);
#endif

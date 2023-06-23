#ifndef __MYI2C_H
#define __MYI2C_H

void MyI2C_Init(void);
void MyI2C_WriteSCL(uint8_t BitValue);
void MyI2C_WriteSDA(uint8_t BitValue);
uint8_t MyI2C_ReadSDA(void);
void MyI2C_Start(void);
void MyI2C_Stop(void);
void MyI2C_SendByte(uint8_t Byte);
uint8_t MyI2C_RecieveByte(void);
void MyI2C_SendAck(uint8_t Bit);
uint8_t MyI2C_RecieveAck(void);

#endif

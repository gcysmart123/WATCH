#ifndef _I2C_24C16_H
#define _I2C_24C16_H
#include <REG_MA82G5B32.H>


#include "typedef.h"
#include "define.h"
#include "keyproc.h"

#define		DELAY_TIME	1
#define		TRUE	1
#define		FALSE	0

#define		EEP_DEVICEA_DDR_W   0XA0   //24C02–¥µÿ÷∑
#define		EEP_DEVICEA_DDR_R   0XA1   //24C02∂¡µÿ÷∑

extern int8_t I2CEEr;
void I2C_Start(void);
void I2C_Stop(void);
void I2C_Ack(void);
void I2C_NoAck(void);
int8_t I2C_WaitAck(void);
void I2C_WriteByte(uint8_t dat);
int8_t I2C_ReadByte(void);
void DELAY(unsigned int t);

void EEP_WriteBytes(uint16_t addr,uint8_t *pbuf,uint16_t len);
void EEP_ReadBytes(uint16_t addr,uint8_t *pbuf,uint16_t len);

void Erase_all_EEPROM(void);

#endif

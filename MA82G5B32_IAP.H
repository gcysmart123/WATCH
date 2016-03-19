#ifndef _MA82G5B32_IAP_H
#define _MA82G5B32_IAP_H

#include <reg_ma82g5b32.h>
#include "typedef.h"

#define IAPLB   0X03

typedef enum
{
    Standby_Cmd=0,          /* ±¸ÓÃ  */  
    IAP_Read_Cmd=1,         /*  AP/IAP-´æ´¢Æ÷¶Á  */
    IAP_Write_Cmd=2,        /*  AP/IAP-´æ´¢Æ÷±à³Ì */
    IAP_PageErase_Cmd=3,    /*  AP/IAP-´æ´¢Æ÷Ò³²Á³ý */
	Ppage_Wirte_Cmd=4,      /*  P Ò³ SFR Ð´  */
	Ppage_Read_Cmd=5,       /*  P Ò³ SFR ¶Á  */
}Flash_Cmd;


void Page_P_SFR_Write(uint8_t SFR,uint8_t dat);  //PÒ³SFR¼Ä´æÆ÷Ð´
uint8_t Page_P_SFR_Read(uint8_t SFR);            //PÒ³SFR¼Ä´æÆ÷¶Á

// void IAP_FlashErease(uint16_t addr,uint16_t len);
// uint8_t IAP_ReadByte(uint16_t addr);
// bit IAP_ReadBytes(uint16_t addr,void *buf,uint16_t len);
// bit IAP_WirteByte(uint16_t addr,uint8_t dat);
// bit IAP_WirteBytes(uint16_t addr,void *pdat,uint16_t len);


#endif
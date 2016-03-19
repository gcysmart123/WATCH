#ifndef __MA82G5B32_UART_H
#define __MA82G5B32_UART_H

#include <reg_ma82g5b32.h>
#include "typedef.h"

//#define     SYSCLK              11059200UL     //系统时钟
#define     SYSCLK                6000000UL     //系统时钟
#define     UART0_BAUDRATE        9600UL         //串口0波特率

#define     UART0_BAUDRATE_MODE     3         //波特率发生器：1，使用定时器1。2，使用定时器2。3，使用串口1的发生器
       
void UART0_Init(void);
void UART0_SendByte(char chr);
void UART0_SendStr(const char *pStr);


#endif

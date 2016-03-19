#ifndef __MA82G5B32_UART_H
#define __MA82G5B32_UART_H

#include <reg_ma82g5b32.h>
#include "typedef.h"

//#define     SYSCLK              11059200UL     //ϵͳʱ��
#define     SYSCLK                6000000UL     //ϵͳʱ��
#define     UART0_BAUDRATE        9600UL         //����0������

#define     UART0_BAUDRATE_MODE     3         //�����ʷ�������1��ʹ�ö�ʱ��1��2��ʹ�ö�ʱ��2��3��ʹ�ô���1�ķ�����
       
void UART0_Init(void);
void UART0_SendByte(char chr);
void UART0_SendStr(const char *pStr);


#endif

#ifndef _MA82G5B32_RTC_H
#define _MA82G5B32_RTC_H

#include <REG_MA82G5B32.H>
#include "MA82G5B32_IAP.H"

extern xdata unsigned int POWERON_FLG;  //此变量判断是否是硬上电

extern xdata unsigned long RtcCounter;
extern bit bGetTime;

#define RTC_OUT_Enable()      RTCCR|=1<<6;       //开启RTC从P4.5输出
#define RTC_OUT_Disable()     RTCCR&=~(1<<6);    //禁止RTC输出

#define RTC_Cnt_Start()       RTCCR|=1<<7;       //启动RTC计数器
#define RTC_Cnt_Stop()        RTCCR&=~(1<<7);    //停止RTC计数器

#define RTC_Int_Enable()      SFIE|=1<<4;        //RTC开启中断
#define RTC_Int_Disable()     SFIE&=~(1<<4);     //RTC禁止中断


#define RTC_Clear_Int_Flag()  PCON1|=1<<4;       //写1到RTCF（BIT4）清除中断标志

typedef enum
{
	XTAL_DIV16384 = 0,
	XTAL_DIV32768 = 1,
	SYSCLK_DIV12 = 2,
	SYSCLK_DIV32768 = 3
}rtcclk_def;


void RTC_SourceClk_Config(rtcclk_def clock);
//void Set_RTC_Reload(unsigned char Dat6bits);
// unsigned char Get_RTC_Cnt(void);

void RTC_init(void);


#endif


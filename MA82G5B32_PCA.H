#ifndef __MA82G5B32_PCA_H
#define __MA82G5B32_PCA_H

#include <reg_ma82g5b32.h>
#include "typedef.h"

//PCA时钟源
typedef enum
{
    PCA_USE_SysClk_DIV12 = 0,   // 内部时钟，(system clock)/12做为PCA时钟源
    PCA_USE_SysClk_DIV2 = 1,    // 内部时钟， (system clock)/2做为PCA时钟源
    PCA_USE_TIMER0_OF = 2,      // 定时器 0 溢出率做为PCA时钟源
    PCA_USE_ExtInput = 3,       // 来自 ECI 引脚的外部时钟做为系统时钟源
    None1            = 4,       //保留，不使用
    PCA_USE_SysClk_DIV1 = 5,    //系统时钟(SYSCLK)做为PCA时钟源，不分频
    PCA_USE_SIBRG_OF = 6,       //S1BRG溢出
    None2 = 7                   //保留，不使用
}PCA_ClkDef;

//PCA工作模式定义
typedef enum
{
	PCA_None = 0,                  	   //无操作
	PCA_16BitUpCapture = 0X20,         //16位CEXn引脚上升沿触发捕获模式
	PCA_16BitDownCapture = 0X10,       //16位CEXn引脚下降沿触发捕获模式
	PCA_16BitCapture = 0X30,           //16位CEXn引脚跳变触发捕获模式
	PCA_16BitTimer = 0X48,             //16位软件定时器
	PCA_16BitOutput = 0X4c,            //16位高速输出
	PCA_PWM_Output	= 0X42	           //8位脉宽调制器(PWM)
}PCA_Mode_def;


//PWM工作模式定义
typedef enum
{
    PWM_Mode_8bit = 0,           //8位PWM模式
    PWM_Mode_10bit = 0X40,        //10位PWM模式
    PWM_Mode_12bit = 0X80,        //12位PWM模式
    PWM_Mode_16bit = 0XC0         //16位PWM模式
}PWM_Mode_def;

//PWM的开始相位定义
typedef enum
{
        PWM_Phase_00 =  0,    //使能的 PWM 通道在 0 度开始
        PWM_Phase_90 =  0X08, //使能的 PWM 通道在 90 度开始
        PWM_Phase_180 = 0X10, //使能的 PWM 通道在 180 度开始
        PWM_Phase_270 = 0X18, //使能的 PWM 通道在 270 度开始
        PWM_Phase_120 = 0X20, //使能的 PWM 通道在 120 度开始
        PWM_Phase_240 = 0X28, //使能的 PWM 通道在 240 度开始
        PWM_Phase_60 =  0X30, //使能的 PWM 通道在 60 度开始
        PWM_Phase_300 = 0X38  //使能的 PWM 通道在 300 度开始
}PWM_StarPhase_def;


#define PCA_Go_Idle_Mode()      CMOD&=0X7F;      //让 PCA 计数器在空闲模式下继续运行
#define PCA_Halt_Idle_Mode()    CMOD|=0x80;      //空闲模式下关闭 PCA 计数器

#define PCA_ECF_Disable()       CMOD&=0XFE;      //当 CF 位（CCON 寄存器中）置位时禁止中断
#define PCA_ECF_Enable()        CMOD|=0X01;      //当 CF 位（CCON 寄存器中）置位时使能中断

#define PCA_Int_Disable()         EIE1&=~(1<<2);   //PCA禁止中断
#define PCA_Int_Enable()          EIE1|=1<<2;      //PCA开启中断

#define  PCA_CntStop()        CR=0;       //PCA计数器停止
#define  PCA_CntStart()       CR=1;       //PCA计数器启动

void PCA_SourceClkConfig(PCA_ClkDef SourceClk);
bit PCA_ModelConfig(uint8_t Channel,PCA_Mode_def Mode );

void PCA_Cnt_Reload(uint16_t dat);




#endif

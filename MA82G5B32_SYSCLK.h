#ifndef _MA82G5B32_SYSCLK_H
#define _MA82G5B32_SYSCLK_H

#include "MA82G5B32_IAP.H"

#ifndef CKCON2
#define CKCON2   0x40
#endif

#ifndef CKCON3
#define CKCON3   0x41
#endif


//时钟源输入类型
typedef enum
{
	XTAL_CLK = 1,       //使用外部晶振做为系统时钟
	ILRCO_CLK = 3,      //使用内部32KHZ的低频RC振荡器做为系统时钟
	ECKI_CLK = 4,       //使用外部时钟从P6.0输入做为系统时钟
	IHRCO_12M = 5,      //使用内部12MHZ的RC振荡器做为系统时钟。(出厂默认)
	IHRCO_110592 = 6	//使用内部11.0592MHZ的RC振荡器做为系统时钟
}sysclk_def;

//外部晶振或内部振荡器OSCin倍频器输入选择
typedef enum
{
     OSCin_Div1 = 0,    //OSCin/1 (当 OSCin 为 5 ~ 6.5MHz
     OSCin_Div2 = 1,    //OSCin/2 (当 OSCin 为 10 ~ 13MHz)
     OSCin_Div4 = 2,    //OSCin/4 (当 OSCin 为 20 ~ 26MHz)
     OSCin_Div6 = 3     //OSCin/6 (当 OSCin 为 30 ~ 39MHz)
}OSCinDiv_def;

//CKMI倍频选择
typedef enum
{
    OSCin = 0,          //OSCin
    CKMI_Mul4 = 1,      //CKMI x 4 (ENCKM =1)
    CKMI_Mul533 = 2,    //CKMI x 5.33 (ENCKM =1)
    CKMI_Mul = 3        //CKMI x 8 (ENCKM =1)
}CkmiMul_def;


//MCK分频得到SYSCLK编程选择
typedef enum
{
    MCK_Div1 = 0,
    MCK_Div2 = 1,
    MCK_Div4 = 2,
    MCK_Div8 = 3,
    MCK_Div16 = 4,
    MCK_Div32 = 5,
    MCK_Div64 = 6,
    MCK_Div128 = 7,    
    MCK_Div256 = 0x0F,
    MCK_Div512 = 0x17,
    MCK_Div1024 = 0x1F
}MCK_Div_def;

//MCK时钟输出设置。
typedef enum
{
    MCK_Output_None = 0,
    MCK_Output_Div1 = 1,
    MCK_Output_Div2 = 2,
    MCK_Output_Div4 = 3
}MCK_Output_def;

// CPUCLK设置
typedef enum
{
    SysClk_Div1 = 0,    //SYSCLK不分频做为CPUCLK
    SysClk_Div2 =1      //SYSCLK二分频做为CPUCLK
}CPUCLK_Def;


// void Source_Clk_Select(sysclk_def sysclk);    //系统时钟切换
void OSCin_DIV_To_CKMI(OSCinDiv_def div);     //OSCin分频选择到CKMI
void CKMI_Mul_To_MCK(CkmiMul_def mul);        //CKMI倍频到MCK的输入源
void MCK_Div_To_SysClk(MCK_Div_def mul);      //MCK分频到系统时钟。
void SysClk_Div_To_CPUClk(CPUCLK_Def cpuclk); //系统时钟到CPU时钟的选择
// void Set_MCK_Outut(MCK_Output_def mod);       //P60输出MCK设置
void SysCLK_Init(void);

#endif




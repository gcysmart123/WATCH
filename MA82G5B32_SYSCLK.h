#ifndef _MA82G5B32_SYSCLK_H
#define _MA82G5B32_SYSCLK_H

#include "MA82G5B32_IAP.H"

#ifndef CKCON2
#define CKCON2   0x40
#endif

#ifndef CKCON3
#define CKCON3   0x41
#endif


//ʱ��Դ��������
typedef enum
{
	XTAL_CLK = 1,       //ʹ���ⲿ������Ϊϵͳʱ��
	ILRCO_CLK = 3,      //ʹ���ڲ�32KHZ�ĵ�ƵRC������Ϊϵͳʱ��
	ECKI_CLK = 4,       //ʹ���ⲿʱ�Ӵ�P6.0������Ϊϵͳʱ��
	IHRCO_12M = 5,      //ʹ���ڲ�12MHZ��RC������Ϊϵͳʱ�ӡ�(����Ĭ��)
	IHRCO_110592 = 6	//ʹ���ڲ�11.0592MHZ��RC������Ϊϵͳʱ��
}sysclk_def;

//�ⲿ������ڲ�����OSCin��Ƶ������ѡ��
typedef enum
{
     OSCin_Div1 = 0,    //OSCin/1 (�� OSCin Ϊ 5 ~ 6.5MHz
     OSCin_Div2 = 1,    //OSCin/2 (�� OSCin Ϊ 10 ~ 13MHz)
     OSCin_Div4 = 2,    //OSCin/4 (�� OSCin Ϊ 20 ~ 26MHz)
     OSCin_Div6 = 3     //OSCin/6 (�� OSCin Ϊ 30 ~ 39MHz)
}OSCinDiv_def;

//CKMI��Ƶѡ��
typedef enum
{
    OSCin = 0,          //OSCin
    CKMI_Mul4 = 1,      //CKMI x 4 (ENCKM =1)
    CKMI_Mul533 = 2,    //CKMI x 5.33 (ENCKM =1)
    CKMI_Mul = 3        //CKMI x 8 (ENCKM =1)
}CkmiMul_def;


//MCK��Ƶ�õ�SYSCLK���ѡ��
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

//MCKʱ��������á�
typedef enum
{
    MCK_Output_None = 0,
    MCK_Output_Div1 = 1,
    MCK_Output_Div2 = 2,
    MCK_Output_Div4 = 3
}MCK_Output_def;

// CPUCLK����
typedef enum
{
    SysClk_Div1 = 0,    //SYSCLK����Ƶ��ΪCPUCLK
    SysClk_Div2 =1      //SYSCLK����Ƶ��ΪCPUCLK
}CPUCLK_Def;


// void Source_Clk_Select(sysclk_def sysclk);    //ϵͳʱ���л�
void OSCin_DIV_To_CKMI(OSCinDiv_def div);     //OSCin��Ƶѡ��CKMI
void CKMI_Mul_To_MCK(CkmiMul_def mul);        //CKMI��Ƶ��MCK������Դ
void MCK_Div_To_SysClk(MCK_Div_def mul);      //MCK��Ƶ��ϵͳʱ�ӡ�
void SysClk_Div_To_CPUClk(CPUCLK_Def cpuclk); //ϵͳʱ�ӵ�CPUʱ�ӵ�ѡ��
// void Set_MCK_Outut(MCK_Output_def mod);       //P60���MCK����
void SysCLK_Init(void);

#endif




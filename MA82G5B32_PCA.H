#ifndef __MA82G5B32_PCA_H
#define __MA82G5B32_PCA_H

#include <reg_ma82g5b32.h>
#include "typedef.h"

//PCAʱ��Դ
typedef enum
{
    PCA_USE_SysClk_DIV12 = 0,   // �ڲ�ʱ�ӣ�(system clock)/12��ΪPCAʱ��Դ
    PCA_USE_SysClk_DIV2 = 1,    // �ڲ�ʱ�ӣ� (system clock)/2��ΪPCAʱ��Դ
    PCA_USE_TIMER0_OF = 2,      // ��ʱ�� 0 �������ΪPCAʱ��Դ
    PCA_USE_ExtInput = 3,       // ���� ECI ���ŵ��ⲿʱ����Ϊϵͳʱ��Դ
    None1            = 4,       //��������ʹ��
    PCA_USE_SysClk_DIV1 = 5,    //ϵͳʱ��(SYSCLK)��ΪPCAʱ��Դ������Ƶ
    PCA_USE_SIBRG_OF = 6,       //S1BRG���
    None2 = 7                   //��������ʹ��
}PCA_ClkDef;

//PCA����ģʽ����
typedef enum
{
	PCA_None = 0,                  	   //�޲���
	PCA_16BitUpCapture = 0X20,         //16λCEXn���������ش�������ģʽ
	PCA_16BitDownCapture = 0X10,       //16λCEXn�����½��ش�������ģʽ
	PCA_16BitCapture = 0X30,           //16λCEXn�������䴥������ģʽ
	PCA_16BitTimer = 0X48,             //16λ������ʱ��
	PCA_16BitOutput = 0X4c,            //16λ�������
	PCA_PWM_Output	= 0X42	           //8λ����������(PWM)
}PCA_Mode_def;


//PWM����ģʽ����
typedef enum
{
    PWM_Mode_8bit = 0,           //8λPWMģʽ
    PWM_Mode_10bit = 0X40,        //10λPWMģʽ
    PWM_Mode_12bit = 0X80,        //12λPWMģʽ
    PWM_Mode_16bit = 0XC0         //16λPWMģʽ
}PWM_Mode_def;

//PWM�Ŀ�ʼ��λ����
typedef enum
{
        PWM_Phase_00 =  0,    //ʹ�ܵ� PWM ͨ���� 0 �ȿ�ʼ
        PWM_Phase_90 =  0X08, //ʹ�ܵ� PWM ͨ���� 90 �ȿ�ʼ
        PWM_Phase_180 = 0X10, //ʹ�ܵ� PWM ͨ���� 180 �ȿ�ʼ
        PWM_Phase_270 = 0X18, //ʹ�ܵ� PWM ͨ���� 270 �ȿ�ʼ
        PWM_Phase_120 = 0X20, //ʹ�ܵ� PWM ͨ���� 120 �ȿ�ʼ
        PWM_Phase_240 = 0X28, //ʹ�ܵ� PWM ͨ���� 240 �ȿ�ʼ
        PWM_Phase_60 =  0X30, //ʹ�ܵ� PWM ͨ���� 60 �ȿ�ʼ
        PWM_Phase_300 = 0X38  //ʹ�ܵ� PWM ͨ���� 300 �ȿ�ʼ
}PWM_StarPhase_def;


#define PCA_Go_Idle_Mode()      CMOD&=0X7F;      //�� PCA �������ڿ���ģʽ�¼�������
#define PCA_Halt_Idle_Mode()    CMOD|=0x80;      //����ģʽ�¹ر� PCA ������

#define PCA_ECF_Disable()       CMOD&=0XFE;      //�� CF λ��CCON �Ĵ����У���λʱ��ֹ�ж�
#define PCA_ECF_Enable()        CMOD|=0X01;      //�� CF λ��CCON �Ĵ����У���λʱʹ���ж�

#define PCA_Int_Disable()         EIE1&=~(1<<2);   //PCA��ֹ�ж�
#define PCA_Int_Enable()          EIE1|=1<<2;      //PCA�����ж�

#define  PCA_CntStop()        CR=0;       //PCA������ֹͣ
#define  PCA_CntStart()       CR=1;       //PCA����������

void PCA_SourceClkConfig(PCA_ClkDef SourceClk);
bit PCA_ModelConfig(uint8_t Channel,PCA_Mode_def Mode );

void PCA_Cnt_Reload(uint16_t dat);




#endif
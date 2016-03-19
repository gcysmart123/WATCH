#include "interrupt.h"
#include "MA82G5B32_Timer.h"
#include "typedef.h"
#include "define.h"
#include "Keyproc.h"
#include "menu.h"
#include "ma82g5b32_sysclk.h"

void KBI_Init(void)
{
    PUCON0|=0x30;   //��4λ�˿ڿ�����
    KBPATN=0xFF;
    KBMASK=0X0F;
    P27=1;
    P25=1;
    P21=1;
    P20=1;
    AUXR1&=0XBF;  //KBI�˿��ڣ�P2.7, P2.5, P2.1, P2.0
    KBCON=0X00;   //KBI�˿ڲ�����0X0Fʱ�����ж�
//    EIE1|=1<<5;   //EKBI=1;KBI���ж�
}

void KBI_ISR(void)  interrupt 13
{
    KBCON=0X00;  //���־  
    if(!bPowerSet)             //�ػ���״̬�����ǿ���
    {   
        LongPressSec=0;
        while(!KEY_RETURN)
        {   ClearWatchDog();  
            if(LongPressSec>2)
            {
                POWERON_FLG=0x55AA;
                ISPCR=0X60;
            }
        } 
    }    
    else
    {
        if(!KEY_OK) 
        {   
            gKeyValue=KEY_VALUE_OK;
        }
        else if(!KEY_UP) 
        {
            gKeyValue=KEY_VALUE_UP; 
        }
        else if(!KEY_DOWN)
        {           
            gKeyValue=KEY_VALUE_DOWN;  
        }
        else if(!KEY_RETURN)
        {   
            gKeyValue=KEY_VALUE_RETURN; 
        }
    }
    EIE1&=~(1<<5);   //���ж�
}



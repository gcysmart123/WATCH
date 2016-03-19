#include "MA82G5B32_Timer.h"
#include "typedef.h"
#include "define.h"
#include "Keyproc.h"
#include "pt2240_decode.h"

/******��ʱ��0��ʱ��Դѡ��***************************

[T0XL:T0X12] = 00 :SYSCLK/12Ϊ��ʱ��0ʱ��Դ��Ĭ�ϣ�
             = 01 :SYSCLKΪ��ʱ��0ʱ��Դ
             = 10 :SYSCLK/48
             = 11 ��SYSCLK/192

**************************************************/

//��ʱ��0��ʼ������
void Timer0_Init(void)
{
    AUXR0 |= T0XL;
    AUXR2 |= T0X12;   //ѡ��ʱ��ʱ��ԴΪSYSCLK /192
//     AUXR2 |= 0X01;     //ʹ��T0��P3.4���
//    AUXR2 &= 0xFE;       //��ֹT0��P3.4���
    TMOD&=0XF0;
    TMOD|=0X02;         //��ʱ��0�����ڷ���1����16λ��ʱ��
 
    TH0=256-(6000000UL/192)/1000ul;  //1ms��ʱ
    
    ET0=1;              //��ʱ��0�����ж�
    TR0=1;
}

//��ʱ��2��ʼ������
void Timer2_Init(void)
{
    CP_RL2=0;      
//     EXEN2=1;  
    Page_P_SFR_Write(0x4c,0x00);
//     AUXR0|=0X0C;   //��ʱ��2������л�ΪP45     
//     T2MOD|=1<<1;  //T2OE=1,��ʱ�����ʹ��
    P1M0|=0X01;
//    T2MOD|=1<<4;  //TX12=1,��ʱ��2������1Tģʽ
    T2MOD&=~(1<<4);
    T2MOD &=0XFE;   //DCEN2=0,��ʱ��2���ϼ���    
    TH2=0;
    TL2=0;    
    RCAP2H=0XFD;    
    RCAP2L=0XD4;
    TR2=0;
    BUZZER=0;
}

extern int8_t bDecoding;
extern bit bSignalSet;   //�źŽ�������
static xdata int16_t LightTim=0;
xdata uint16_t DecodeTim=0;

// xdata uint16_t DelayTim=0;


// //������ʱ
// void delay_ms(uint16_t tm)
// {
//     DelayTim=tm;
//     while(tm);
// }


extern int8_t timout_ms;

xdata uint16_t TimingMs=0;

//��ʱ��0�жϷ������
void Timer0_ISR(void) interrupt 1
{
    if(!bDecoding)
    {
       DecodeTim++;
       if(DecodeTim>8) DecodeTim=0;
    }

    if(timout_ms>0)
    {
       timout_ms--;
    }    


    if(bBeep)             
    {
        if(BeepMs>0)
        {
           BeepMs--; 
        }
    
        if(0==BeepMs)       //����
        {
            TR2=0;
            T2MOD&=~(1<<1);
            bBeep=0;
            BUZZER=0;
        }       
    }

    if(bShake)             
    {   
        if(ShakeMs>0)
        {
           ShakeMs--;
        }
        if(0==ShakeMs)      //��
        {
           SHAKE=0;
           bShake=0;           
        }    
    }

    if(bLight)              //����
    {   
        LightTim++;
        if(LightTim>800)
        {
            LED=~LED;
            LightTim=0;
        }
              
        if(--LightMs<0)     //ʱ�䵽���ر�
        {
            LED=1;
            bLight=0;
            LightTim=0;
        }     
    }
    
    
    
    
    if(bPrompt) //�źŽ�����ʾ
    {
//         beepInterval++;
//         if(beepInterval==400)     //����������
//         {
//             if(MsgBeep)  Beep(200);
//         }
//         else if(beepInterval==800)
//         {
//             if(MsgBeep) Beep(200);
//             if(MsgShake) Shake(500);
//         } 
//         else if(beepInterval>2000)   
//         {
//             beepInterval=0;
//             TimingMs=0;
//             if(MsgLight) Light(2500);
//         } 

		
        beepInterval++;
        if(beepInterval==200)     //����������
        {
            if(MsgBeep)  Beep(200);
            if(MsgShake) Shake(300);
        }
        else if(beepInterval==600)
        {
			if(MsgBeep) Beep(200);
        }
        else if(beepInterval==1000)
        {
            if(MsgBeep) Beep(200);
            if(MsgShake) Shake(300);
        } 		
        else if(beepInterval>2000)   
        {
            beepInterval=0;
            TimingMs=0;
            if(MsgLight) Light(2500);
        } 		
		

        TimingMs++;
        if(TimingMs>1000)
        {
             TimingMs=0;
             PromptTim_tmp--;
             if(PromptTim_tmp<=0)
             {
                  bPrompt=0;
                  BeepMs=0;
                  ShakeMs=0;
                  ShakeMs=0;
                  LightTim=0;
             }
        }
    }    
    

    
    
    
    
    if( (0==bBeep)&&(0==bShake)&&(0==bLight) )
    {
        BUZZER=0;
        SHAKE=0;
        LED=1;
    }
}





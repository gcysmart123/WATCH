#include "MA82G5B32_Timer.h"
#include "typedef.h"
#include "define.h"
#include "Keyproc.h"
#include "pt2240_decode.h"

/******定时器0的时钟源选择***************************

[T0XL:T0X12] = 00 :SYSCLK/12为定时器0时钟源（默认）
             = 01 :SYSCLK为定时器0时钟源
             = 10 :SYSCLK/48
             = 11 ：SYSCLK/192

**************************************************/

//定时器0初始化程序
void Timer0_Init(void)
{
    AUXR0 |= T0XL;
    AUXR2 |= T0X12;   //选定时器时钟源为SYSCLK /192
//     AUXR2 |= 0X01;     //使能T0从P3.4输出
//    AUXR2 &= 0xFE;       //禁止T0从P3.4输出
    TMOD&=0XF0;
    TMOD|=0X02;         //定时器0工作于方试1，即16位定时器
 
    TH0=256-(6000000UL/192)/1000ul;  //1ms定时
    
    ET0=1;              //定时器0开启中断
    TR0=1;
}

//定时器2初始化程序
void Timer2_Init(void)
{
    CP_RL2=0;      
//     EXEN2=1;  
    Page_P_SFR_Write(0x4c,0x00);
//     AUXR0|=0X0C;   //定时器2输出脚切换为P45     
//     T2MOD|=1<<1;  //T2OE=1,定时器输出使能
    P1M0|=0X01;
//    T2MOD|=1<<4;  //TX12=1,定时器2工作在1T模式
    T2MOD&=~(1<<4);
    T2MOD &=0XFE;   //DCEN2=0,定时器2向上计数    
    TH2=0;
    TL2=0;    
    RCAP2H=0XFD;    
    RCAP2L=0XD4;
    TR2=0;
    BUZZER=0;
}

extern int8_t bDecoding;
extern bit bSignalSet;   //信号接收设置
static xdata int16_t LightTim=0;
xdata uint16_t DecodeTim=0;

// xdata uint16_t DelayTim=0;


// //毫秒延时
// void delay_ms(uint16_t tm)
// {
//     DelayTim=tm;
//     while(tm);
// }


extern int8_t timout_ms;

xdata uint16_t TimingMs=0;

//定时器0中断服务程序
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
    
        if(0==BeepMs)       //蜂鸣
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
        if(0==ShakeMs)      //振动
        {
           SHAKE=0;
           bShake=0;           
        }    
    }

    if(bLight)              //闪灯
    {   
        LightTim++;
        if(LightTim>800)
        {
            LED=~LED;
            LightTim=0;
        }
              
        if(--LightMs<0)     //时间到，关闭
        {
            LED=1;
            bLight=0;
            LightTim=0;
        }     
    }
    
    
    
    
    if(bPrompt) //信号接收提示
    {
//         beepInterval++;
//         if(beepInterval==400)     //嘀嘀嘀三声
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
        if(beepInterval==200)     //嘀嘀嘀三声
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





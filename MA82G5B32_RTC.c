#include "MA82G5B32_RTC.H"
#include "keyproc.h"
#include "date.h"
#include "menu.h"
#include "define.h"

xdata unsigned long RtcCounter;

xdata unsigned int POWERON_FLG;

/***********************************************************************
  * Function：void RTC_SourceClk_Config(rtcclk_def clock)
  *         
  * Description:  设置RTC的时钟源
  *         
  * Input:     rtcclk_def clock为RTC的时钟源
  *
  * Output:    None
  *
  * Return:    None
  **********************************************************************/
void RTC_SourceClk_Config(rtcclk_def clock)
{
    unsigned char tmp;
    RTCTM &= 0x3F;
    RTCTM |= (clock<<6);
    if( (clock==XTAL_DIV16384)||(clock==XTAL_DIV32768) )  //时钟源选择外部晶振时要把外部振荡器使能
    {
        tmp = Page_P_SFR_Read(0X40);   //把P页的CKCON2（地址：0X40）读出来
        tmp&=0X1F;
        tmp|=1<<5;    //XTALE=1,使能外部振荡器
        Page_P_SFR_Write(0X40,tmp);    
    }
}



/***********************************************************************
  * Function：void Set_RTC_Reload(unsigned char Dat6bits)
  *         
  * Description:  设置RTC的6BIT 重载值
  *         
  * Input:     6bit的重载值
  *
  * Output:    None
  *
  * Return:    None
  **********************************************************************/
// void Set_RTC_Reload(unsigned char Dat6bits)
// {
//     RTCCR &= 0XC0;
//     Dat6bits &= 0x3F;
//     RTCCR |= Dat6bits;
// }


/***********************************************************************
  * Function：unsigned char Get_RTC_Cnt(void)
  *         
  * Description:  获取RTC的计数值
  *         
  * Input:     None
  *
  * Output:    None
  *
  * Return:    RTC 6位计数值
  **********************************************************************/
// unsigned char Get_RTC_Cnt(void)
// {
//     return RTCTM&0X3F;
// }



void RTC_init(void)
{
    RTC_SourceClk_Config(XTAL_DIV32768);    //设置RTC的时钟源为使用外部的32768HZ的晶振，

//    Set_RTC_Reload(63);                    //设置RTC的6bit重载值为63,1S中断一次
    
    EIP1L|=1<<3;       //RTC为中断最高优先级
    EIP1H|=1<<3;
    RTCCR=0XBF;    //重载值为63,1S中断一次，并启动RTC    
   
    EIE1 |= ESF;              //ESF=1,开系统标志中断    
    
    if(POWERON_FLG!=0x55AA)      //软件复位不清0
    {
        SysTime.tm_year=2015;
        SysTime.tm_mon=6;
        SysTime.tm_mday=10;
        SysTime.tm_hour=00;
        SysTime.tm_min=00;
        SysTime.tm_sec=0;
        RtcCounter = mktimev(&SysTime);
    }
    to_tm(RtcCounter,&SysTime);
    
    RTC_Int_Enable();        //开RTC中断
//     RTC_Cnt_Start();         //启动RTC定时计数器
}


bit bGetTime=0;     //重新获取到时间
//RTC中断服务程序入口
void RTC_isr(void) interrupt 11
{
    if(PCON1&0X10)    //判断是否是RTC中断标志，即RTCF是否为1
    {
        RTC_Clear_Int_Flag();    //清除中断标志
                                 //1S中断一次                       
        RtcCounter++;
        bGetTime=1;              //时间刷新

        if(BkLightTim>0)        //屏幕背光灯打开时间
        {
            LCDBK=0;            //开LED背光灯
            BkLightTim--;
            {
                if(0==BkLightTim)
                {
                    LCDBK=1;    //关闭背光灯
                    bBkLight=0;
                }
            }
        }      
        
        if(MenuTim>0)          //菜单超时不工作返回
        {
            MenuTim--;
            if(0==MenuTim)
            {
                repaintMenu=1;
            }
        }

        if(!(KEY_OK&&KEY_RETURN))   //判断是否是长按
        {
            LongPressSec++;
        }        
    }
}






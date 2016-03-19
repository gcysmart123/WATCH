#include "MA82G5B32_RTC.H"
#include "keyproc.h"
#include "date.h"
#include "menu.h"
#include "define.h"

xdata unsigned long RtcCounter;

xdata unsigned int POWERON_FLG;

/***********************************************************************
  * Function��void RTC_SourceClk_Config(rtcclk_def clock)
  *         
  * Description:  ����RTC��ʱ��Դ
  *         
  * Input:     rtcclk_def clockΪRTC��ʱ��Դ
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
    if( (clock==XTAL_DIV16384)||(clock==XTAL_DIV32768) )  //ʱ��Դѡ���ⲿ����ʱҪ���ⲿ����ʹ��
    {
        tmp = Page_P_SFR_Read(0X40);   //��Pҳ��CKCON2����ַ��0X40��������
        tmp&=0X1F;
        tmp|=1<<5;    //XTALE=1,ʹ���ⲿ����
        Page_P_SFR_Write(0X40,tmp);    
    }
}



/***********************************************************************
  * Function��void Set_RTC_Reload(unsigned char Dat6bits)
  *         
  * Description:  ����RTC��6BIT ����ֵ
  *         
  * Input:     6bit������ֵ
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
  * Function��unsigned char Get_RTC_Cnt(void)
  *         
  * Description:  ��ȡRTC�ļ���ֵ
  *         
  * Input:     None
  *
  * Output:    None
  *
  * Return:    RTC 6λ����ֵ
  **********************************************************************/
// unsigned char Get_RTC_Cnt(void)
// {
//     return RTCTM&0X3F;
// }



void RTC_init(void)
{
    RTC_SourceClk_Config(XTAL_DIV32768);    //����RTC��ʱ��ԴΪʹ���ⲿ��32768HZ�ľ���

//    Set_RTC_Reload(63);                    //����RTC��6bit����ֵΪ63,1S�ж�һ��
    
    EIP1L|=1<<3;       //RTCΪ�ж�������ȼ�
    EIP1H|=1<<3;
    RTCCR=0XBF;    //����ֵΪ63,1S�ж�һ�Σ�������RTC    
   
    EIE1 |= ESF;              //ESF=1,��ϵͳ��־�ж�    
    
    if(POWERON_FLG!=0x55AA)      //�����λ����0
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
    
    RTC_Int_Enable();        //��RTC�ж�
//     RTC_Cnt_Start();         //����RTC��ʱ������
}


bit bGetTime=0;     //���»�ȡ��ʱ��
//RTC�жϷ���������
void RTC_isr(void) interrupt 11
{
    if(PCON1&0X10)    //�ж��Ƿ���RTC�жϱ�־����RTCF�Ƿ�Ϊ1
    {
        RTC_Clear_Int_Flag();    //����жϱ�־
                                 //1S�ж�һ��                       
        RtcCounter++;
        bGetTime=1;              //ʱ��ˢ��

        if(BkLightTim>0)        //��Ļ����ƴ�ʱ��
        {
            LCDBK=0;            //��LED�����
            BkLightTim--;
            {
                if(0==BkLightTim)
                {
                    LCDBK=1;    //�رձ����
                    bBkLight=0;
                }
            }
        }      
        
        if(MenuTim>0)          //�˵���ʱ����������
        {
            MenuTim--;
            if(0==MenuTim)
            {
                repaintMenu=1;
            }
        }

        if(!(KEY_OK&&KEY_RETURN))   //�ж��Ƿ��ǳ���
        {
            LongPressSec++;
        }        
    }
}






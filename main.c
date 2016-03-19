#include "MA82G5B32_SYSCLK.h"
#include "MA82G5B32_Timer.h"
#include "MA82G5B32_ADC.H"
#include "MA82G5B32_RTC.H"
#include "define.h"
#include "pt2240_decode.h"
#include "lcd12864.h"
#include "keyproc.h"
#include "interrupt.h"
#include "pt2240_decode.h"
#include "menu.h"
#include "string.h"
#include "date.h"
#include "font.h"
#include "24c16.h"

extern xdata uint16_t DecodeTim;
xdata uint32_t Decode;
xdata RF_list_def RFlst;
int8_t state=0;      //为1表示收到信号

xdata RF_list_def rf;

void Delete_All_Data(void)
{
    xdata unsigned char dofly[32] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
    unsigned int k;
    for (k = 0; k<256; k++)
    {
        EEP_WriteBytes( 0 + (k << 5), dofly, 32);                   //写入24c02
    }
}

void main(void)
{
    Page_P_SFR_Write(0X44,0X1f);    //使能2.4V复位
    
    SysCLK_Init();   //系统时钟初始化
    Timer0_Init();
    Timer2_Init();     
    initial_lcd();
    RTC_init();  
    KBI_Init();  
    pt2240_init();   //2240初始化     
    PUCON1=0X04;
    PUCON0=0XF8;
    P2M0|=1<<4;   //马达为推推输出
    P3M1|=0X01;   //i2c SCL为推勉
    SHAKE=0;  
    P1M0|=1<<5;   //LED设置为推挽 

//没有使用的IO设置为非悬浮模式，以使功耗最小  //P14,P16,P17,P23,P13, P42--P47
    Page_P_SFR_Write(0X4c,0x02);   //P44，P45为IO
    P1M0|=((1<<4)|(1<<6)|(1<<7));
    P2M0|=1<<3;
    P4M0|= 0XB0;
    P6M0|=0X03;

    bBkLight=1;    

////////////////////////////////////
    //Delete_All_Data();
///////////////////////////////////

    ADC_Init(); 
    env_init(); 
    Test_PowerOn();

    LCDBK=0;     //开LED背光灯
    LCDBK_ON(20);    
    Draw_Initialize(); 

    main_menu();
    EA=1;
 //   Beep(500);
    Shake(500);
    Light(2000);
    
    EEP_RFCnt=RF_EEPROM_Cnt();
    
    while(1)
    {   
        ClearWatchDog();  
        keyproc();
        ClearWatchDog();  
        if(bGetTime)      //时间有刷新
        {
            bGetTime=0;
            to_tm(RtcCounter,&SysTime);   //转换时间
           Diaplay_BAT();   //显示电池图标            

            if(bClockAlarm) continue;  //闹铃时不往下指示
            Refresh_Time_Priodic();
        }

        if(bSignalSet)
        {
            if(DecodeTim<8)
            {
                POWER_CS=0;
                state=PT2440_Decode(&Decode);      //解码
                if(state==1)
                {
                    DecodeTim=8;
                }
            }
            else
            {
                POWER_CS=1;
                PCON0|=0X01;  //进入空闲模式
            }
        }
        
        if(state)  //正确解到码
        { 
            state=0;			
            Add_Transmit(Decode);
            Set_Region_Key(Decode);

            if((MenuMode==0)&&(index==0))             //正常菜单设置界面
            { 
                DisplayCall(Decode);      
            }

            else if(MenuMode==2)               //显示RF接收列表界面
            { 				
                if(parse_sevice(&RFlst,Decode))
                {
                    Add_call_list(&RFlst);
                    
                    if(RFlst.SrvNo==1)         //取消
                    {
                        bPrompt=0; //提示音消除
                        RF_List_remove(CurrentIndex);                            
                    }
                    
					LCDBK_ON(15);
					Display_RF_list();
                }
            } 
        }   
        
        if((!bSignalSet)&&(index==0))      //如果信号是关的状状就进行省电模式
        {   
            ClearWatchDog(); 
            EIE1|=1<<5;     //EKBI=1;KBI开中断
            PCON0|=0X02;    //掉电进入RTC模式
        }
        if(!bPowerSet)      //关机
        {
            clear_screen();
            LCD_print(4,50,Bye[Language],0);
            Power_Down();
        }
    }
}


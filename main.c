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
int8_t state=0;      //Ϊ1��ʾ�յ��ź�

xdata RF_list_def rf;

void Delete_All_Data(void)
{
    xdata unsigned char dofly[32] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
    unsigned int k;
    for (k = 0; k<256; k++)
    {
        EEP_WriteBytes( 0 + (k << 5), dofly, 32);                   //д��24c02
    }
}

void main(void)
{
    Page_P_SFR_Write(0X44,0X1f);    //ʹ��2.4V��λ
    
    SysCLK_Init();   //ϵͳʱ�ӳ�ʼ��
    Timer0_Init();
    Timer2_Init();     
    initial_lcd();
    RTC_init();  
    KBI_Init();  
    pt2240_init();   //2240��ʼ��     
    PUCON1=0X04;
    PUCON0=0XF8;
    P2M0|=1<<4;   //���Ϊ�������
    P3M1|=0X01;   //i2c SCLΪ����
    SHAKE=0;  
    P1M0|=1<<5;   //LED����Ϊ���� 

//û��ʹ�õ�IO����Ϊ������ģʽ����ʹ������С  //P14,P16,P17,P23,P13, P42--P47
    Page_P_SFR_Write(0X4c,0x02);   //P44��P45ΪIO
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

    LCDBK=0;     //��LED�����
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
        if(bGetTime)      //ʱ����ˢ��
        {
            bGetTime=0;
            to_tm(RtcCounter,&SysTime);   //ת��ʱ��
           Diaplay_BAT();   //��ʾ���ͼ��            

            if(bClockAlarm) continue;  //����ʱ������ָʾ
            Refresh_Time_Priodic();
        }

        if(bSignalSet)
        {
            if(DecodeTim<8)
            {
                POWER_CS=0;
                state=PT2440_Decode(&Decode);      //����
                if(state==1)
                {
                    DecodeTim=8;
                }
            }
            else
            {
                POWER_CS=1;
                PCON0|=0X01;  //�������ģʽ
            }
        }
        
        if(state)  //��ȷ�⵽��
        { 
            state=0;			
            Add_Transmit(Decode);
            Set_Region_Key(Decode);

            if((MenuMode==0)&&(index==0))             //�����˵����ý���
            { 
                DisplayCall(Decode);      
            }

            else if(MenuMode==2)               //��ʾRF�����б����
            { 				
                if(parse_sevice(&RFlst,Decode))
                {
                    Add_call_list(&RFlst);
                    
                    if(RFlst.SrvNo==1)         //ȡ��
                    {
                        bPrompt=0; //��ʾ������
                        RF_List_remove(CurrentIndex);                            
                    }
                    
					LCDBK_ON(15);
					Display_RF_list();
                }
            } 
        }   
        
        if((!bSignalSet)&&(index==0))      //����ź��ǹص�״״�ͽ���ʡ��ģʽ
        {   
            ClearWatchDog(); 
            EIE1|=1<<5;     //EKBI=1;KBI���ж�
            PCON0|=0X02;    //�������RTCģʽ
        }
        if(!bPowerSet)      //�ػ�
        {
            clear_screen();
            LCD_print(4,50,Bye[Language],0);
            Power_Down();
        }
    }
}


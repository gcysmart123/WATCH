#include <intrins.h>
#include "define.h"                         
#include "typedef.h"
#include "lcd12864.h"
#include "keyproc.h"
#include "absacc.h"
#include "font.h"
#include "string.h"
#include "MA82G5B32_RTC.H"
#include "menu.h"
#include "string.h"
#include "pt2240_decode.h"
#include "MA82G5B32_ADC.H"
#include "date.h"
#include "interrupt.h"
#include "24c16.h"
#include "ma82g5b32_sysclk.h"


// xdata Alarm_clk_def AlarmClock[10];   //定时闹铃

// xdata Alarm_clk_def TimSign[10];      //定时开关信号

xdata RF_list_def RFlist[RFLIST_QTY+1];

xdata int16_t PromptTim_tmp=0;    //提示时间
xdata int8_t ClearTim=0;     //定时消号时间
xdata int8_t ClearTim_tmp=0;     //定时消号时间

xdata int8_t CyclingTim=0;   //循环时间
xdata int8_t CyclingTim_tmp=0;   //循环时间
xdata int8_t PromptTim=0;    //提示时间
xdata int8_t bPrompt=0;    //为1收到信号，提示

uint8_t index=0;    //菜单索引号

xdata uint16_t EEP_RFCnt=0;     //RF存放在EE在的数量

bit repaintMenu=0;
xdata uint8_t MenuMode=0;        //菜单显示模式

xdata int8_t ListIndex=0;
xdata int8_t ListLine=1;
xdata int8_t CurrentIndex=0;

bit bBeep=0;       //蜂鸣器是否响
bit bLight=0;      //闪灯
bit bShake=0;      //振动
bit bCharging=0;   //充电中
bit bBkLight=0;

bit bPowerDown=0;      //掉电模式
bit bClockAlarm=0;     //有闹铃

xdata int16_t LightMs=0;     //闪灯时间，毫秒
xdata uint16_t BeepMs=0;      //蜂鸣响的时间，毫秒
xdata uint16_t ShakeMs=0;     //振动时间，毫秒

xdata int16_t beepInterval=0;  //蜂鸣器间格时间

xdata uint8_t BkLightTim;
xdata uint8_t MenuTim=0;

xdata uint8_t LongPressSec;  //长按时间
uint8_t gKeyValue;           //按键按下的值
xdata int8_t KeyBoardNo=0;


xdata Env_t EEprom;   //保存在EE里的变量

xdata int8_t Count=0;

void (*Function)(void);

void delayms(unsigned int t) reentrant
{
    unsigned int  i;
    while(t--)
    {
        for(i=0;i<250;i++)
        {
            ;
        }
        ClearWatchDog();
    }
}


//主界面，显示时间    
void main_menu(void )
{ 
    clear_screen();
    if(!index_tmp)
    {    
        Test_Power_ADC();    //显示电池图标
        Display_Time();
        bSignalSet?LCD_print(7,94,"♀√",0):LCD_print(7,94,"♀×",0);   //信号图标
        MenuTimout(0);
        MenuMode=0;
        index=0;
        repaintMenu=0;
    }
    else
    {
        index=index_tmp;
        Display_Time_Line(1);
        displayRF();
        MenuMode=2;
        repaintMenu=0;
    }
    Light(0);
    LED=1;
}


unsigned char key_scan(void)
{
    uint8_t Keyval=0;
    if(!(KEY_OK&&KEY_UP&&KEY_DOWN&&KEY_RETURN))
    {   POWER_CS=1;
        delayms(10);
        if(bClockAlarm)            //闹铃在任意键取消
        {
            bClockAlarm=0;return 0;
        }
        if(!(KEY_OK&&KEY_UP&&KEY_DOWN&&KEY_RETURN))
        {
            Beep(50);
            if(!KEY_OK) Keyval=KEY_VALUE_OK;
            if(!KEY_UP) Keyval=KEY_VALUE_UP;
            if(!KEY_DOWN) Keyval=KEY_VALUE_DOWN;
            if(!KEY_RETURN) Keyval=KEY_VALUE_RETURN;         
        }  
    }
    return Keyval;
}


//按键操作处理，菜单显示
void keyproc(void)
{ 
    LongPressSec=0;
    gKeyValue=key_scan();
    if(gKeyValue)
    {
        if(bPrompt)     //如果接收提示铃声响，第一次按键选停止提示铃声
        {
//             PromptTim_tmp=0;
            bPrompt=0;
            gKeyValue=0;
            delayms(300);
        }
    }

    switch(gKeyValue)
    {
        case KEY_VALUE_OK:
            LCDBK_ON(15);        
            if(!bBkLight)      //第一次打开背光灯时不能进入菜单 
            {
                bBkLight=1;     
                LCDBK=0;
                while(!KEY_OK) ClearWatchDog();  
                break;
            }
            index = MenuProc[index].Enter;
            Function = MenuProc[index].handle;
            MenuTimout(20);
            (*Function)();
            while(!KEY_OK) ClearWatchDog();
            break;
        
        case KEY_VALUE_UP:
            LCDBK_ON(15);       
            if(!bBkLight)
            {
                bBkLight=1;LCDBK=0;
                while(!KEY_UP)ClearWatchDog();
                break;
            }
            index = MenuProc[index].Up;
            Function = MenuProc[index].handle;
            MenuTimout(20);            
            (*Function)();
            break;
        
        case KEY_VALUE_DOWN:
            LCDBK_ON(15); 
            if(!bBkLight)
            {
                bBkLight=1;LCDBK=0;
                while(!KEY_DOWN)ClearWatchDog();
                break;
            }        
            index = MenuProc[index].Down;
            Function = MenuProc[index].handle;
            MenuTimout(20);
            (*Function)();            

            break;
            
        case KEY_VALUE_RETURN:
            LCDBK_ON(15);        
            if(!bBkLight)
            {
                bBkLight=1;LCDBK=0;
                while(!KEY_RETURN)ClearWatchDog();  
                break;
            }
            else
            {
                while((!KEY_RETURN)&&(index==0))
                {
                    if( LongPressSec>2)      // 长按
                    {
                        index=63;
                        MenuMode=3;
                         Function = MenuProc[63].handle;
                        (*Function)();
                        return;
                    }
                    ClearWatchDog();                      
                }
						index = MenuProc[index].Return;
            Function = MenuProc[index].handle;
            if(index==0)
            {
                MenuTimout(0);
            }
            else
            {
                MenuTimout(20); 
            }
            (*Function)();
            delayms(80);
            break;
            }
            break;
#if 0		 /*sjz change*/							
                    if( LongPressSec>2)      // 长按
                    {
                        index=58;
                        MenuMode=3;
                         Function = MenuProc[58].handle;
                        (*Function)();
                        return;
                    }
                    ClearWatchDog();                      
                }
            }
            index = MenuProc[index].Return;
            Function = MenuProc[index].handle;
            if(index==0)
            {
                MenuTimout(0);
            }
            else
            {
                MenuTimout(20); 
            }
            (*Function)();
            delayms(80);
            break;
#endif            
        default:
            break;        
    }
}


//环境变量初始化
void env_init(void)
{
//     xdata uint8_t tmp[10];
    xdata uint8_t i;
    xdata uint8_t new_eeprom =0 ;
    xdata unsigned char servicetype[12];
     xdata uint8_t Key[KEY_NUM];
    if(POWERON_FLG!=0x55AA)    // 重启或者是重新开机时
    {
//         memset(&AlarmClock,0,10*sizeof(Alarm_clk_def));  //闹铃清0
//         memset(&TimSign,0,10*sizeof(Alarm_clk_def));     //定时开关信号清0 
    }    
    memset(&RFlist,0,(RFLIST_QTY+1)*sizeof(RF_list_def));    //删除RF接收链表
    
    EEP_ReadBytes(0x0000,&EEprom,sizeof(Env_t));
    
    Language=0;//EEprom.language;
    WorkMode=EEprom.workmode;
    ClearTim=EEprom.cleartim;
    CyclingTim=EEprom.cyclingtim;
    PromptTim=EEprom.prompttim;
    KeyBoardNo=EEprom.keyboardno;
    DspMode=EEprom.dspmode;
    MsgBeep=EEprom.prompBeep;
    MsgShake=EEprom.promptShake;
    MsgLight=EEprom.promptLight;
    
    if(EEprom.language>1)
    {
       Language=EEprom.language=0;     //语言
    }

    if(EEprom.workmode>5)
    {    
        WorkMode=EEprom.workmode=1;    //呼叫器模式
    }
    
    if(EEprom.cleartim>99)
    {
       ClearTim=EEprom.cleartim=0;      //定时消号时间
    }
    
    if(EEprom.cyclingtim>99)
    {
       CyclingTim=EEprom.cyclingtim=0;  //循环显示时间
    }
    
    if(EEprom.prompttim>99)                 
    {
       PromptTim=EEprom.prompttim=5;    //呼叫提示时间
    }    
    
    if(EEprom.dspmode>1)
    {
       DspMode=EEprom.dspmode=0;        //显示模式
    }
    
    if(EEprom.keyboardno>10)
    {
       KeyBoardNo=EEprom.keyboardno=10;   //键盘
    }

    if(MsgBeep>1)
    {
       EEprom.prompBeep=MsgBeep=1;
    }
    
    if(MsgShake>1)
    {
       EEprom.promptShake=MsgShake=1;
    }
    if(MsgLight>1)
    {
       EEprom.promptLight=MsgLight=1;
    }    

    EEP_WriteBytes(0x0000,&EEprom,sizeof(Env_t)); 
#if 1
   EEP_ReadBytes((uint16_t)SERVICE_TYPE_EEPROM_ADDR,servicetype,12);
/*
    for(i=0;i<10;i++)  //20160308
    {
           if(((servicetype[i]>'A')&&(servicetype[i]<'Z') )||((servicetype[i]>'a')&&(servicetype[i]<'z') ))
           {
               new_eeprom =0;
           }
           else
           {
               new_eeprom =1;
	        break;
           }
    }
*/
    if(servicetype[0] == 0xff)
    {
        new_eeprom = 1;
    }
    if(new_eeprom == 1)
    {
      for(i=0;i<40;i++)  //20160308
      {
    	  EEP_WriteBytes((uint16_t)SERVICE_TYPE_EEPROM_ADDR+(i<<4),KeySevice[0][(uint8_t)i],12);   
      }
    }
#endif
/*set the key value

call   08
water  04
bill   02
cancel   01
order    03
Service            15

        Key[gSrvNo]=rfKey;  
		
        
*/
#if 1
	Key[0]=8;
       Key[6]=4;
       Key[3]=2;
	   Key[1]=1;
	   Key[5]=3;
	   Key[9]=15;
      Store_Key_Fun('0',Key);  /*only 0 region*/
#endif
}




//显示呼叫菜单
void DisplayCall(uint32_t dat)
{
    xdata RF_list_def rf;  

    if(parse_sevice(&rf,dat))    //收到无线码，并解析正确
    { 
//         if(BANK!=WorkMode)
//         {
            if(rf.SrvNo==1) return;     //取消键，不保存
//         }    

        Add_call_list(&rf);
        Display_Call(3,0,1);
        Display_Call(5,1,0);
        Display_Call(7,2,0);
        index=55;
        MenuMode=2;    //切换界面到显示RF列表
        index_tmp=54;
        LCDBK_ON(15);
    }   
}



void Display_Time_Line(uint8_t line)
{
//20XX-MM-DD HH:MM   
    char str[12];
    {
        byte_to_str(SysTime.tm_mday,&str[0]); 
        str[2]=0x2f; 
        byte_to_str(SysTime.tm_mon,&str[3]);
        str[5]=0x2f;
        int_to_str(SysTime.tm_year,&str[6]);
        str[10]=0;    
        print_str8x5(line,1,str,0);
    } 

    byte_to_str(SysTime.tm_hour,&str[0]); 
    str[2]=':';
    byte_to_str(SysTime.tm_min,&str[3]);  
    str[5]=0;     
    print_str8x5(line,66,str,0);    
}


//解析无线接收，保存到列表
int8_t parse_sevice(RF_list_def *rf,uint32_t dat)
{
    xdata RF_def RFtmp;
    uint8_t i;
    xdata uint8_t EEkey[KEY_NUM];

    uint8_t key=(uint8_t)(dat&0x0f);
    uint8_t KB=0;
    
    static xdata uint32_t LastTimeCall=0;
    static xdata uint32_t LastRFaddr=0;

    if(dat==LastRFaddr)
    {
        if(RtcCounter<LastTimeCall+3)
        {
           return 0;
        }
    }
    LastTimeCall=RtcCounter; 
    LastRFaddr=dat;


//     if((int8_t)((dat>>20)&0xf)==KeyBoardNo)
//     {
//         rf->cnt=0;
//         rf->Tim = RtcCounter;
//         
//         if( (((dat>>16)&0x0f)<10)&&(((dat>>12)&0x0f)<10)&&(((dat>>8)&0x0f)<10) )
//         {
//             rf->region[0]=((dat>>20)&0x0f)+'0';
//             rf->region[1]=((dat>>16)&0x0f)+'0';
//             rf->region[2]=((dat>>12)&0x0f)+'0';
//             rf->region[3]=((dat>>8)&0x0f)+'0';
//             
//             Find_Key_Fun(rf->region[0],EEkey); 
//             rf->SrvNo=0;                      //默认呼叫代码为0
//             for(i=0;i<KEY_NUM;i++)                  //查找呼叫代号，找不到使用默认值为0
//             {
//                 if(EEkey[i]==key)
//                 {
//                     rf->SrvNo=i;
//                     break;
//                 }
//             }
//             
//             bPrompt=1;    //蜂鸣，振动，LED提示
//             PromptTim_tmp=PromptTim/2;
//             
//             if(MsgBeep) Beep(300);
//             if(ShakeMs) Shake(300);
//             if(MsgLight) Light(3000);
//             return 1; 
//         }        
//     }
                        
    if(KeyBoardNo!=10)
    {

        if( (((dat>>16)&0x0f)<10)&&(((dat>>12)&0x0f)<10)&&(((dat>>8)&0x0f)<10) )
        {   
            KB=(int8_t)((dat>>20)&0xf);
            if( (KB==KeyBoardNo) ||((KeyBoardNo==11)&&(KB<10)) )
            {
                rf->cnt=0;
                rf->Tim = RtcCounter;

                rf->region[0]=((dat>>20)&0x0f)+'0';
                rf->region[1]=((dat>>16)&0x0f)+'0';
                rf->region[2]=((dat>>12)&0x0f)+'0';
                rf->region[3]=((dat>>8)&0x0f)+'0';
                
                Find_Key_Fun(rf->region[0],EEkey); 
                rf->SrvNo=0;                            //默认呼叫代码为0
		
                for(i=0;i<KEY_NUM;i++)                  //查找呼叫代号，找不到使用默认值为0
                {
                    if(EEkey[i]==key)
                    {
                        rf->SrvNo=i;
                        break;
                    }
                }
                if(rf->SrvNo==1)         //取消
                {
                    return 1;   
                } 
                
                
                bPrompt=1;    //蜂鸣，振动，LED提示

                if(PromptTim ==0 )  
                {
                    PromptTim_tmp=0x7fff;   //提示时间PromptTim=0时，无限提示
                }
                else
                {
                    PromptTim_tmp=PromptTim/2;
                }
                
                if(MsgBeep) Beep(200);
                if(MsgShake) Shake(400);
                if(MsgLight) Light(3000);
                return 1; 
            }        
        }
    }    

    if(Find_RF_EEPROM(&RFtmp ,dat))      //查找一个RF在EE中的位置p为存放查找到的地址，dat为入RF表
    {  
         	
        Find_Key_Fun(RFtmp.region[0],EEkey);    

        memcpy(rf->region,RFtmp.region,4);        
        rf->cnt=0;
        rf->Tim = RtcCounter;             //获取呼叫的时间
        rf->SrvNo=0;                      //默认呼叫代码为0

        for(i=0;i<KEY_NUM;i++)                  //查找呼叫代号，找不到使用默认值为0
        {
            
            if(EEkey[i] == key)
            {
                rf->SrvNo=i;
                break;
            }
        }

        if(rf->SrvNo==1)         //取消
        {
            return 1;   
        }

//         bPrompt=1;    //蜂鸣，振动，LED提示
//         PromptTim_tmp=PromptTim/2;

        bPrompt=1;    //蜂鸣，振动，LED提示

        if(PromptTim ==0 )  
        {
            PromptTim_tmp=0x7fff;   //提示时间PromptTim=0时，无限提示
        }
        else
        {
            PromptTim_tmp=PromptTim/2;
        }
        
        if(MsgBeep) Beep(200);
        if(MsgShake) Shake(400);
        if(MsgLight) Light(3000);
        
        return 1;            
    }
    return 0;
}



//增加呼叫服务列表,使用先进去的存在首位
void Add_call_list(RF_list_def *rf)
{
    int8_t i,j;
    for(i=0;i<RFLIST_QTY;i++)     //共存有多少数据在链表里
    {
        if(RFlist[i].use==0)
        {
            i+=1;
            break;
        }
    }
    j = i;
 
    if(0==DspMode)   //即时显示
    {
     
        for(i=0;i<j;i++)             //查看已经存有的是否有相同条目
        {   
            if( *(volatile uint32_t *)(rf->region) == *(volatile uint32_t *)(RFlist[i].region))  //查找是否有相等
            {  
                rf->cnt=RFlist[i].cnt;
                for(;i>0;i--)
                {
                    memcpy(&RFlist[i],&RFlist[i-1],sizeof(RF_list_def));  //有相同条目取出来放到首位
                }
                goto ENDADD;
            }    
        }
        j--;
        for(;j>0;j--)
        {
            memcpy(&RFlist[j],&RFlist[j-1],sizeof(RF_list_def));    //链表里没有相同的条件，往下存
        }
            
    ENDADD:
        rf->use=1;
        rf->cnt++;
        memcpy(&RFlist[0],rf,sizeof(RF_list_def)); 
        CurrentIndex=0;
    }
    
    else     //排队显示
    {
        for(i=0;i<RFLIST_QTY;i++)
        {
            if(RFlist[i].use==0)
            {
                rf->use=1;
                rf->cnt+=1;
                memcpy(&RFlist[i],rf,sizeof(RF_list_def)); 
                CurrentIndex=i;
                break;
            }
            else if( *(volatile uint32_t *)(rf->region) == *(volatile uint32_t *)(RFlist[i].region))  //查找是否有相等
            {
                rf->cnt=RFlist[i].cnt+1;
                rf->use=1;
                memcpy(&RFlist[i],rf,sizeof(RF_list_def));      //有相等的覆盖
                CurrentIndex=i;                                                         
                break;
            }
        }
    }
}

//计算列表里存有多少条目
uint8_t RF_list_cnt(void)
{
    uint8_t i,cnt=0;
    for(i=0;i<RFLIST_QTY;i++)
    {
        if(RFlist[i].use==1)
        {
            cnt++;
        }  
    }
    return cnt;
}






//进入掉电模式
void Power_Down(void)
{
    delayms(2000);
    
    POWERON_FLG=0x55AA;
    bPowerSet=0;
    index=0;
    MenuMode=0;   
    EIE1|=1<<5;   //EKBI=1;KBI开中断
    P2M0|=1<<2;    
    P1=0XFF;
    P2=0XFF;
    P3=0XFF;
    P4=0XFF;
    P5=0XFF;
    P6=0XFF; 
    ET0=0;
    TR0=0;    
    BUZZER=0;
    SHAKE=0;
    LCD12864_CS=0;
    LCD12864_RST=0;
    LCDBK=1; 
    BkLightTim=0;
    EA=1;
    while(1)
    {   ClearWatchDog(); 
        if(bPowerSet) break;
        EIE1|=1<<5;   //EKBI=1;KBI开中断
        PCON0|=0X02; 
    }   
}


//绘制电池电量标志
void BAT_sign(void)
{
    Fill_one_line(1,110,0x3c,2);
    Fill_one_line(1,112,0x42,15);
    Fill_one_line(1,127,0x7e,1);
}


xdata uint16_t ADCTim=0;

code char *TEXT2[1]={
    "Low power!     please charge",
};


//电压精度为0.00224V,充电输入电压为2.22V(990)，电压满电压为2.1V(935)，低压为1.8V（805）
//3.4V=750，关机，760低电。满电：900
#define     FULL_VOL       896u        //满电压   1.95V（电阻分压之后）
#define     LOW_VOL        776u        //低电压   
#define     OFF_VOL        776u        //关机电压

#define     PER_VOL        ((FULL_VOL-OFF_VOL)/15u)     //每个格代表多少个二进制电压

//检测电压，低电就关机
void Test_PowerOn(void)
{
    uint16_t adc_bat;
    uint8_t i;
    ADCON0|=0X80;    
    for(i=0;i<20;i++);
    adc_bat = GetADC_Value(2);   //P12电池电压，P11为USB电压。
    ADCON0&=0XEF; 
    if(adc_bat<(OFF_VOL-50) )    //低电时不给开机
    {
        Power_Down();
    }
}

void Diaplay_BAT(void)
{
   char str[10];
    static uint8_t prompt=0;
    
    uint8_t j;
    uint16_t adc_usb;
    uint16_t adc_bat;

    if(bShake||bBeep) return;    //蜂鸣器响或振动开启时不测试AD

    if(((MenuMode==0)&&(index==0))||(MenuMode==2)||(MenuMode==3)) 
    {                                                                              
        ADCON0|=0X80;
        for(j=0;j<10;j++);
        adc_bat = GetADC_Value(2);   //P12电池电压，P11为USB电压。
        adc_usb = GetADC_Value(1);   //根据实际调整
        ADCON0&=0XEF;                                        

        if(adc_usb<OFF_VOL)   //USB没插电
        {
            if(adc_bat<LOW_VOL)
            {
                if((prompt==0)||(prompt==30))
                {
                    clear_screen();
                    LCD_print(4,1,TEXT2[Language],0);
                    Shake(500);
                    delayms(2000);
                    main_menu();
                }
                prompt++;
                if(prompt>=60)      //电量低于一定程度，直接休眼
                {   
                    prompt=0;
                    clear_screen();
                    LCD_print(4,30,Bye[Language],0);
                    Power_Down();
                }
            }
       }
       else
       {
           prompt=0;
       }
                      
//         int_to_str(adc_bat,str);      //Debug时打开显示电压值
//         print_str8x5(2,105,str,0);
//         int_to_str(adc_usb,str);
//         print_str8x5(3,105,str,0);
        
        Test_Power_ADC();    
    }
}

//测试电量ADC。
void Test_Power_ADC(void)
{
    static uint8_t i=15;
    uint8_t j;
    uint16_t adc_usb;
    uint16_t adc_bat;
    
    ADCON0|=0X80;
    for(j=0;j<10;j++);
    adc_bat = GetADC_Value(2);   //P12电池电压，P11为USB电压。
    adc_usb = GetADC_Value(1);   //根据实际调整
    ADCON0&=0XEF;      
    
    j=(adc_bat-OFF_VOL)/PER_VOL;   //共有15格,每格为8个电压值        
    if(j>14) j=15;
    
    BAT_sign();      
    if(adc_usb>adc_bat)
    {
        if(adc_bat>FULL_VOL-5)
        {
            Fill_one_line(1,112,0x7e,15);
        }
        else
        {
            i--;
            if(i<=0) i=j;
            Fill_one_line(1,111+i,0x7e,16-i);
        }    
    }
    else if(adc_bat>OFF_VOL)
    {
         Fill_one_line(1,127-j,0x7e,j);
    }    
}




code char *InitTex[2]={
 " \xd5\xfd在初始化",
 "initialize"
};

//绘制开机图形，正在初始化  
void Draw_Initialize(void)
{
//     uint8_t i;

    LCD_print(1,1,"   wireless   ",0);   
    LCD_print(3,1,"calling system",0);
    LCD_print(5,1,"   V20160101 ",0);
    delayms(2000);
    
//     LCD_print(5,20,InitTex[Language],0);   
//     for(i=0;i<20;i++)
//     {
//         Fill_one_line(7,24+i*4,0xff,3);  
//         delayms(30);
//     }  
}


//周期性的刷新时间
void Refresh_Time_Priodic(void)
{
    uint8_t RFlistCnt = RF_list_cnt();
//     if(SysTime.tm_sec==0)
//     {
//         if(ClearTim>0)
//         {
//             ClearTim--;
//             if(ClearTim==0)
//             {
//                 repaintMenu=1;
//                 MenuMode=0;
//                 index=0;
//                 memset(RFlist,0,sizeof(RF_list_def)*RFLIST_QTY);
//             }       
//         }
//     }  
     
     
    if(repaintMenu==1)
    {
         main_menu();
    }     
 
     
     if(MenuMode==0)
     {
        if(MenuTim==0)
        {
             Display_Time();
        }
     }
     else if(MenuMode!=1)
     {
          Display_Time_Line(1);
     }
     
       
    if(MenuMode==2)    //循环显示，定时消号，
    {
          if( (ClearTim<CyclingTim)||(CyclingTim==0) )
          {
              if(ClearTim>0)
              {
                  ClearTim_tmp++;
                  if(ClearTim_tmp>ClearTim)
                  {
                       ClearTim_tmp=0;
                       bPrompt=0;               //消号即停止提示音
                       menu_RF_del();
                  }
              }
          }
          if( (ClearTim>CyclingTim)||(ClearTim==0) )
          {
             if(CyclingTim>0)
             {
                 CyclingTim_tmp++;
                 if(CyclingTim_tmp>CyclingTim)
                 {
                      CyclingTim_tmp=0;
                      
                      if(RFlistCnt>3 )
                      {
    //                       ListLine=0;
                          menu_RF_down();                       
                      }
                 }
             }
         }         
    }
}

//闹铃监控
uint8_t AlarmTim=0;

// void Alarm_Detect(void)
// {
//     if(bClockAlarm)
//     {
//         Beep(300);
//         AlarmTim++;
//         clear_screen();
//         Display_Time_Line(1);
//         Language==0?LCD_print(5,56,"闹铃",0):LCD_print(5,56,"Alarm",0);
//         repaintMenu=1;
//         if((gKeyValue!=0)||(AlarmTim>60))   //1min内无按键退出闹铃
//         {
//             bClockAlarm=0;  //任意键取消闹铃
//             AlarmTim=0;
//         }
//     }
// }


// //删除一个RF在EEPROM中的存储
// int8_t Del_RF_EEPROM(uint8_t *p)
// {
//     xdata RF_def RFtmp;
//     xdata uint16_t addr=RF_EEPROM_BASE_ADDR;
//     int16_t i;
//     for(i=0;i<RF_COUNTER;i++)
//     {
//         EEP_ReadBytes(addr,(uint8_t *)&RFtmp,sizeof(RF_def));
//         if(!(RFtmp.rf&0xff000000ul))
//         {
// 			if(*(volatile uint32_t*)p==*(volatile uint32_t*)(RFtmp.region))
//             {
//                 memset(&RFtmp,0xff,sizeof(RF_def));
//                 EEP_WriteBytes(addr,(uint8_t *)&RFtmp,sizeof(RF_def));
//                 EEP_RFCnt--;
//                 break;
//             }
//         }
//         addr+=sizeof(RF_def);      
//     }
//     return 0;
// }


//删除全部EEPROM里的RF
void Del_all_RF_EEPROM(void) 
{  
//     int8_t i;
//     xdata int8_t buf[128];
//     uint16_t addr;
//     addr=RF_EEPROM_BASE_ADDR;
//     memset(buf,0xffu,sizeof(buf));
//     Fill_one_line(5,10,0xff,56);
//     for(i=0;i<56;i++)
//     {
//         EEP_WriteBytes(addr,buf,sizeof(buf));
//         addr+=sizeof(buf);
//         Fill_one_line(5,9+(56-i),0,1);
//     }
//     EEP_RFCnt=0;



    uint8_t i;
    xdata int8_t buf[32];
    uint16_t addr;
    addr=RF_EEPROM_BASE_ADDR;
    memset(buf,0xffu,sizeof(buf));
    Fill_one_line(5,10,0xff,52);
    for(i=0;i<125;i++)
    {
        EEP_WriteBytes(addr,buf,sizeof(buf));
        addr+=sizeof(buf);
        Fill_one_line(5,6+(56-i/2),0,1);
    }
    EEP_RFCnt=0;
}


// void Erase_all_EEPROM(void) 
// {  
// //     int8_t i;
// //     xdata int8_t buf[128];
// //     uint16_t addr=0;
// //     memset(buf,0xff,sizeof(buf));
// //     Fill_one_line(5,1,0xff,64);
// //     for(i=0;i<64;i++)
// //     {
// //         EEP_WriteBytes(addr,buf,sizeof(buf));
// //         addr+=sizeof(buf);
// //         Fill_one_line(5,0+(64-i),0,1);
// //     }
//     
//     
//     int16_t i;
//     xdata int8_t buf[32];
//     uint16_t addr=0;
//     memset(buf,0xff,sizeof(buf));
//     Fill_one_line(5,1,0xff,64);
//     for(i=0;i<256;i++)
//     {
//         EEP_WriteBytes(addr,buf,sizeof(buf));
//         addr+=sizeof(buf);
//         Fill_one_line(5,64-i/4,0,1);
//     }     
// }



//共存有多少个RF在EE里
uint16_t RF_EEPROM_Cnt(void)
{
    RF_def RFtmp;
    uint16_t addr;
    uint16_t i,n=0;
    addr = RF_EEPROM_BASE_ADDR;
    for(i=0;i<RF_COUNTER;i++)
    { 
        EEP_ReadBytes(addr,(uint8_t *)&RFtmp,sizeof(RF_def));
        if(!(RFtmp.rf&0xff000000ul))    
        {
           n++; 
        }
        addr+=sizeof(RF_def);
        ClearWatchDog(); 
    }
    return n;    
}

//查找对应的区号里的编号有没有存相关数据,有返回地址
// uint16_t Find_RF_NO_inEE(uint8_t *rgn)
// {
//     xdata char buf[512];
//     RF_def *RFtmp;
//     uint16_t addr;
//     int16_t i,j;
//     addr=RF_EEPROM_BASE_ADDR;
//     for(i=0;i<14;i++)
//     {
//         EEP_ReadBytes(addr,buf,sizeof(buf));
//         RFtmp=(RF_def *)buf;
//         for(j=0;j<72;j++)
//         {
//             if(!(RFtmp->rf&0xff000000))
//             {
//                 if(*(volatile uint32_t*)(RFtmp->region) == *(volatile uint32_t*)rgn)    
//                 {
//                    return addr; 
//                 }  
//             }
//             RFtmp++;
//             addr+=sizeof(RF_def);
//         }
//         ClearWatchDog();
//     }        
//     return 0;       
// }

//查找一个RF在EE中的位置，dat为RF地址码
int8_t Find_RF_EEPROM(RF_def *p ,uint32_t dat)
{
    RF_def RFtmp;
    uint16_t addr;
    uint16_t i;
    addr=RF_EEPROM_BASE_ADDR;

    for(i=0;i<RF_COUNTER;i++)
    {
        EEP_ReadBytes(addr,(uint8_t *)&RFtmp,sizeof(RF_def));
        
        if(WorkMode==MULTI_KEY)      //多键模式低位为0
        {
            RFtmp.rf&=0x00fffff0;
            dat&=0x00fffff0;
        } 
        
        if(RFtmp.rf == dat) 
        {
            memcpy(p,&RFtmp,sizeof(RF_def));
            return 1;
        }
        addr += sizeof(RF_def);
        if(i>(EEP_RFCnt+50))  break;  //加快速度
        ClearWatchDog();
    }
    return 0;
}

extern  xdata int8_t NumTmp[5];
extern  xdata int8_t NumIndex;
 //存储一个RF列表到EE
int8_t Store_RF_TO_EEPROM(RF_def *pRF)   
{
    uint16_t addr;
    uint16_t i;
    RF_def RFtmp;
    uint16_t tmp=0;
    addr=RF_EEPROM_BASE_ADDR;
    

    for(i=0;i<RF_COUNTER;i++)
    {
        EEP_ReadBytes(addr,(uint8_t *)&RFtmp,sizeof(RF_def));

        if(!(RFtmp.rf&0xff000000))  
        {
        
            if(WorkMode==MULTI_KEY)      //多键模式低位为0
            {
                RFtmp.rf&=0x00fffff0;
                pRF->rf&=0x00fffff0;
            }         
        
            if(RFtmp.rf == pRF->rf)    //有相等RF在里面，重新覆盖
            {
                memcpy(NumTmp,RFtmp.region,4);
                NumTmp[4]=0;
                LCD_print(3,32,NumTmp,0);
                Beep(200);
                return 1;
            }
        }
        else
        {
            if(!tmp) tmp=addr;
        }
        addr += sizeof(RF_def);
        ClearWatchDog();
    }
    EEP_WriteBytes(tmp,(uint8_t *)pRF,sizeof(RF_def));
    EEP_RFCnt++;   //数量加1
    Beep(500);
    return 0;        
}






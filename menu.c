#include "define.h"                         
#include "typedef.h"
#include "lcd12864.h"
#include "keyproc.h"
#include "absacc.h"
#include "font.h"
#include "string.h"
#include "MA82G5B32_RTC.H"
#include "menu.h"
#include "pt2240_decode.h"
#include "date.h"
#include "24c16.h"
static xdata uint8_t menuLine=1;  //当前反白所在行


xdata uint8_t RF_region=0;  //RF区号
xdata uint16_t RF_No;        //RF

xdata int8_t menu_arg=0;      //上级菜单向下级传递参数保存
xdata int8_t WorkMode;     //工作模式

xdata int8_t DspMode=0;  //显示模式，0即时模式，1排队模式

xdata int8_t index_tmp=0;
xdata int8_t index_tmp2=0;

int8_t gSrvNo=0;    //服务编号
int8_t Language=0;  //语言

bit bSignalSet=1;   //信号接收设置
bit bAlarmSet=0;
bit bPowerSet=1;

int8_t MsgBeep=1;     //收到呼叫是否蜂鸣,默认使能的
int8_t MsgLight=1;    //收到呼叫是否闪灯
int8_t MsgShake=1;    //收到呼叫是否振动


//修改电话号码
xdata int8_t NumTmp[5];
xdata int8_t serviceTmp[12];
xdata int8_t NumIndex=0;


static xdata char str[20];       //这个当成局部变量


code char *Title[1][14]={
    {
        "setting",
        "add caller",     
        "delete caller",     
        "key function",
        "work mode",  
        "display mode",
        "prompt mode",
        "prompt timing",
        "remove timing",
        "cycle timing",
        "date & time",
        "keyboard set",
        "language",
        "factory reset"           
    }
};


//循环显示时间
void Display_Time(void)
{
//     unsigned char str[8];
    //YYYY-MM-DD
    Display_Time_Line(1);

    byte_to_str(SysTime.tm_hour,str);
    str[0]-='0';
    str[1]-='0';
    
    byte_to_str(SysTime.tm_min,&str[2]);
    str[2]-='0';
    str[3]-='0';

    display_graphic_24x12(3,8,ASCII_12X16[str[0]]);      //XX:XX  时间
    display_graphic_24x12(3,25,ASCII_12X16[str[1]]);
    display_graphic_24x12(3,42,ASCII_12X16[10]);         //:

    display_graphic_24x12(3,59,ASCII_12X16[str[2]]);
    display_graphic_24x12(3,76,ASCII_12X16[str[3]]);
    

    LCD_print(7,12,WeekDay[Language][SysTime.tm_wday],0);
    
    byte_to_str(SysTime.tm_sec,str);
    print_str8x5(5,91,str,0);
}

void Menu1_Display(void)
{
         
    LCD_print_line(1,32,Title[Language][0],0);     //头行显示 “系统设置”    
 
    byte_to_str(index,str);
    print_str8x5(2,98,str,0); 
    print_str8x5(2,111,"/13",0);
    
    if(gKeyValue==KEY_VALUE_UP)
    {
        menuLine--;if(menuLine<1) menuLine=1;
    }
    else if(gKeyValue==KEY_VALUE_DOWN)
    {
        menuLine++;if(menuLine>3) menuLine=3;
    }
#if 0     //sjz 20160305
    LCD_print_line(3,1,Title[Language][index+1-menuLine],!(menuLine-1));
    LCD_print_line(5,1,Title[Language][index+2-menuLine],!(menuLine-2)); 
    LCD_print_line(7,1,Title[Language][index+3-menuLine],!(menuLine-3));     
#else
	LCD_print_line(3,10,Title[Language][index+1-menuLine],0);
	LCD_print_line(5,10,Title[Language][index+2-menuLine],0); 
       LCD_print_line(7,10,Title[Language][index+3-menuLine],0);     
    if(menuLine == 1)
    {
       LCD_print(3,1,"*",0);
    }
    else if (menuLine == 2)
    {
    	LCD_print(5,1,"*",0);
    }
    else if (menuLine == 3)
    {
    	LCD_print(7,1,"*",0);
    }	
#endif
    delayms(60);  
}

//呼叫器注册
void menu1(void ) 
{
    menu_arg=0;
    MenuMode=1;
    menuLine=1;
    LCD_print_line(1,32,Title[Language][0],0);     //头行显示 “系统设置”
    print_str8x5(2,98,"01/13",0);
    //LCD_print_line(3,1,"*",0);
    LCD_print_line(3,10,Title[Language][index],0);  //sjz 20160305
    LCD_print(3,1,"*",0);
    LCD_print_line(5,10,Title[Language][index+1],0);
    LCD_print_line(7,10,Title[Language][index+2],0);

    memset(NumTmp,'0',4);
    NumTmp[3]='1';
    NumTmp[4]=0;
    NumIndex=2;
    
    delayms(60);
}

//删除呼叫器
void menu2(void)
{
    Menu1_Display();
    NumIndex=2;
}

void menu3(void)
{
    Menu1_Display();
    menu_arg=0;
    memset(NumTmp,'0',4);
    NumTmp[4]=0;    
}

void menu4(void)
{
    Menu1_Display();
    menu_arg=0;    
}

void menu5(void)
{
    Menu1_Display();
    menu_arg=0;    
}


void menu6(void)
{
    Menu1_Display();
    menu_arg=0;    
}


void menu7(void)
{ 
    Menu1_Display();
    menu_arg=0;
}


void menu8(void)
{
    Menu1_Display();
    menu_arg=0;    
}


void menu9(void)
{
    Menu1_Display();
    menu_arg=0;
}


void menu10(void)
{
    Menu1_Display();
}


void menu11(void)
{
    Menu1_Display();
}


void menu12(void)
{
    Menu1_Display();
}

// void menu13(void)
// {
//     Menu1_Display();
// }


void menu14(void)
{
    
    LCD_print_line(1,32,Title[Language][0],0);     //头行显示 “系统设置”
    print_str8x5(2,98,"13/13",0);
    LCD_print_line(3,9,Title[Language][index-2],0);
    LCD_print_line(5,9,Title[Language][index-1],0);
    
    LCD_print_line(7,10,Title[Language][index],0);  /*sjz 20160305*/
    LCD_print(7,1,"*",0);
    menuLine=3;
    delayms(60);
    
}

//整形转为字符串形
void int_to_str(uint16_t dat,char *str)
{
    uint16_t tmp;
    if(dat>9999) dat=0;
    str[0]=dat/1000+'0';
    tmp=dat%1000;
    str[1]=tmp/100+'0';
    tmp=tmp%100;
    str[2]=tmp/10+'0';
    str[3]=tmp%10+'0';
    str[4]=0;
}

//字符串转整形
uint16_t str_to_int(char *str)
{
    uint16_t tmp=0; 
//     tmp=(str[0]-'0')*1000;
    tmp+=(str[1]-'0')*100;
    tmp+=(str[2]-'0')*10;
    tmp+=(str[3]-'0');
    return tmp;
}


//字节转为字符串
void byte_to_str(uint8_t dat,char *str)
{
    str[0]=dat/10+'0';
    str[1]=dat%10+'0';
    str[2]=0;
}

static xdata struct rtc_time Tim;

void DisplayHMS(void)   //显示时分秒
{

    RtcCounter = mktimev(&Tim);
    to_tm(RtcCounter,&SysTime); 

    LCD_print(4,44,":",0);        //时
    LCD_print(4,76,":",0);        //分
   LCD_print(4,110,":",0);     //秒
   
    byte_to_str(Tim.tm_hour,str);
    LCD_print(4,24,str,0);    
    byte_to_str(Tim.tm_min,str);
    LCD_print(4,56,str,0);
    byte_to_str(Tim.tm_sec,str);
    LCD_print(4,88,str,0);
    
     Display_Time_EndLine();
}

//二极菜单，时间日期设置---> 时
void menu_4_sub1_1(void ) //
{   
    RTC_Cnt_Stop();    
    to_tm(RtcCounter,&Tim);
    if(gKeyValue==KEY_VALUE_OK)
    {
        clear_screen();    
        LCD_print_line(1,10,"Date&Time set",0);       //显示时间日期设置 
    }    
    
    if(gKeyValue==KEY_VALUE_UP)
    {
        Tim.tm_hour++;
        if(Tim.tm_hour>23)
        {
            Tim.tm_hour=0;
        }
    }
    else if(gKeyValue==KEY_VALUE_DOWN)
    {
        Tim.tm_hour--;
        if(Tim.tm_hour<1)
        {
            Tim.tm_hour=23;
        }
    }
 
    DisplayHMS(); 
    byte_to_str(SysTime.tm_hour,str);
    LCD_print(4,24,str,1);
  
    RTC_Cnt_Start();
    delayms(100);
}

//二极菜单，时间日期设置---> 分
void menu_4_sub1_2(void ) //
{      
    RTC_Cnt_Stop();    
    to_tm(RtcCounter,&Tim);  

    Fill_one_line(4,0,0,128);
    Fill_one_line(5,0,0,128);
    
    if(gKeyValue==KEY_VALUE_UP)
    {
        Tim.tm_min++;
        if(Tim.tm_min>59)
        {
            Tim.tm_min=0;
        }
    }
    else if(gKeyValue==KEY_VALUE_DOWN)
    {
        Tim.tm_min--;
        if(Tim.tm_min<0)
        {
            Tim.tm_min=59;
        }
    }
    
    DisplayHMS();     

    byte_to_str(SysTime.tm_min,str);
    LCD_print(4,56,str,1);
    RTC_Cnt_Start();
    delayms(100);
}


//二极菜单，时间日期设置---> 秒
void menu_4_sub1_3(void) //
{    
    RTC_Cnt_Stop();    
    to_tm(RtcCounter,&Tim); 
    Fill_one_line(4,0,0,128);
    Fill_one_line(5,0,0,128);      
    if(gKeyValue==KEY_VALUE_UP)
    {
        Tim.tm_sec++;
        if(Tim.tm_sec>59)
        {
            Tim.tm_sec=0;
        }
    }
    else if(gKeyValue==KEY_VALUE_DOWN)
    {
        Tim.tm_sec--;
        if(Tim.tm_sec<0)
        {
            Tim.tm_sec=59;
        }
    }
    DisplayHMS(); 
    
    byte_to_str(SysTime.tm_sec,str);
    LCD_print(4,88,str,1);

    RTC_Cnt_Start();
    delayms(100);  
}

void DisplayYMD(void)
{
    RtcCounter = mktimev(&Tim);
    to_tm(RtcCounter,&SysTime);
    {
        byte_to_str(SysTime.tm_mday,&str[0]); 
        str[2]=0x2f; 
        byte_to_str(SysTime.tm_mon,&str[3]);
        str[5]=0x2f;
        int_to_str(SysTime.tm_year,&str[6]);
        str[10]=0;    
        LCD_print(4,20,str,0);
    }
    Display_Time_EndLine(); 
  
}

//二极菜单，时间日期设置---> 年
void menu_4_sub1_4(void) 
{    
    RTC_Cnt_Stop();    
    to_tm(RtcCounter,&Tim);  
    LCD_print_line(4,0," ",0);  
                            //  06/04/2015
    {
        if(gKeyValue==KEY_VALUE_UP)
        {
            Tim.tm_mday++;
            if(Tim.tm_mday>31)
            {
                Tim.tm_mday=1;
            }
        }
        else if(gKeyValue==KEY_VALUE_DOWN)
        {
            Tim.tm_mday--;
            if(Tim.tm_mday<1)
            {
                Tim.tm_mday=31;
            }
        }
        DisplayYMD();     

        byte_to_str(SysTime.tm_mday,str);
        LCD_print(4,20,str,1);
    }  
    RTC_Cnt_Start(); 
    delayms(100);   
}

//二极菜单，时间日期设置---> 月
void menu_4_sub1_5(void) //
{     
    RTC_Cnt_Stop();    
    to_tm(RtcCounter,&Tim); 
    LCD_print_line(4,0," ",0);    

    if(gKeyValue==KEY_VALUE_UP)
    {
        Tim.tm_mon++;
        if(Tim.tm_mon>12)
        {
            Tim.tm_mon=1;
        }
    }
    else if(gKeyValue==KEY_VALUE_DOWN)
    {
        Tim.tm_mon--;
        if(Tim.tm_mon<1)
        {
            Tim.tm_mon=12;
        }
    }
    DisplayYMD();     
    {
        byte_to_str(SysTime.tm_mon,str);
        LCD_print(4,47,str,1);    
    
    }
    RTC_Cnt_Start(); 
    delayms(100);   
}

//二极菜单，时间日期设置---> 日
void menu_4_sub1_6(void) //
{     
    RTC_Cnt_Stop();    
    to_tm(RtcCounter,&Tim); 
    LCD_print_line(4,0," ",0);   
 
                        // 06/04/2015
    {

        if(gKeyValue==KEY_VALUE_UP)
        {
            Tim.tm_year++;
            if(Tim.tm_year>2100)
            {
                Tim.tm_year=2000;
            }
        }
        else if(gKeyValue==KEY_VALUE_DOWN)
        {
            Tim.tm_year--;
            if(Tim.tm_year<2000)
            {
                Tim.tm_year=2100;
            }
        }

        DisplayYMD(); 
        int_to_str(SysTime.tm_year,str);
        LCD_print(4,74,str,1);
    }



    RTC_Cnt_Start(); 
    delayms(100);    
}


void Display_RF_Cnt(void)
{
    int_to_str(EEP_RFCnt,str);
    {
        LCD_print_line(5,1,"Total:",0);
    }
    LCD_print(5,56,"000/500",0);
    LCD_print(5,56,&str[1],0);
}


void MenuKey_proc(void)
{
    if(gKeyValue==KEY_VALUE_OK)
    {
        NumIndex++;
        if(NumIndex>3) NumIndex=0;                 
    }  
    else if(gKeyValue==KEY_VALUE_UP)
    {
        NumTmp[NumIndex]++;
        if((NumTmp[NumIndex]>'9')&&(NumTmp[NumIndex]<'A') )
        {
            NumTmp[NumIndex]='A';
        }
        else if(NumTmp[NumIndex]>'Z')
        {
            NumTmp[NumIndex]='0';
        }
    }
    else if(gKeyValue==KEY_VALUE_DOWN)
    {
        NumTmp[NumIndex]--;
        if((NumTmp[NumIndex]>'9')&&(NumTmp[NumIndex]<'A') )
        {
            NumTmp[NumIndex]='9';
        }
        else if(NumTmp[NumIndex]<'0')
        {
            NumTmp[NumIndex]='Z';
        }
    }
}

//注册呼叫器
void menu2_AddCaller(void)
{
    LCD_print_line(7,1,"",0); //删除最后一行
    {
        LCD_print_line(1,20,"add caller",0); 
        LCD_print_line(3,1,"add     ID",0);    
    }
    
    LCD_print(3,32,NumTmp,0);
    MenuKey_proc(); 
    display_ASCII_16x8(3,32+9*NumIndex,ASCII_16X8[NumTmp[NumIndex]-0x20],1);  
    
    Display_RF_Cnt();
    
    delayms(80);    
}

//学习添加码,主循环里调用
void Add_Transmit(uint32_t dat)     
{
    RF_def tmp;
    uint8_t c;
    
    if(15==index)
    {   
        ClearWatchDog();
        
        LCDBK_ON(60);
        MenuTimout(60);
//         tmp.rf = dat>>4;
        tmp.rf = dat;
        
        memcpy(tmp.region,NumTmp,4);

        if(!Store_RF_TO_EEPROM(&tmp))    //已经注册了区号加1
        {
            c=NumTmp[0];
            RF_No = str_to_int(NumTmp);
            RF_No++;
            int_to_str(RF_No,NumTmp);
            NumTmp[0]=c;
			LCD_print(3,32,NumTmp,0);
        }
        display_ASCII_16x8(3,32+9*NumIndex,ASCII_16X8[NumTmp[NumIndex]-0x20],1);           
        ClearWatchDog();

        Display_RF_Cnt();        
        LCD_print_line(7,10,"done",0);
        delayms(500);
        LCD_print_line(7,10," ",0); 
    }        
}


//在最后一行显示日期和时间
void Display_Time_EndLine(void)
{
//20XX-XX-XX HH:MM:SS   
        Display_Time_Line(8);
        str[0]=':';
        byte_to_str(SysTime.tm_sec,&str[1]);         //秒
        print_str8x5(8,98,str,0);   
}

//呼叫显示
void Display_Call(uint8_t line,int8_t idx,int8_t bHlight)
{
    xdata struct rtc_time tm;
    xdata  uchar i;
    xdata unsigned char servicetype[12];
   //      char str[20];

    if((RFlist[idx].use==0)||(idx>=RF_list_cnt()) )
    {
       LCD_print_line(line,0," ",0);     //删除本行
       return;
    }
     
    to_tm(RFlist[idx].Tim,&tm); 
    memcpy(str,RFlist[idx].region,4); 
    LCD_print_line(line,1,str,bHlight);     
    
    byte_to_str(RFlist[idx].cnt,str);
    print_str8x5(line,112,str,bHlight);
    
    byte_to_str(tm.tm_hour,str);
    print_str8x5(line,68,str,bHlight);
    print_str8x5(line,80,":",bHlight);
    byte_to_str(tm.tm_min,str);
    print_str8x5(line,86,str,bHlight);   
    // type_id =RFlist[idx].SrvNo;  //20160308
   //Read_SERVICE_TYPE(type_id,service_type);
    //byte_to_str(RFlist[idx].SrvNo,str);
    //LCD_print_line(line+2,1,str,bHlight);    
    EEP_ReadBytes((uint16_t)SERVICE_TYPE_EEPROM_ADDR+((RFlist[idx].SrvNo)<<4),servicetype,12);   
    print_str8x5_sjz(line+1,40,servicetype,bHlight);

}




//RF显示菜单 删除键
void menu_RF_del(void)
{
     uint8_t cnt;
     index_tmp=index;
     Del_call_list(ListIndex);    
     cnt=RF_list_cnt();
     
     if(RFlist[ListIndex].use==0)
     {
        ListIndex--;
        if(ListIndex<0) ListIndex=cnt-1;
        
     }    
     if((RFlist[0].use==0)||(cnt==0))
     {
        ListLine=1;
        ListIndex=0;
        index_tmp=0;
        main_menu(); 
        return;
     }
     else
     {
        if(cnt<2)
        {
            if(ListLine==2)ListLine=1;
        }
        else if(cnt<3)
        {
            if(ListLine==3)ListLine=2;
        }
     }
     displayRF();          

     
     while(!KEY_RETURN)ClearWatchDog();  
}



//删除一个RF列表
void RF_List_remove(int8_t CurrentIndex)
{
     Del_call_list(CurrentIndex); 
     ListLine=1;
     ListIndex=0;
     if(RFlist[0].use==0)
     {
        ListLine=1;
        ListIndex=0;
        index_tmp=0;
        main_menu(); 
        return;
     }
     displayRF(); 
}


code char *menu3Text[1][2]={

   "Delete single",
   "Delete all",
};



//删除全部发射器
void menu3_DelCaller(void)  
{
    while(!KEY_OK)
    {
        if(LongPressSec>1)
        {
            clear_screen();
            LCD_print_line(3,1, "removing all",0);
            
            Del_all_RF_EEPROM();   //删除存储的码表    
            
            LCD_print_line(3,1,"removed!",0);
            ClearWatchDog();
            delayms(1000);
            index=16;
            menu_RF_remove_single();
    
    		while(!KEY_OK) ClearWatchDog();
        }
//         else
//         {
//            index=61;
//         }
        ClearWatchDog();
    }
}

void menu_RF_remove_single(void)
{


        LCD_print_line(1,8, Title[Language][2],0);

     
    LCD_print_line(3,10, menu3Text[Language][0],0);   //删除单个呼叫器 /*sjz 20160305*/
    LCD_print(3,1,"*",0); /*sjz 20160305*/
    LCD_print_line(5,10, menu3Text[Language][1],0);   //删除全部呼叫器
    LCD_print_line(7,1," ",0);
    NumIndex=3;
    memset(NumTmp,'0',4);
    NumTmp[4]=0;
    delayms(60);    
}


void menu_RF_remove_all(void)
{
    LCD_print_line(3,10, menu3Text[Language][0],0);
    LCD_print_line(5,10, menu3Text[Language][1],0); /*sjz 20160305*/
    LCD_print(5,1,"*",0);  /*sjz 20160305*/
    delayms(60);
}


//删除单个呼叫器
void do_del_RF_single(void)
{
    LCD_print_line(1,10, menu3Text[Language][0],0);
    LCD_print_line(3,1," ",0);
    LCD_print_line(5,35,NumTmp,0);
    LCD_print_line(7,1," ",0);
    while(!KEY_OK)
    {
        if(LongPressSec>1)
        {
             Del_RF_EEPROM(NumTmp);
			 LCD_print_line(7,10," removed",0);
			 ClearWatchDog();
			 delayms(800);
			 LCD_print_line(7,1," ",0);
			 NumIndex=3;
			 while(!KEY_OK) ClearWatchDog();
        }
        ClearWatchDog();
    }
    MenuKey_proc();
    display_ASCII_16x8(5,35+9*NumIndex,ASCII_16X8[NumTmp[NumIndex]-0x20],1);
}



code char* menu4_text[1][2]={
    {"□live view" , "□queue view"}
};


//即时显示
void menu4_LiveView(void)
{
    clear_screen();

	{
		LCD_print_line(1,15, Title[Language][5],0);
	}
    
    LCD_print_line(3,10, menu4_text[Language][0],0);/*sjz 20160305*/
    LCD_print(3,1,"*",0);  /*sjz 20160305*/
    LCD_print_line(5,10, menu4_text[Language][1],0);
    
    if(gKeyValue==KEY_VALUE_OK)
    {
        if(!menu_arg)
        {
           menu_arg=1; 
        }
        else
        {
            DspMode=0;
        }
    }
    
    if(0==DspMode)    //即时模式
    {
       LCD_print(3,10,"■",0); /*20160305 sjz */
    }
    else
    {
       LCD_print(5,10,"■",0);/*20160305 sjz */
    }
    EEprom.dspmode=DspMode;
    EEP_WriteBytes(0x0000,&EEprom,sizeof(Env_t));    
}

//排队显示
void menu4_Queue(void)
{
    clear_screen();
    LCD_print_line(1,28, Title[Language][4],0);
    LCD_print_line(3,10, menu4_text[Language][0],0);
    LCD_print_line(5,10, menu4_text[Language][1],0);
    LCD_print(5,1,"*",0);  /*sjz 20160305*/
    if(gKeyValue==KEY_VALUE_OK)
    {
        DspMode=1;
    }
    
    if(1==DspMode)    //排队模式
    {
       LCD_print(5,10,"■",0);/*20160305 sjz */
    }
    else
    {
       LCD_print(3,10,"■",0);/*20160305 sjz */
    }
    
    EEprom.dspmode=DspMode;
    EEP_WriteBytes(0x0000,&EEprom,sizeof(Env_t));       
}



//消号时间设置
void menu5_sub1(void)
{
    clear_screen();
    LCD_print_line(1,20,"Timing clear",0);
    byte_to_str(ClearTim,str);
    LCD_print(4,48,str,1);
    LCD_print(4,68,"sec",0);
}


//消号时间设置，菜单向上移
void menu5_sub1_up(void)
{
    ClearTim++;
    if(ClearTim>99) ClearTim=0;
    
    EEprom.cleartim=ClearTim;
    EEP_WriteBytes(0x0000,&EEprom,sizeof(Env_t));
    byte_to_str(ClearTim,str);
    LCD_print(4,48,str,1);  
    delayms(100); 
}


//消号时间设置，菜单向下移
void menu5_sub1_down(void)
{
    ClearTim--;
    if(ClearTim<0) ClearTim=99;
    
    EEprom.cleartim=ClearTim;
    EEP_WriteBytes(0x0000,&EEprom,sizeof(Env_t));
    byte_to_str(ClearTim,str);
    LCD_print(4,48,str,1);    
    delayms(100);
}




//循环时间设置
void menu6_sub1(void)
{
    clear_screen();
    {
        LCD_print_line(1,15,"cycling time",0);
    }
    byte_to_str(CyclingTim,str);
    LCD_print(4,48,str,1);
    LCD_print(4,68,"sec",0);
}


//循环时间设置，菜单向上移
void menu6_sub1_up(void)
{
    CyclingTim++;
    if(CyclingTim>99) CyclingTim=0;
    
    EEprom.cyclingtim=CyclingTim;
    EEP_WriteBytes(0x0000,&EEprom,sizeof(Env_t));
    byte_to_str(CyclingTim,str);
    LCD_print(4,48,str,1); 
    delayms(100);
}


//循环时间设置，菜单向下移
void menu6_sub1_down(void)
{
    CyclingTim--;
    if(CyclingTim<0) CyclingTim=99;
    
    EEprom.cyclingtim=CyclingTim;
    EEP_WriteBytes(0x0000,&EEprom,sizeof(Env_t));
    byte_to_str(CyclingTim,str);
    LCD_print(4,48,str,1);    
    delayms(100);
}




//信号提示设置
code char *Menu8Text[1][3]={
{"□light","□vibration","□sound"}
};

void menu7_sub2_display(void)
{
    LCD_print_line(1,10,Title[Language][6],0);  //信号接收设置
    LCD_print_line(3,10,Menu8Text[Language][0],0);           
    LCD_print_line(5,10,Menu8Text[Language][1],0);
    LCD_print_line(7,10,Menu8Text[Language][2],0);
    if(MsgLight)
    {
        LCD_print(3,10,"■",0);
    }
    if(MsgShake)
    {
        LCD_print(5,10,"■",0);
    }
    if(MsgBeep)
    {
        LCD_print(7,10,"■",0);
    }

}
void menu7_sub2_Light(void)
{
    menu7_sub2_display();
    if(gKeyValue==KEY_VALUE_OK)
    {
        if(menu_arg)
        {
            MsgLight=!MsgLight;
            
            EEprom.promptLight=MsgLight;
            EEP_WriteBytes(0x0000,&EEprom,sizeof(Env_t));
        }
    }
    menu_arg=1;
    LCD_print_line(3,10,Menu8Text[Language][0],0);     
    LCD_print(3,1,"*",0);  /*sjz 20160305*/
    if(MsgLight)
    {
        LCD_print(3,10,"■",0);
    } 
}

void menu7_sub2_Sound(void)
{
    menu7_sub2_display();
    if(gKeyValue==KEY_VALUE_OK)
    {
        MsgBeep=!MsgBeep;
        
        EEprom.prompBeep=MsgBeep;
        EEP_WriteBytes(0x0000,&EEprom,sizeof(Env_t));        
        
    }
    
    LCD_print_line(7,10,Menu8Text[Language][2],0);
    LCD_print(7,1,"*",0);  /*sjz 20160305*/
    if(MsgBeep)
    {
        LCD_print(7,10,"■",0);
    } 
}

void menu7_sub2_Shake(void)
{
    menu7_sub2_display();
    if(gKeyValue==KEY_VALUE_OK)
    {
        MsgShake=!MsgShake;
        
        EEprom.promptShake=MsgShake;
        EEP_WriteBytes(0x0000,&EEprom,sizeof(Env_t));         
    }    
     
    LCD_print_line(5,10,Menu8Text[Language][1],0);
    LCD_print(5,1,"*",0);  /*sjz 20160305*/
    if(MsgShake)
    {
        LCD_print(5,10,"■",0);
    }
}


void menu7_promptTime(void)
{
    clear_screen();
    LCD_print_line(1,20,"prompt time",0);
    byte_to_str(PromptTim,str);
    LCD_print(4,48,str,1);
    LCD_print(4,68,"sec",0);

}


void menu7_sub2_up(void)
{
    PromptTim++;
    if(PromptTim>99) PromptTim=0;
    
    EEprom.prompttim=PromptTim;
    EEP_WriteBytes(0x0000,&EEprom,sizeof(Env_t));
    byte_to_str(PromptTim,str);
    LCD_print(4,48,str,1);   
    delayms(100);
}

void menu7_sub2_down(void)
{
    PromptTim--;
    if(PromptTim<0) PromptTim=99;

    EEprom.prompttim=PromptTim;
    EEP_WriteBytes(0x0000,&EEprom,sizeof(Env_t));    
    byte_to_str(PromptTim,str);
    LCD_print(4,48,str,1);    
    delayms(100);
}


code char *ModeTitle[1][2]={
    { "□Single key","□Multi-key"},   
};


//模式设置--单键模式
void Menu_Mode_Set_1(void)
{ 

    {
        LCD_print_line(1,20,Title[Language][4],0);
    }

    if(gKeyValue==KEY_VALUE_OK) 
    {
        if(menu_arg)
        {        
            WorkMode=EEprom.workmode=ONE_KEY; 
            EEP_WriteBytes(0x0000,&EEprom,sizeof(Env_t));            
        }
        menu_arg=1;
    }
    
    LCD_print_line(3,10,ModeTitle[Language][0],0);
    LCD_print(3,1,"*",0); /*sjz 20160305*/
    LCD_print_line(5,10,ModeTitle[Language][1],0);  /*sjz 20160305*/
    LCD_print_line(7,10," ",0);
    ONE_KEY==WorkMode?LCD_print(3,10,"■",0):LCD_print(5,10,"■",0); /*sjz 20160305*/
    delayms(60);
}

//模式设置--多键模式
void Menu_Mode_Set_2(void)
{

    {
        LCD_print_line(1,20,Title[Language][4],0);
    }
    if(gKeyValue==KEY_VALUE_OK) 
    {
        WorkMode=EEprom.workmode=MULTI_KEY; 
        EEP_WriteBytes(0x0000,&EEprom,sizeof(Env_t));
    }
    LCD_print_line(3,10,ModeTitle[Language][0],0);
 
    LCD_print_line(5,10,ModeTitle[Language][1],0);  /*sjz 20160305*/
    LCD_print(5,1,"*",0); /*sjz 20160305*/
    LCD_print_line(7,10," ",0);    
    ONE_KEY==WorkMode?LCD_print(3,10,"■",0):LCD_print(5,10,"■",0);  /*sjz 20160305*/
    delayms(60);
}



code char *menu10_text[1][6] ={
{"Confirm Del?", " Yes / No "," Yes"," No ","Deleting...","Deleted"}
}; 
//一级菜单------键位功能设置
void menu10_sub1(void)
{
	clear_screen();
    NumTmp[1]=0;
    gSrvNo=0;
        LCD_print_line(1,10,"Key function",0);    
        LCD_print(4,18,"Area ",0);
        LCD_print(4,55,"  ",1);
        LCD_print(4,60,NumTmp,1);      //学习0XX区 
//         LCD_print_line(7,1,"Pls add First",0);
		LCD_print_line(7,1," ",0); 		
    delayms(50);	

}

void menu10_sub1_area_set(void)
{
    NumTmp[1]=0;
    gSrvNo=0;

        LCD_print_line(1,8, Title[Language][3],0);
        LCD_print_line(3,10,"Area Set",0);   //删除单个呼叫器 /*sjz 20160305*/

        LCD_print_line(5,10, "Type set",0);   //删除全部呼叫器
        LCD_print(3,1,"*",0); /*sjz 20160305*/
        LCD_print_line(7,1," ",0);

    delayms(60);    
}
void menu10_sub1_type_set(void)
{

        LCD_print_line(1,8, Title[Language][3],0);
        LCD_print_line(3,10,"Area Set",0);   //删除单个呼叫器 /*sjz 20160305*/
        LCD_print_line(5,10, "Type set",0);   //删除全部呼叫器
        LCD_print(5,1,"*",0); /*sjz 20160305*/
        LCD_print_line(7,1," ",0);

    delayms(60);    
}

void menu10_sub2(void)
{
    clear_screen();
//    gSrvNo=0;

        LCD_print_line(1,10,"Serv Type Set",0);    
        LCD_print(4,10,"Type ",0);
        LCD_print_line(7,10,"Pls Sel Type",0);
        LCD_print(4,55,"  ",1);
        LCD_print(4,60,"01",1);     
        //memset(serviceTmp,'a',10);
    delayms(50);	
}
//二级菜单------键位功能设置--》区号+1
void menu10_sub2_up(void)
{
    if(gSrvNo ==0)
    {
    	   gSrvNo=2;
    }
    else
    {
          gSrvNo++;
    }
    if(gSrvNo==40)
    {
       gSrvNo=0;
    }
     byte_to_str(gSrvNo+1,str);
    LCD_print(4,60,str,1);
    delayms(100);
}


//二级菜单------键位功能设置--》区号-1
void menu10_sub2_down(void)
{
    if(gSrvNo == 0)
    {
       gSrvNo=40;
    }
    gSrvNo--;
    byte_to_str(gSrvNo+1,str);
   LCD_print(4,60,str,1);
    delayms(100);	
}


void MenuKey_proc_sjz(void)
{
    if(gKeyValue==KEY_VALUE_OK)
    {
        NumIndex++;
        if(NumIndex>11) NumIndex=0;                 
    }  
    else if(gKeyValue==KEY_VALUE_UP)
    {
        if(serviceTmp[NumIndex]==' ')
        {
           serviceTmp[NumIndex]='a';
        }
	 else if(serviceTmp[NumIndex]=='Z')
	 {
	     serviceTmp[NumIndex]=' ';
	 }
        else if(serviceTmp[NumIndex]=='z')
        {
            serviceTmp[NumIndex]='A';
        }	
	 else
	 {
            serviceTmp[NumIndex]++;
	 }
	 
    }
    else if(gKeyValue==KEY_VALUE_DOWN)
    {
        if(serviceTmp[NumIndex]==' ')
        {
           serviceTmp[NumIndex]='Z';
        }
	 else if(serviceTmp[NumIndex]=='A')
	 {
	     serviceTmp[NumIndex]='z';
	 }
        else if(serviceTmp[NumIndex]=='a')
        {
            serviceTmp[NumIndex]=' ';
        }	
	 else
	 {
            serviceTmp[NumIndex]--;
	 }
    }
}
void menu10_sub2_enter(void)
{
   uchar *service_type;
   uint8_t *type_name;
   clear_screen();
//   Read_SERVICE_TYPE(gSrvNo,service_type);


    LCD_print_line(1,1,"Modify Type",0); 
    LCD_print_line(3,1,"Type ID",0);    
    byte_to_str(gSrvNo+1,str);
    LCD_print(3,80,str,0);
    EEP_ReadBytes((uint16_t)SERVICE_TYPE_EEPROM_ADDR+(gSrvNo<<4),serviceTmp,12);
    LCD_print(5,1,serviceTmp,0);
    MenuKey_proc_sjz();
    display_ASCII_16x8(5,1+9*NumIndex,ASCII_16X8[serviceTmp[NumIndex]-0x20],1);  
   // type_name=KeySevice[0][gSrvNo];
    EEP_WriteBytes((uint16_t)SERVICE_TYPE_EEPROM_ADDR+(gSrvNo<<4),serviceTmp,12);   

//    LCD_print_line(7,1,service_type,0);    
	
}

/*sjz 20160306 add end*/


//二级菜单------键位功能设置--》区号+1
void menu10_sub1_up(void)
{
    NumTmp[0]++;
    
    if((NumTmp[0]>'9')&&(NumTmp[0]<'A'))
    {
       NumTmp[0]='A';
    }
    else
    {
        if(NumTmp[0]>'Z') NumTmp[0]='0';
    }
    LCD_print(4,60,NumTmp,1);
    
    delayms(100);
}


//二级菜单------键位功能设置--》区号-1
void menu10_sub1_down(void)
{
    NumTmp[0]--;
    if((NumTmp[0]>'9')&&(NumTmp[0]<'A'))
    {
       NumTmp[0]='9';
    }
    else
    {
        if(NumTmp[0]<'0') NumTmp[0]='Z';
    }
    LCD_print(4,60,NumTmp,1);
    delayms(100);	
}

void menu10_sbu1_display(uint8_t bH)
{
    LCD_print_line(7,1,"  ",0);
    {
       LCD_print_line(1,10,"Key function",0); 
       LCD_print_line(3,1,"Set   's key",bH);
       NumTmp[1]=0;
       LCD_print(3,32,NumTmp,bH);  //学习0XX区
       LCD_print_line(5,1,"copy   to all",bH-1);   //复制0XX区到全部
       LCD_print(5,46,NumTmp,bH-1);
    }
    delayms(100);
}                                                                

//三级菜单------键位功能设置--->设置XX区键位功能
void menu10_sbu1_1(void)
{
    menu10_sbu1_display(1); 	
}

//三级菜单------键位功能设置--->复位到XX区
void menu10_sbu1_2(void)
{
    menu10_sbu1_display(0);
    menu_arg=0;
}

//添加键位功能
void menu10_sbu1_set(void)
{    
   int8_t servicetype[12],i;
    switch(gKeyValue)
    {
        case KEY_VALUE_OK:
            break;
        case KEY_VALUE_UP:
            gSrvNo++;
            if(gSrvNo>(KEY_NUM-1)) gSrvNo=0;
            break;
        case KEY_VALUE_DOWN:
            gSrvNo--;
            if(gSrvNo<0) gSrvNo=(KEY_NUM-1);
            break;
        default:
            break;
     }
     clear_screen();
     EEP_ReadBytes((uint16_t)SERVICE_TYPE_EEPROM_ADDR+(gSrvNo<<4),servicetype,12);
     //memset(servicetype,'w',10);
     LCD_print_line(1,10,"Key function",0);
     LCD_print_line(3,1,"press area  's",0);
        NumTmp[1]=0;
        LCD_print(3,95,NumTmp,0); 
	 LCD_print_line(5,1," ",0);   
        LCD_print_line(7,1," ",0);  
	 print_str8x5_sjz(6,1,servicetype,0);
	 //LCD_print(5,1,KeySevice[Language][(uint8_t)gSrvNo],1);       
    byte_to_str(gSrvNo+1,str);
    print_str8x5(6,98,str,0); 
    print_str8x5(6,110,"/40",0);
    delayms(80);    
}


void menu10_sbu1_yes(void)
{
    uint8_t i;
    xdata uint8_t Key[KEY_NUM];
    uint16_t addr;
	clear_screen();
    LCD_print_line(3,10,"Copy to all？",0);
    LCD_print_line(5,10,menu10_text[Language][1],0);
    LCD_print(5,10,menu10_text[Language][2],1);
    
    if(gKeyValue==KEY_VALUE_OK)
    {
        if(!menu_arg)
        {
            while(!KEY_OK)
            {   
                ClearWatchDog();  
                if(LongPressSec>2) break;
            }
            menu_arg=1;
            return;
        }
        clear_screen();
       LCD_print_line(3,10,"copying...",0); 
        
        Find_Key_Fun(NumTmp[0],Key);
        
        addr=RF_KEY_EEPROM_ADDR;
        for(i='0';i<='Z';i++)        //复制到0区到Z区
        {
            EEP_WriteBytes(addr,Key,sizeof(Key));
			delayms(10);
            addr+=KEY_NUM;
        }
        LCD_print_line(3,10,"Complete",0);
        delayms(800);
        menu10_sbu1_2();
        index=24;
    }
    delayms(50);

}

void menu10_sbu1_no(void)
{
    LCD_print_line(3,10,"Copy to all？",0);
    LCD_print(5,10,menu10_text[Language][1],0);
    LCD_print(5,66,menu10_text[Language][3],1); 
    
    delayms(50);    
}

//设置键位功能
void Set_Region_Key(uint32_t dat)
{
    xdata uint8_t Key[KEY_NUM];
    uint8_t i;
    uint8_t rfKey=dat&0x0f;
 //   byte_to_str(rfKey,str);	
    if(index==25)
    {   
        LCDBK_ON(60);
        MenuTimout(60);    
    
		ClearWatchDog();     
        Find_Key_Fun(NumTmp[0],Key);
        
        for(i=0;i<KEY_NUM;i++)       //其它存有的键值要删除
        {
            if(Key[i]==rfKey)
            {
                Key[i]=0;
            }
        }        
        Key[gSrvNo]=rfKey;  
		
        Store_Key_Fun(NumTmp[0],Key);
        LCD_print_line(7,10,"successfully",0); 
	
        Beep(500);
        delayms(700);
        LCD_print_line(7,0," ",0); 
		
		
		
// 		Find_Key_Fun(NumTmp[0],Key);
// 		for(i=0;i<6;i++)
// 		{
// 			byte_to_str(Key[i],str);
// 			print_str8x5(8,1+i*15,str,0);
// 		}
// 		memcpy(str,NumTmp,5);
// 		print_str8x5(8,100,str,0);
    }   
}

//语言选择
void menu12_Chinese(void)
{
    if(menu_arg)
    {
        while(!KEY_OK)
        {
            if(LongPressSec>9)
            { 
                  menu_arg=0;
                  Language=EEprom.language=0;    //中文
                  EEP_WriteBytes(0x0000,&EEprom,sizeof(Env_t));
                  
                  Beep(300);
                  LCDBK_OFF();
                  delayms(300);
                  LCDBK_ON(10);
                  
                  break;
            }
            ClearWatchDog();
        }
    }
    menu_arg=1;    
    LCD_print_line(1,25,Title[1][12],0); 
    LCD_print_line(3,1,"*CN",0);/*sjz 20160305*/
    LCD_print_line(5,1," EN",0); 
    LCD_print_line(7,1," ",0); 
    while(!KEY_OK)
    {    
        ClearWatchDog();
    }
    delayms(300);
}

void menu12_English(void)  
{
    while(!KEY_OK)
    {
        if(LongPressSec>9)
        {
            Language=EEprom.language=1;    //英文
            EEP_WriteBytes(0x0000,&EEprom,sizeof(Env_t));

            Beep(300);
            LCDBK_OFF();
            delayms(300);
            LCDBK_ON(10);  
              
             break;
        }
        ClearWatchDog();
    }

    LCD_print_line(1,25,Title[1][12],0); 
    LCD_print_line(3,1," CN",0);
    LCD_print_line(5,1,"*EN",0); /*20160305*/
    LCD_print_line(7,1," ",0); 
    menu_arg=0;
    
    while(!KEY_OK)
    {    
        ClearWatchDog();
    }
    delayms(300);
}


code char *Text1[1][3]={
 {"Signal ON/OFF"," Power off  ", "   Reset    "}
};


void Display_RF_Power_Set(int8_t hLine)
{
    Display_Time_Line(1);
    Diaplay_BAT();   //显示电池图标
    MenuMode=3;
    LCD_print(3,10,Text1[Language][0],!(hLine-1));
    LCD_print(5,10,Text1[Language][1],!(hLine-2));    
    LCD_print(7,10,Text1[Language][2],!(hLine-3)); 
    
    while(!KEY_RETURN)ClearWatchDog();
    delayms(20);
}

void RF_Power_Set(void)
{
    clear_screen();
    if(gKeyValue==KEY_VALUE_OK)      //是否要开关信号
    {
        bSignalSet=!bSignalSet;      //关无线电接收
        POWER_CS=!bSignalSet;
        main_menu();
        return;
    }
    Display_RF_Power_Set(1);
}

void PowerDown_Set(void)
{
    clear_screen();
    if(gKeyValue==KEY_VALUE_OK)         //关机
    {
        LCD_print(4,50,Bye[Language],0);
        Power_Down();
    } 
    Display_RF_Power_Set(2);  
}
void PowerDown_Set_sjz(void)
{
    clear_screen();
    LCD_print(5,10,Text1[Language][1],!(0));  
	  while(!KEY_RETURN)ClearWatchDog();
	  delayms(120);
	  clear_screen();
    LCD_print(4,50,Bye[Language],0);
    Power_Down();
    //Display_RF_Power_Set(2);  
}
void Rest_Set(void)
{
    clear_screen();
    if(gKeyValue==KEY_VALUE_OK)
    {
        POWERON_FLG=0x55AA;
        ISPCR=0X60; //重启到ISP
    }   
    Display_RF_Power_Set(3); 
}



void displayRF(void)
{   

    Display_Call(3,ListIndex+1-ListLine,!(ListLine-1));
    Display_Call(5,ListIndex+2-ListLine,!(ListLine-2));
    Display_Call(7,ListIndex+3-ListLine,!(ListLine-3));          
}

void menu_RF_list(void )   //请勿删除此函数
{
//     _nop_ ();
    index_tmp=index;
        
}


//RF显示菜单 上翻
void menu_RF_up(void)
{
    int8_t cnt=RF_list_cnt();
    ListIndex--;
    if(ListIndex<0)
    {   
        ListIndex=cnt-1;
        if(cnt>=3)
        {
           ListLine=3; 
        }
        else
        {
            ListLine=cnt;
        }
    }
    else
    {
        ListLine--;
        if(ListLine<1) ListLine=1;
    }    
    displayRF();
}



//RF显示菜单 下翻
void menu_RF_down(void)
{
    uint8_t cnt=RF_list_cnt();
    ListIndex++; 
    if(ListIndex>cnt-1)  
    {
        ListIndex=0;
        ListLine=1;
    }
    else if(ListLine==1)
    {
         ListLine=2;
    }
    else if((ListLine==2)||((ListLine==3)))
    {
         ListLine=3;
    }
    else
    {
       ListLine=1;
    }
    
    displayRF();
}

void Display_RF_list(void)
{

	if(RFlist[0].use==1)
	{
		Display_Call(3,0,1);
		if(RFlist[1].use==1)
		{
			Display_Call(5,1,0);
			if(RFlist[2].use==1)
			{
				Display_Call(7,2,0);
			}
		}
	}
    ListIndex=0;
    ListLine=1;  
}

//从列表里删除一个RF
void Del_call_list(int8_t idx)
{
    uint8_t i;
    uint8_t cnt;
    cnt=RF_list_cnt();
    if(cnt>0)
    {
        for(i=idx;i<RFLIST_QTY;i++)
        {      
           memcpy(&RFlist[i],&RFlist[i+1],sizeof(RF_list_def));
        }
        memset(&RFlist[cnt-1],0,sizeof(RF_list_def));
    }
}


// 键盘设置
void Menu11_KeyBoard(void)
{
    clear_screen();
    {
        LCD_print_line(1,15,Title[Language][11],0);
    }


    
    if(gKeyValue==KEY_VALUE_UP)
    {
        KeyBoardNo++;
        if(KeyBoardNo>11)
        {
            KeyBoardNo=0;
        }
    }
    else if(gKeyValue==KEY_VALUE_DOWN)
    {
        KeyBoardNo--;
        if(KeyBoardNo<0)
        {
            KeyBoardNo=11;
        }    
    }
    
    EEprom.keyboardno=KeyBoardNo;
    EEP_WriteBytes(0x0000,&EEprom,sizeof(Env_t));    
    
    byte_to_str(KeyBoardNo,str);
    
    {
        if(KeyBoardNo==10)
        {
//              LCD_print_line(4,1,"Receive ",0);
//              LCD_print(4,73,"OFF ALL",1);
				LCD_print_line(4,1,"OFF ALL",1);
			    LCD_print_line(6,1,"Keyboard",0);
        }
        else if(KeyBoardNo==11)
        {
//              LCD_print_line(4,1,"Receive ",0);
//              LCD_print(4,73,"ON ALL",1);
				LCD_print_line(4,1,"ON ALL",1);
			    LCD_print_line(6,1,"Keyboard",0);			
        }
        else
        {
            LCD_print_line(4,1,"Receive ",0);
            LCD_print(4,73,str,1);
			LCD_print_line(6,1," ",0);
        }
    }   
}




//OK键被按下时
void menu_factory_eset(void)
{
   
}

void menu11_sbu1_yes(void)
{   
    clear_screen();
   LCD_print_line(3,1,"Confirm reset？",0);
    LCD_print_line(5,10,menu10_text[Language][1],0);
    LCD_print(5,10,menu10_text[Language][2],1);

    while(!KEY_OK)
    {
        if(LongPressSec>4)
        {
            clear_screen();
//            Language==0?LCD_print_line(3,1,"\xd5\xfd在恢复出厂设置",0):LCD_print_line(3,1,"Factory reseting",0);

						{
									LCD_print_line(2,1,"    Factory     ",0);
									LCD_print_line(6,1,"    reseting    ",0);
						}

            Erase_all_EEPROM();

            memset(&EEprom,0xff,sizeof(Env_t));
            EEprom.language=Language;   //恢复时不恢复语言和显示模式
            EEprom.dspmode=DspMode;
            EEP_WriteBytes(0x0000,&EEprom,sizeof(Env_t));         

 //           Language==0?LCD_print_line(3,1,"完成,\xd5\xfd在重启",0):LCD_print_line(3,1,"Done,will reset",0);

						{
								LCD_print_line(2,1,"     Done      ",0);
								LCD_print_line(6,1,"   will reset  ",0);
						}
            delayms(1500);
            POWERON_FLG=0x55AA;
            ISPCR=0X60;
            break;
        }
        ClearWatchDog();
    }     
    
    delayms(50);    
}

void menu11_sbu1_no(void)
{   
    clear_screen();
   LCD_print_line(3,1,"Confirm reset？",0);
    LCD_print(5,10,menu10_text[Language][1],0);
    LCD_print(5,66,menu10_text[Language][3],1);  
    
    delayms(50);    
}
     

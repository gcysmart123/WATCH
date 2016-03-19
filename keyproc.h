#ifndef __KEYPROCK_H
#define __KEYPROCK_H

#include "MA82G5B32_RTC.H"
#include "Keyproc.h"
#include "menu.h"

#define RF_COUNTER    500u     //最大存储多少码表
#define ALARM_CNT     10u
#define RFLIST_QTY    50u        //RF列表数量  
#define RF_EEPROM_BASE_ADDR  0X40    //RF例表存放在EEPROM的起始地址
#define RF_KEY_EEPROM_ADDR   0XC00  //存放键值的起始地址
#define SERVICE_TYPE_EEPROM_ADDR   0X900  //存放键值的起始地址
#define Beep(x)     /*BUZZER=0;*/{BeepMs=x;T2MOD|=1<<1;bBeep=1;TR2=1;TR0=1;}
#define Shake(x)    {ShakeMs=x;SHAKE=1;bShake=1;TR0=1;}
#define Light(x)    {LED=0;LightMs=x;bLight=1;TR0=1;}

#define LCDBK_ON(sec)   {LCDBK=0;BkLightTim=sec;}    //LCD背光打开时间
#define LCDBK_OFF()     {LCDBK=1;BkLightTim=0;}      //关闭LCD背光
#define MenuTimout(sec) {EA=0;MenuTim=sec;EA=1;}               //sec秒无操作会回到主菜单

#define ClearWatchDog()  WDTCR=0XBF;


typedef struct Env_struct{
    uint8_t language;     //语言
    uint8_t workmode;     //工作模式
    uint8_t cleartim;     //定时消号时间
    uint8_t cyclingtim;   //循环时间
    uint8_t prompttim;    //提示时间
    uint8_t dspmode;      //显示模式
    uint8_t keyboardno;   //键盘号
    uint8_t promptLight;  //提示LED
    uint8_t promptShake;  //振动
    uint8_t prompBeep;    //蜂鸣
}Env_t;



//RF存储在EE中的位置为0X0000~0X1C00
typedef struct {
    uint8_t region[4];      //区号0~9~A~Z; //存放的ASCII码
    uint32_t rf;            //RF地址码，高8位为0表示此块已经被使用。其它为未使用 
}RF_def;

//存储在EE中的位置为0X1840~18F8
// xdata uint8_t RF_KEY[36][6];     //键位功能


typedef struct{
    uint8_t region[4];
    uint8_t SrvNo;    //服务代号
    uint8_t cnt;      //呼叫次数
    uint32_t Tim;     //呼叫的时间
    uint8_t use;      //链表是否被使用
}RF_list_def;


extern uint8_t index;
extern xdata uint8_t MenuMode;        //菜单显示模式,0为无进入菜单，1。系统设置菜单。2.RF显示菜单。3，长按返回键菜单
extern bit repaintMenu;

extern uint8_t gKeyValue;            //键值
extern xdata uint8_t LongPressSec;   //按键是否是长按

extern xdata RF_list_def RFlist[];

extern xdata int8_t ListIndex;
extern xdata int8_t ListLine;
extern xdata int8_t bPrompt;
extern xdata int8_t CurrentIndex;
extern xdata int8_t Count;


extern xdata int16_t beepInterval;
extern bit bBeep;      //蜂鸣打开标志
extern bit bLight;     //闪灯标志
extern bit bShake;     //振动标志
extern bit bCharging;   //充电中
extern bit bBkLight;

extern bit bPowerDown;  //掉电模式
extern bit bClockAlarm;  //有闹铃

extern xdata int16_t LightMs;     //闪灯时间，毫秒
extern xdata uint16_t BeepMs;      //蜂鸣响的时间，毫秒
extern xdata uint16_t ShakeMs;     //振动时间，毫秒


extern xdata uint16_t EEP_RFCnt;     //RF存放在EE在的数量

extern xdata uint8_t BkLightTim;    //LCD背光亮时间
extern xdata uint8_t MenuTim;       //菜单不操作时反回时间

extern xdata uint16_t ADCTim;       //获取ADC时间

extern xdata int8_t ClearTim;       //定时清除时间
extern xdata int8_t ClearTim_tmp;

extern xdata int8_t CyclingTim;   //循环时间
extern xdata int8_t CyclingTim_tmp;   //循环时间

extern xdata int8_t PromptTim;
extern xdata int8_t KeyBoardNo;  //键盘号
extern xdata int16_t PromptTim_tmp;

extern xdata Env_t EEprom;

void delayms(unsigned int t) reentrant;
unsigned char key_scan(void);
void keyproc(void);

void Display_Time(void);
void env_init(void);   //环境变量初始化

void Display_Call(uint8_t line,int8_t idx,int8_t bHlight); //呼叫显示

int8_t parse_sevice(RF_list_def *rf,uint32_t dat);  //解析无线接收

void Add_call_list(RF_list_def *rf);   //增加呼叫服务列表,使用先进去的存在首位

void Del_call_list(int8_t idx);     //从列表里删除一个RF

uint8_t RF_list_cnt(void);

void DisplayCall(uint32_t dat);    //显示呼叫菜单
void Display_Time_Line(uint8_t line);
//进入掉电模式
void Power_Down(void);
void BAT_sign(void);
void Diaplay_BAT(void);

void displayRF(void);
void Display_RF_list(void);
void menu_RF_list(void );
void menu_RF_up(void);
void menu_RF_down(void);
void menu_RF_del(void);

void Draw_Initialize(void);

//周期性的刷新时间
void Refresh_Time_Priodic(void);


//查找一个RF在EE中的位置，dat为入RF地址
//int8_t Del_OneRF_EE(uint32_t dat);
//查找一个RF在EE中的位置p为存放查找到的地址，dat为入RF表
int8_t Find_RF_EEPROM(RF_def *p ,uint32_t dat);
//删除一个RF在EEPROM中的存储
int8_t Del_RF_EEPROM(uint8_t *p);
//删除全部EEPROM里的RF
void Del_all_RF_EEPROM(void);
//共存有多少个RF在EE里
uint16_t RF_EEPROM_Cnt(void);
 //存储一个RF列表到EE
int8_t Store_RF_TO_EEPROM(RF_def *pRF);
//查找对应的区号里的编号有没有存相关数据,有返回地址
//uint16_t Find_RF_NO_inEE(uint8_t *rgn);
//存储对应区号的键值
void Store_Key_Fun(uint8_t rgn,uint8_t *Key);
//查找对应区号的值值
void Find_Key_Fun(uint8_t rgn,uint8_t *Key);

void Test_PowerOn(void);
void Test_Power_ADC(void);

#endif

#ifndef __MENU_H
#define __MENU_H

#include "24c16.h"

typedef enum
{
	ONE_KEY= 0,     //单键模式
	MULTI_KEY = 1,  //多键模式

}WORKMODE_t;

struct Menu_def 
{
    uint8_t Index;               //本菜单索引号
    uint8_t Enter;               //按确认确时的索引
    uint8_t Up;                  //按UP键时的索引
    uint8_t Down;                //按DOWN键时的索引
    uint8_t Return;              //要执行的函数
    void (*handle)(void);    //本单的标题
};

extern code struct Menu_def MenuProc[];
extern xdata int8_t WorkMode;   //工作模式
extern xdata int8_t DspMode;

extern int8_t MsgBeep;     //收到呼叫是否蜂鸣,默认使能的
extern int8_t MsgLight;    //收到呼叫是否闪灯
extern int8_t MsgShake;    //收到呼叫是否振动

extern bit bSignalSet;   //信号接收设置
extern bit bAlarmSet;
extern bit bPowerSet;

extern int8_t Language; 
extern bit repaint;
extern xdata int8_t index_tmp;


/**********************************************************/

void main_menu(void);
void menu1(void);
void menu2(void);
void menu3(void);
void menu4(void);
void menu5(void);
void menu6(void);
void menu7(void);
void menu8(void);
void menu9(void);
void menu10(void);
void menu11(void);
void menu12(void);
void menu12(void);
void menu13(void);
void menu14(void);

void menu_4_sub1_1(void) ;//时     //时间日期设置
void menu_4_sub1_2(void) ;//分
void menu_4_sub1_3(void) ;//秒
void menu_4_sub1_4(void) ;//年
void menu_4_sub1_5(void) ;//月
void menu_4_sub1_6(void) ;//日

void menu2_AddCaller(void);    //注册呼叫器
void menu3_DelCaller(void);
void Add_Transmit(uint32_t dat); //学习添加码,主循环里调用


//即时显示
void menu4_LiveView(void);
//排队显示
void menu4_Queue(void);

//消号时间设置
void menu5_sub1(void);
void menu5_sub1_up(void);
void menu5_sub1_down(void);

//循环时间设置
void menu6_sub1(void);
void menu6_sub1_up(void);
void menu6_sub1_down(void);

//呼叫提示设置
void menu7_sub1_1(void);
void menu7_sub1_2(void);

void menu7_sub2_Light(void);
void menu7_sub2_Sound(void);
void menu7_sub2_Shake(void);

void menu7_promptTime(void);
void menu7_sub2_up(void);
void menu7_sub2_down(void);


//模式设置
void Menu_Mode_Set_1(void);
void Menu_Mode_Set_2(void);

//键位功能设置
void menu10_sub1(void);
void menu10_sub1_up(void);
void menu10_sub1_down(void);
void menu10_sbu1_1(void);
void menu10_sbu1_2(void);
void menu10_sbu1_set(void);
void menu10_sbu1_yes(void);
void menu10_sbu1_no(void);
//设置键位功能
void Set_Region_Key(uint32_t dat);

//语言选择
void menu12_Chinese(void);
void menu12_English(void);

//恢复出厂值
void menu_factory_eset(void);


//长按返回键时
void RF_Power_Set(void);
void PowerDown_Set(void);
void PowerDown_Set_sjz(void);
void Rest_Set(void);

// 键盘设置
void Menu11_KeyBoard(void);

void byte_to_str(uint8_t dat,char *str);
void int_to_str(uint16_t dat,char *str);

void Display_Time_EndLine(void);
void Display_Call(uint8_t line,int8_t idx,int8_t bHlight);
void displayRF(void);
void menu_RF_del(void);
//从列表里删除一个RF
void Del_call_list(int8_t idx);
void RF_List_remove(int8_t CurrentIndex);

void menu_RF_remove_single(void);
void menu_RF_remove_all(void);
void do_del_RF_single(void);

void menu11_sbu1_yes(void);
void menu11_sbu1_no(void);

/*sjz 20160308*/
void menu10_sub1_type_set(void);
void menu10_sub1_area_set(void);
void menu10_sub2(void);
void menu10_sub2_up(void);
void menu10_sub2_down(void);
void menu10_sub2_enter(void);

#endif

#ifndef __DEFINE_H_
#define __DEFINE_H_

//12864引脚定义
#define  LCD12864_RS        P35
#define  LCD12864_CS        P41
#define  LCD12864_SID       P33
#define  LCD12864_SCLK      P34
#define  LCD12864_RST       P40
#define  LCDBK              P26

//24c64引脚定义
#define   SCL     P30       //复位脚
#define   SDA     P31       
// #define   RST     P14      

//PT2240的接收脚定义
#define     SDIN      P22      //接收引脚

#define     POWER_CS  P32      //接收模块的电源开关

//蜂鸣器
#define     BUZZER      P10   
                       // P12电池电压，P11充电电压
//振动
#define     SHAKE    P24

//按键定义，使用KBI键盘中断
#define     KEY_OK      P27
#define     KEY_RETURN  P20
#define     KEY_UP      P21
#define     KEY_DOWN    P25

//ADC测试电池电量
#define     ADC_USB     P11
#define     ADC_BAT     P12

//LED
#define     LED         P15


typedef enum {
    KEY_VALUE_OK=1,
    KEY_VALUE_UP=2,
    KEY_VALUE_DOWN=3,
    KEY_VALUE_RETURN=4
}Key_def;




#endif

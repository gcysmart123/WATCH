#ifndef __DEFINE_H_
#define __DEFINE_H_

//12864���Ŷ���
#define  LCD12864_RS        P35
#define  LCD12864_CS        P41
#define  LCD12864_SID       P33
#define  LCD12864_SCLK      P34
#define  LCD12864_RST       P40
#define  LCDBK              P26

//24c64���Ŷ���
#define   SCL     P30       //��λ��
#define   SDA     P31       
// #define   RST     P14      

//PT2240�Ľ��սŶ���
#define     SDIN      P22      //��������

#define     POWER_CS  P32      //����ģ��ĵ�Դ����

//������
#define     BUZZER      P10   
                       // P12��ص�ѹ��P11����ѹ
//��
#define     SHAKE    P24

//�������壬ʹ��KBI�����ж�
#define     KEY_OK      P27
#define     KEY_RETURN  P20
#define     KEY_UP      P21
#define     KEY_DOWN    P25

//ADC���Ե�ص���
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

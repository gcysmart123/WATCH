#ifndef __KEYPROCK_H
#define __KEYPROCK_H

#include "MA82G5B32_RTC.H"
#include "Keyproc.h"
#include "menu.h"

#define RF_COUNTER    500u     //���洢�������
#define ALARM_CNT     10u
#define RFLIST_QTY    50u        //RF�б�����  
#define RF_EEPROM_BASE_ADDR  0X40    //RF��������EEPROM����ʼ��ַ
#define RF_KEY_EEPROM_ADDR   0XC00  //��ż�ֵ����ʼ��ַ
#define SERVICE_TYPE_EEPROM_ADDR   0X900  //��ż�ֵ����ʼ��ַ
#define Beep(x)     /*BUZZER=0;*/{BeepMs=x;T2MOD|=1<<1;bBeep=1;TR2=1;TR0=1;}
#define Shake(x)    {ShakeMs=x;SHAKE=1;bShake=1;TR0=1;}
#define Light(x)    {LED=0;LightMs=x;bLight=1;TR0=1;}

#define LCDBK_ON(sec)   {LCDBK=0;BkLightTim=sec;}    //LCD�����ʱ��
#define LCDBK_OFF()     {LCDBK=1;BkLightTim=0;}      //�ر�LCD����
#define MenuTimout(sec) {EA=0;MenuTim=sec;EA=1;}               //sec���޲�����ص����˵�

#define ClearWatchDog()  WDTCR=0XBF;


typedef struct Env_struct{
    uint8_t language;     //����
    uint8_t workmode;     //����ģʽ
    uint8_t cleartim;     //��ʱ����ʱ��
    uint8_t cyclingtim;   //ѭ��ʱ��
    uint8_t prompttim;    //��ʾʱ��
    uint8_t dspmode;      //��ʾģʽ
    uint8_t keyboardno;   //���̺�
    uint8_t promptLight;  //��ʾLED
    uint8_t promptShake;  //��
    uint8_t prompBeep;    //����
}Env_t;



//RF�洢��EE�е�λ��Ϊ0X0000~0X1C00
typedef struct {
    uint8_t region[4];      //����0~9~A~Z; //��ŵ�ASCII��
    uint32_t rf;            //RF��ַ�룬��8λΪ0��ʾ�˿��Ѿ���ʹ�á�����Ϊδʹ�� 
}RF_def;

//�洢��EE�е�λ��Ϊ0X1840~18F8
// xdata uint8_t RF_KEY[36][6];     //��λ����


typedef struct{
    uint8_t region[4];
    uint8_t SrvNo;    //�������
    uint8_t cnt;      //���д���
    uint32_t Tim;     //���е�ʱ��
    uint8_t use;      //�����Ƿ�ʹ��
}RF_list_def;


extern uint8_t index;
extern xdata uint8_t MenuMode;        //�˵���ʾģʽ,0Ϊ�޽���˵���1��ϵͳ���ò˵���2.RF��ʾ�˵���3���������ؼ��˵�
extern bit repaintMenu;

extern uint8_t gKeyValue;            //��ֵ
extern xdata uint8_t LongPressSec;   //�����Ƿ��ǳ���

extern xdata RF_list_def RFlist[];

extern xdata int8_t ListIndex;
extern xdata int8_t ListLine;
extern xdata int8_t bPrompt;
extern xdata int8_t CurrentIndex;
extern xdata int8_t Count;


extern xdata int16_t beepInterval;
extern bit bBeep;      //�����򿪱�־
extern bit bLight;     //���Ʊ�־
extern bit bShake;     //�񶯱�־
extern bit bCharging;   //�����
extern bit bBkLight;

extern bit bPowerDown;  //����ģʽ
extern bit bClockAlarm;  //������

extern xdata int16_t LightMs;     //����ʱ�䣬����
extern xdata uint16_t BeepMs;      //�������ʱ�䣬����
extern xdata uint16_t ShakeMs;     //��ʱ�䣬����


extern xdata uint16_t EEP_RFCnt;     //RF�����EE�ڵ�����

extern xdata uint8_t BkLightTim;    //LCD������ʱ��
extern xdata uint8_t MenuTim;       //�˵�������ʱ����ʱ��

extern xdata uint16_t ADCTim;       //��ȡADCʱ��

extern xdata int8_t ClearTim;       //��ʱ���ʱ��
extern xdata int8_t ClearTim_tmp;

extern xdata int8_t CyclingTim;   //ѭ��ʱ��
extern xdata int8_t CyclingTim_tmp;   //ѭ��ʱ��

extern xdata int8_t PromptTim;
extern xdata int8_t KeyBoardNo;  //���̺�
extern xdata int16_t PromptTim_tmp;

extern xdata Env_t EEprom;

void delayms(unsigned int t) reentrant;
unsigned char key_scan(void);
void keyproc(void);

void Display_Time(void);
void env_init(void);   //����������ʼ��

void Display_Call(uint8_t line,int8_t idx,int8_t bHlight); //������ʾ

int8_t parse_sevice(RF_list_def *rf,uint32_t dat);  //�������߽���

void Add_call_list(RF_list_def *rf);   //���Ӻ��з����б�,ʹ���Ƚ�ȥ�Ĵ�����λ

void Del_call_list(int8_t idx);     //���б���ɾ��һ��RF

uint8_t RF_list_cnt(void);

void DisplayCall(uint32_t dat);    //��ʾ���в˵�
void Display_Time_Line(uint8_t line);
//�������ģʽ
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

//�����Ե�ˢ��ʱ��
void Refresh_Time_Priodic(void);


//����һ��RF��EE�е�λ�ã�datΪ��RF��ַ
//int8_t Del_OneRF_EE(uint32_t dat);
//����һ��RF��EE�е�λ��pΪ��Ų��ҵ��ĵ�ַ��datΪ��RF��
int8_t Find_RF_EEPROM(RF_def *p ,uint32_t dat);
//ɾ��һ��RF��EEPROM�еĴ洢
int8_t Del_RF_EEPROM(uint8_t *p);
//ɾ��ȫ��EEPROM���RF
void Del_all_RF_EEPROM(void);
//�����ж��ٸ�RF��EE��
uint16_t RF_EEPROM_Cnt(void);
 //�洢һ��RF�б�EE
int8_t Store_RF_TO_EEPROM(RF_def *pRF);
//���Ҷ�Ӧ��������ı����û�д��������,�з��ص�ַ
//uint16_t Find_RF_NO_inEE(uint8_t *rgn);
//�洢��Ӧ���ŵļ�ֵ
void Store_Key_Fun(uint8_t rgn,uint8_t *Key);
//���Ҷ�Ӧ���ŵ�ֵֵ
void Find_Key_Fun(uint8_t rgn,uint8_t *Key);

void Test_PowerOn(void);
void Test_Power_ADC(void);

#endif

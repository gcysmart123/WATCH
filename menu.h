#ifndef __MENU_H
#define __MENU_H

#include "24c16.h"

typedef enum
{
	ONE_KEY= 0,     //����ģʽ
	MULTI_KEY = 1,  //���ģʽ

}WORKMODE_t;

struct Menu_def 
{
    uint8_t Index;               //���˵�������
    uint8_t Enter;               //��ȷ��ȷʱ������
    uint8_t Up;                  //��UP��ʱ������
    uint8_t Down;                //��DOWN��ʱ������
    uint8_t Return;              //Ҫִ�еĺ���
    void (*handle)(void);    //�����ı���
};

extern code struct Menu_def MenuProc[];
extern xdata int8_t WorkMode;   //����ģʽ
extern xdata int8_t DspMode;

extern int8_t MsgBeep;     //�յ������Ƿ����,Ĭ��ʹ�ܵ�
extern int8_t MsgLight;    //�յ������Ƿ�����
extern int8_t MsgShake;    //�յ������Ƿ���

extern bit bSignalSet;   //�źŽ�������
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

void menu_4_sub1_1(void) ;//ʱ     //ʱ����������
void menu_4_sub1_2(void) ;//��
void menu_4_sub1_3(void) ;//��
void menu_4_sub1_4(void) ;//��
void menu_4_sub1_5(void) ;//��
void menu_4_sub1_6(void) ;//��

void menu2_AddCaller(void);    //ע�������
void menu3_DelCaller(void);
void Add_Transmit(uint32_t dat); //ѧϰ�����,��ѭ�������


//��ʱ��ʾ
void menu4_LiveView(void);
//�Ŷ���ʾ
void menu4_Queue(void);

//����ʱ������
void menu5_sub1(void);
void menu5_sub1_up(void);
void menu5_sub1_down(void);

//ѭ��ʱ������
void menu6_sub1(void);
void menu6_sub1_up(void);
void menu6_sub1_down(void);

//������ʾ����
void menu7_sub1_1(void);
void menu7_sub1_2(void);

void menu7_sub2_Light(void);
void menu7_sub2_Sound(void);
void menu7_sub2_Shake(void);

void menu7_promptTime(void);
void menu7_sub2_up(void);
void menu7_sub2_down(void);


//ģʽ����
void Menu_Mode_Set_1(void);
void Menu_Mode_Set_2(void);

//��λ��������
void menu10_sub1(void);
void menu10_sub1_up(void);
void menu10_sub1_down(void);
void menu10_sbu1_1(void);
void menu10_sbu1_2(void);
void menu10_sbu1_set(void);
void menu10_sbu1_yes(void);
void menu10_sbu1_no(void);
//���ü�λ����
void Set_Region_Key(uint32_t dat);

//����ѡ��
void menu12_Chinese(void);
void menu12_English(void);

//�ָ�����ֵ
void menu_factory_eset(void);


//�������ؼ�ʱ
void RF_Power_Set(void);
void PowerDown_Set(void);
void PowerDown_Set_sjz(void);
void Rest_Set(void);

// ��������
void Menu11_KeyBoard(void);

void byte_to_str(uint8_t dat,char *str);
void int_to_str(uint16_t dat,char *str);

void Display_Time_EndLine(void);
void Display_Call(uint8_t line,int8_t idx,int8_t bHlight);
void displayRF(void);
void menu_RF_del(void);
//���б���ɾ��һ��RF
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

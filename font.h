#ifndef __FONT_H
#define __FONT_H

// ------------------  ������ģ�����ݽṹ���� ------------------------ //
typedef struct typFNT_GB12      // ������ģ���ݽṹ
{
       unsigned char Index[2];  // ������������
       char Msk[24];            // ����������
};
extern code struct typFNT_GB12  GB_12[];


#define KEY_NUM   40     //��������


extern code unsigned char *KeySevice[][KEY_NUM];
extern code unsigned char Sign_8X5[][5];
extern code unsigned char ASCII_12X16[11][36];
extern code unsigned char ASCII_8X5[][5];
extern unsigned char code ASCII_16X8[][16];

extern code char* WeekDay[][7];
extern code char *Bye[1];

#define  OFFSET    0X10      //ASCII�������ƫ��

#endif

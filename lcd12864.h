#ifndef __LCD12864_H
#define __LCD12864_H

#include <REG_MA82G5B32.H>
#include "define.h"


#define uchar unsigned char
#define uint unsigned int
#define ulong unsigned long
  
void transfer_command(uchar data1);
void transfer_data(uchar data1);
void initial_lcd(void);
void lcd_address(uchar page,uchar column);
void clear_screen(void);
// void full_display(uchar data_left,uchar data_right);
// void display_graphic_32x32(uchar page,uchar column,uchar *dp);
// void display_graphic_16x16(uchar page,uchar column,uchar *dp);
// void display_string_8x16(uint page,uint column,uchar *text,uchar bH);
// void display_string_5x7(uint page,uint column,uchar *text);
// void display_one_line_16X12(uchar page,uchar column,uchar *pCode[]);
void display_graphic_24x12(uchar page,uchar column,uchar *dp);  
void Display_ASCII_8x5(uchar page,uchar column,uchar *dp,uchar bH);
void display_graphic_16x12(uchar page,uchar column,uchar *dp);
// void Highlight_one_line_16X12(uchar page,uchar column,uchar *pCode[]);
void display_ASCII_16x8(uchar page,uchar column,uchar *dp,uchar bH);

// void HighLight_Hanzi_16x12(uchar page,uchar column,uchar *dp);  //������ʾ16X12��������
// void Highlight_Hanzis_16X12(uchar page,uchar column,uchar *pCode[]);  //������ʾ�����ִ�

//���һ���ϵ����ݡ�
void Fill_one_line(uchar page,uchar column,uchar dat,uchar len);

//��ӡ8X5�ַ���
void print_str8x5(uchar page,uchar column,char *s,uchar bH);
void print_str8x5_sjz(uchar page,uchar column,char *s,uchar bH);
void print_str16x8_sjz(uchar page,uchar column,char *s,uchar bH);
//��ӡ16X8�ַ���
// void print_str16x8(uchar page,uchar column,char *s,uchar  bH) ;
// void printf_str16x8_line(uchar page,char *s,uchar bH);

//LCD��ӡ���ֻ��ַ���
void LCD_print(uchar page,uchar column,uchar *s,uchar bH);
//LCD��ӡһ����
void LCD_print_line(uchar page,uchar column,uchar *s,uchar bH);
#endif


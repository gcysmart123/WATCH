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

// void HighLight_Hanzi_16x12(uchar page,uchar column,uchar *dp);  //反白显示16X12单个汉字
// void Highlight_Hanzis_16X12(uchar page,uchar column,uchar *pCode[]);  //反白显示汉字字串

//填充一行上的数据。
void Fill_one_line(uchar page,uchar column,uchar dat,uchar len);

//打印8X5字符串
void print_str8x5(uchar page,uchar column,char *s,uchar bH);
void print_str8x5_sjz(uchar page,uchar column,char *s,uchar bH);
void print_str16x8_sjz(uchar page,uchar column,char *s,uchar bH);
//打印16X8字符串
// void print_str16x8(uchar page,uchar column,char *s,uchar  bH) ;
// void printf_str16x8_line(uchar page,char *s,uchar bH);

//LCD打印汉字或字符串
void LCD_print(uchar page,uchar column,uchar *s,uchar bH);
//LCD打印一行字
void LCD_print_line(uchar page,uchar column,uchar *s,uchar bH);
#endif


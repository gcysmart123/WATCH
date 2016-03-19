#include "lcd12864.h" 
#include "font.h" 
#include <intrins.h>

void delayms(unsigned int t) reentrant;

void lcd12864_send_data(uchar data1)
{
    unsigned char i;
    for(i=0;i<8;i++)
    {
        LCD12864_SCLK=0;
        if(data1&0x80) LCD12864_SID=1;
        else LCD12864_SID=0;
        LCD12864_SCLK=1;
        data1=data1<<=1;
    }
}



  
/******************************************************************** 
* 名称 : void transfer_command(int data1) 
* 功能 : 写指令到 LCD 模块 
* 输入 : z 
* 输出 : 无 
***********************************************************************/  
void transfer_command(uchar data1)
{
//     unsigned char i;
    LCD12864_CS=0;
    LCD12864_RS=0;
//     for(i=0;i<8;i++)
//     {
//         LCD12864_SCLK=0;
//         if(data1&0x80) LCD12864_SID=1;
//         else LCD12864_SID=0;
//         LCD12864_SCLK=1;
//         data1=data1<<=1;
//     }
    lcd12864_send_data(data1);
}


void transfer_data(uchar data1)
{
//     char i;
    LCD12864_CS=0;
    LCD12864_RS=1;
//     for(i=0;i<8;i++)
//     {
//         LCD12864_SCLK=0;
//         if(data1&0x80) LCD12864_SID=1;
//         else LCD12864_SID=0;
//         LCD12864_SCLK=1;
//         data1=data1<<=1;
//     }
    lcd12864_send_data(data1);
}






// void delay(int i)
// {
//     int j,k;
//     for(j=0;j<i;j++)
//     for(k=0;k<110;k++);
// }


void initial_lcd(void)
{
//     P4M0|=0X01;
//     P4M0|=1<<1;
//     P4M0|=1<<5;
//     P2M0|=1<<6;
    P4M0|=0X63;

    LCD12864_CS=0;
    LCD12864_RST=0; /*低电平复位*/
    delayms(100);
    LCD12864_RST=1;
    /*复位完毕*/
    delayms(20);
    transfer_command(0xe2);
    /*软复位*/
    delayms(5);
    transfer_command(0x2c); /*升压步聚 1*/
    delayms(5);
    transfer_command(0x2e); /*升压步聚 2*/
    delayms(5);
    transfer_command(0x2f); /*升压步聚 3*/
    delayms(5);
    transfer_command(0x24); /*粗调对比度，可设置范围 0x20～0x27*/
    transfer_command(0x81); /*微调对比度*/
    transfer_command(0x1a); /*0x1a,微调对比度的值，可设置范围 0x00～0x3f*/
    transfer_command(0xa2); /*1/9 偏压比（bias）*/
    transfer_command(0xc8); /*行扫描顺序：从上到下*/
    transfer_command(0xa0); /*列扫描顺序：从左到右*/
    transfer_command(0x40); /*起始行：第一行开始*/
    transfer_command(0xaf); /*开显示*/
    LCD12864_CS=1;
    
    clear_screen();
}


void lcd_address(uchar page,uchar column)
{
    LCD12864_CS=0;
    column=column-1;
    //我们平常所说的第 1 列，在 LCD 驱动 IC 里是第 0 列。所以在这里减去 1.
    page=page-1;
    transfer_command(0xb0+page);
    //设置页地址。每页是 8 行。一个画面的 64 行被分成 8 个页。我们平常所说的第 1 页，在 LCD 驱动IC 里是第 0 页，所以在这里减去 1*/
    transfer_command(((column>>4)&0x0f)+0x10); //设置列地址的高 4 位
    transfer_command(column&0x0f);
    //设置列地址的低 4 位
}

void clear_screen(void)
{
    unsigned char i,j;
    LCD12864_CS=0;
    for(i=0;i<9;i++)
    {
        lcd_address(1+i,1);
        for(j=0;j<132;j++)
        {
            transfer_data(0x00);
        }
    }
    LCD12864_CS=1;
}


void Display_ASCII_8x5(uchar page,uchar column,uchar *dp,uchar bH)
{
    uchar i;
    lcd_address(page,column);
    for (i=0;i<5;i++)
    {
        bH?transfer_data(~*dp):transfer_data(*dp);
        /*写数据到 LCD,每写完一个 8 位的数据后列地址自动加 1*/
        dp++;
    }
    LCD12864_CS=1;
}

static xdata uchar bHighlight=0;

void display_graphic_16x12(uchar page,uchar column,uchar *dp)
{
    uchar i,j;

    for(j=0;j<2;j++)
    {
        lcd_address(page+j,column);
        for (i=0;i<12;i++)
        {
            if(!bHighlight)
            {
                transfer_data(*dp);
            }
            else
            {
                transfer_data(~*dp);
            }
            /*写数据到 LCD,每写完一个 8 位的数据后列地址自动加 1*/
            dp++;
        }
    }
    LCD12864_CS=1;
}

void display_graphic_24x12(uchar page,uchar column,uchar *dp)
{
    uchar i,j;

    for(j=0;j<3;j++)
    {
        lcd_address(page+j,column);
        for (i=0;i<12;i++)
        {
            transfer_data(*dp);
            /*写数据到 LCD,每写完一个 8 位的数据后列地址自动加 1*/
            dp++;
        }
    }
    LCD12864_CS=1;
}

// void display_string_5x7(uint page,uint column,uchar *text)
// {
//     uint i=0,j,k;
//     LCD12864_CS=0;
//     while(text[i]>0x00)
//     {
//         if((text[i]>=0x20)&&(text[i]<0x7e))
//         {
//             j=text[i]-0x20;
//             lcd_address(page,column);
//             for(k=0;k<5;k++)
//             {
//                 transfer_data(ascii_table_5x7[j][k]);/*显示 5x7 的 ASCII 字到 LCD 上，y 为页地址，x 为列地址，最后为数据*/
//             }
//             i++;
//             column+=6;
//         }
//         else
//         i++;
//     }
// }



void display_ASCII_16x8(uchar page,uchar column,uchar *dp,uchar  bH)
{
    uchar i,j;

    for(j=0;j<2;j++)
    {
        lcd_address(page+j,column);
        for (i=0;i<8;i++)
        {
            if(!bH)
            {
                transfer_data(*dp);
            }
            else
            {
                transfer_data(~*dp);
            }
            /*写数据到 LCD,每写完一个 8 位的数据后列地址自动加 1*/
            dp++;
        }
    }
    LCD12864_CS=1;
}


//填充一行上的数据。
void Fill_one_line(uchar page,uchar column,uchar dat,uchar len)
{

    lcd_address(page,column);
    for (;len>0;len--)
    {
        transfer_data(dat);   
    }
    LCD12864_CS=1;
}


//打印8X5字符串
void print_str8x5(uchar page,uchar column,char *s,uchar bH)
{
    while(*s)
    {
        Display_ASCII_8x5(page,column,ASCII_8X5[*s-0X20],bH);
        s++;
        column+=6;
    }
}
void print_str8x5_sjz(uchar page,uchar column,char *s,uchar bH)
{
    unsigned char i=0;
    while((*s)&&(i<12))
    {
        Display_ASCII_8x5(page,column,ASCII_8X5[*s-0X20],bH);
        s++;
	 i++;
        column+=6;
    }
}
void print_str16x8_sjz(uchar page,uchar column,char *s,uchar bH)
{
    unsigned char i=0;
    while((*s)&&(i<12))
    {
        display_ASCII_16x8(page,column,ASCII_16X8[*s-0X20],bH);
        s++;
	 i++;
        column+=9;
    }
}
//打印16X8字符串
// void print_str16x8(uchar page,uchar column,char *s,uchar  bH) 
// {
//     char *p=s,i=0;
//     while(*p)
//     {
//         i++;
//         p++;
//     }
//     i-=1;
//     if(bH)
//     {
//         Fill_one_line(page,column,0xff,i*9);
//         Fill_one_line(page+1,column,0xff,i*9);
//     }
//     else
//     {
//         Fill_one_line(page,column,0,i*9);
//         Fill_one_line(page+1,column,0,i*9);
//     }
//     while(*s)
//     {
//         display_ASCII_16x8(page,column,ASCII_16X8[*s-0X20],bH);
//         s++;
//         column+=9;
//     }
// }





unsigned char Find_GB12(unsigned char *coded)
{
    uchar i;    
    for(i=0;GB_12[i].Index!=0;i++)
    {
        if((*coded==GB_12[i].Index[0])&&(*(coded+1)==GB_12[i].Index[1]))
        {
            return i;
        }
        else if(i>240)  //最大240个汉字，安全，防无限循环
        {
            break;
        }
    }
    return 0;
}


//汉字和字符串打印
void LCD_print(uchar page,uchar column,uchar *s,uchar bH)
{
    uchar Idx;
    uchar tmp;
    bH?(tmp=0xff):(tmp=0);
    while(*s)
    {
        if(*s>=0xA0)      //是GB2312编码汉字
        {
            Idx=Find_GB12(s);
            if(!Idx) return;
            bHighlight=bH;
            display_graphic_16x12(page,column,GB_12[Idx].Msk);
            bHighlight=0;
            s+=2;
            column+=13;       
        }
        else if(*s>=0x20)  //是ASCII码
        {
            display_ASCII_16x8(page,column,ASCII_16X8[*s-0X20],bH);   
            s++;
            column+=9;
        }
        else               //非打印字符
        {
            return;
        }
        Fill_one_line(page,column-1,tmp,1);   
        Fill_one_line(page+1,column-1,tmp,1);
        
        if(column>127)        //一行显示满换到下一行
        {
            column=1;page+=2;
        }
    }
}


//LCD打印一行字
void LCD_print_line(uchar page,uchar column,uchar *s,uchar bH)
{
    uchar tmp;
    bH?(tmp=0xff):(tmp=0);
    Fill_one_line(page,1,tmp,128);   
    Fill_one_line(page+1,1,tmp,128);
    LCD_print(page,column,s,bH);
}


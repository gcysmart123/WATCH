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
* ���� : void transfer_command(int data1) 
* ���� : дָ� LCD ģ�� 
* ���� : z 
* ��� : �� 
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
    LCD12864_RST=0; /*�͵�ƽ��λ*/
    delayms(100);
    LCD12864_RST=1;
    /*��λ���*/
    delayms(20);
    transfer_command(0xe2);
    /*��λ*/
    delayms(5);
    transfer_command(0x2c); /*��ѹ���� 1*/
    delayms(5);
    transfer_command(0x2e); /*��ѹ���� 2*/
    delayms(5);
    transfer_command(0x2f); /*��ѹ���� 3*/
    delayms(5);
    transfer_command(0x24); /*�ֵ��Աȶȣ������÷�Χ 0x20��0x27*/
    transfer_command(0x81); /*΢���Աȶ�*/
    transfer_command(0x1a); /*0x1a,΢���Աȶȵ�ֵ�������÷�Χ 0x00��0x3f*/
    transfer_command(0xa2); /*1/9 ƫѹ�ȣ�bias��*/
    transfer_command(0xc8); /*��ɨ��˳�򣺴��ϵ���*/
    transfer_command(0xa0); /*��ɨ��˳�򣺴�����*/
    transfer_command(0x40); /*��ʼ�У���һ�п�ʼ*/
    transfer_command(0xaf); /*����ʾ*/
    LCD12864_CS=1;
    
    clear_screen();
}


void lcd_address(uchar page,uchar column)
{
    LCD12864_CS=0;
    column=column-1;
    //����ƽ����˵�ĵ� 1 �У��� LCD ���� IC ���ǵ� 0 �С������������ȥ 1.
    page=page-1;
    transfer_command(0xb0+page);
    //����ҳ��ַ��ÿҳ�� 8 �С�һ������� 64 �б��ֳ� 8 ��ҳ������ƽ����˵�ĵ� 1 ҳ���� LCD ����IC ���ǵ� 0 ҳ�������������ȥ 1*/
    transfer_command(((column>>4)&0x0f)+0x10); //�����е�ַ�ĸ� 4 λ
    transfer_command(column&0x0f);
    //�����е�ַ�ĵ� 4 λ
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
        /*д���ݵ� LCD,ÿд��һ�� 8 λ�����ݺ��е�ַ�Զ��� 1*/
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
            /*д���ݵ� LCD,ÿд��һ�� 8 λ�����ݺ��е�ַ�Զ��� 1*/
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
            /*д���ݵ� LCD,ÿд��һ�� 8 λ�����ݺ��е�ַ�Զ��� 1*/
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
//                 transfer_data(ascii_table_5x7[j][k]);/*��ʾ 5x7 �� ASCII �ֵ� LCD �ϣ�y Ϊҳ��ַ��x Ϊ�е�ַ�����Ϊ����*/
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
            /*д���ݵ� LCD,ÿд��һ�� 8 λ�����ݺ��е�ַ�Զ��� 1*/
            dp++;
        }
    }
    LCD12864_CS=1;
}


//���һ���ϵ����ݡ�
void Fill_one_line(uchar page,uchar column,uchar dat,uchar len)
{

    lcd_address(page,column);
    for (;len>0;len--)
    {
        transfer_data(dat);   
    }
    LCD12864_CS=1;
}


//��ӡ8X5�ַ���
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
//��ӡ16X8�ַ���
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
        else if(i>240)  //���240�����֣���ȫ��������ѭ��
        {
            break;
        }
    }
    return 0;
}


//���ֺ��ַ�����ӡ
void LCD_print(uchar page,uchar column,uchar *s,uchar bH)
{
    uchar Idx;
    uchar tmp;
    bH?(tmp=0xff):(tmp=0);
    while(*s)
    {
        if(*s>=0xA0)      //��GB2312���뺺��
        {
            Idx=Find_GB12(s);
            if(!Idx) return;
            bHighlight=bH;
            display_graphic_16x12(page,column,GB_12[Idx].Msk);
            bHighlight=0;
            s+=2;
            column+=13;       
        }
        else if(*s>=0x20)  //��ASCII��
        {
            display_ASCII_16x8(page,column,ASCII_16X8[*s-0X20],bH);   
            s++;
            column+=9;
        }
        else               //�Ǵ�ӡ�ַ�
        {
            return;
        }
        Fill_one_line(page,column-1,tmp,1);   
        Fill_one_line(page+1,column-1,tmp,1);
        
        if(column>127)        //һ����ʾ��������һ��
        {
            column=1;page+=2;
        }
    }
}


//LCD��ӡһ����
void LCD_print_line(uchar page,uchar column,uchar *s,uchar bH)
{
    uchar tmp;
    bH?(tmp=0xff):(tmp=0);
    Fill_one_line(page,1,tmp,128);   
    Fill_one_line(page+1,1,tmp,128);
    LCD_print(page,column,s,bH);
}


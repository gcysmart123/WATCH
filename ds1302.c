#include <REG_MA82G5B32.H>
#include "MA82G5B32_IAP.h"
#include "ds1302.h"
#include "define.h"
#include "intrins.h"

#define  DS1302_CLK  P44              //实时时钟时钟线引脚 
#define  DS1302_IO   P45              //实时时钟数据线引脚 
#define  DS1302_RST  P14              //实时时钟复位线引脚

void delayus(unsigned int n)
{
    unsigned char i;
    while(n--)
    {
        for(i=0;i<10;i++);
    }
}

 /*************************************************
 名称:ucharDS1302Read()*说明:先写地址，后读数据
*功能:从cmd相应地址中读取一个字节的数据
*调用:DS1302WriteByte(),DS1302ReadByte()
*输入:cmd:要写入的控制字节
*输出:dat:读取的数据
*************************************************/
unsigned char DS1302Read(unsigned char cmd)
{
    unsigned char dat;
    RST=0;  //初始CE线置为0
    SCLK=0;//初始时钟线置为0
    _nop_();
    _nop_();    
    RST=1;  //初始CE置为1，传输开始
    DS1302WriteByte(cmd);   //传输命令字，要读取的时间/日历地址
    dat=DS1302ReadByte();   //读取要得到的时间/日期
    SCLK=1;     //时钟线拉高
    
    RST=0;      //读取结束，CE置为0，结束数据的传输
    return dat; //返回得到的时间/日期
}


/*************************************************名称:DS1302Write
*说明:先写地址，后写数据
*功能:向cmd相应地址中写一个字节的数据
*调用:DS1302WriteByte()
*输入:cmd:要写入的控制字,dat:要写入的数据*输出:无
*************************************************/
void DS1302Write(unsigned char cmd,unsigned char dat)
{
    RST=0;      //初始CE线置为0
    _nop_();
    _nop_();    
    SCLK=0;     //初始时钟线置为0
    _nop_();
    _nop_();    
    RST=1;      //初始CE置为1，传输开始
    DS1302WriteByte(cmd);   //传输命令字，要写入的时间/日历地址
    DS1302WriteByte(dat);   //写入要修改的时间/日期
    SCLK=1;     //时钟线拉高
    _nop_();
    _nop_();    
    RST=0;      //读取结束，CE置为0，结束数据的传输
}


/*************************************************
名称:DS1302WriteByte
*说明:无
*功能:写入8bit数据*调用:delayus()
*输入:dat:要写入的数据*输出:无
*************************************************/
void DS1302WriteByte(unsigned char dat)
{
    unsigned char i;
    SCLK=0;             //初始时钟线置为0
    delayus(2);
    for(i=0;i<8;i++)    //开始传输8个字节的数据
    {
        SDA=dat&0x01;   //取最低位，注意DS1302的数据和地址都是从最低位开始传输的
        delayus(2);
        SCLK=1;         //时钟线拉高，制造上升沿，SDA的数据被传输
        delayus(2);
        SCLK=0;         //时钟线拉低，为下一个上升沿做准备
        dat>>=1;        //数据右移一位，准备传输下一位数据
    }
}


/*************************************************
名称:DS1302ReadByte()
*说明:无
*功能:读取8bit的数据
*调用:delayus()
*输入:无
*输出:dat:读取的数据
*************************************************/
unsigned char DS1302ReadByte()
{
    unsigned char i,dat;
    delayus(2);
    for(i=0;i<8;i++)
    {
        dat>>=1;        //要返回的数据左移一位
        if(SDA==1)      //当数据线为高时，证明该位数据为1
        {
            dat|=0x80;  //要传输数据的当前值置为1,若不是,则为0
        }
        else
        {
            dat&=0x7f;
        }
        SCLK=1;         //拉高时钟线
        delayus(2);
        SCLK=0;         //制造下降沿
        delayus(2);
    }
    return dat;         //返回读取出的数据
}

/*-----------------------------------------
函数名称:DS1302_NReadRam(unsigned char* rstr)
函数功能:多字节突发模式读RAM，DS1302_NRRAM一次可进行31个片内RAM单元读
输入参数:*rstr：存放读到的N个数据
输出参数：无
------------------------------------------*/
void DS1302_burst_Read(unsigned char *rstr)
{
    unsigned char i;
    RST=0;
    _nop_();
    _nop_();    
    SCLK=0;
    RST=1;      //CE拉高，传输开始
    DS1302WriteByte(0XFF);  //写0XFF，多字节突发方式读RAM具体细节见上一节
    for(i=0;i<31;i++)   //连续读取个31字节
    {
        *rstr=DS1302ReadByte(); //此处的ADDRESS指的是你需要进行连续读取的地址
        rstr++;
    }
    RST=0;  //CE信号拉低，传输结束
    SCLK=1;
}


/*---------------------
函数名称:DS1302_NReadRam(unsigned char *rstr)
函数功能:多字节突发模式写RAM，
DS1302_NRRAM一次可进行31个片内RAM单元写输入参数:
*wstr：要被写入的N个数据输出参数：无
-------------------------------------*/
void DS1302_burstWrite(unsigned char *wstr)
{
    unsigned char i;
    unsigned char* tmpstr;
    tmpstr=wstr;
    DS1302Write(0x8e,0x00); //写保护关
    RST=0;
    SCLK=0;
    RST=1;
    DS1302WriteByte(0XFE);  //写0XFE，多字节突发方式写RAMRAM,,具体细节见上节
    for(i=0;i<31;i++)       //连续写入31字节
    {
        DS1302WriteByte(*tmpstr);
        tmpstr++;
    }
    RST=0;
    SCLK=1;
    DS1302Write(0x8e,0x80); //开写保护
}


void Initial_DS1302(void)
{
	unsigned char tmp;
    Time_def time;
    Page_P_SFR_Write(0x4c,0x00); //把P44,P45切换为IO脚
    P4M0|=1<<4;      //P44为推挽 DS1302 CLK为推挽输出
    P4M0&=~(1<<5);   //P45为推挽 DS1302 SDA为推挽输出
    PUCON0=0XFC;
    P1M0|=1<<4;      //P14为推挽输出,RST
    
    P2M0|=1<<6;
    
    tmp=DS1302Read(0XC1);  //读出0XC1 的RAM地址
    
	if(tmp!=0XA5)
    {   
        DS1302Write(0X8E,0);     //禁止写保护
		DS1302Write(0XC0,0XA5);       
        
        time.Year=0x00;           //默认为2000-1-1,星期6，0：0：0 
        time.Month=0x01;
        time.Day=0x1;
        time.Week=1;
        time.Hour=0x00;
        time.Minute=0x00;
        time.Second=0;         

        DS1302Write(0x8e,0x00); //写保护关
        DS1302Write(WRITE_SEC,time.Second);
        DS1302Write(WRITE_MIN,time.Minute );
        DS1302Write(WRITE_HOUR,time.Hour);
        DS1302Write(WRITE_DAY,time.Day);      
        DS1302Write(WRITE_MON,time.Month);
        DS1302Write(WRITE_WEEK,time.Week);
        DS1302Write(WRITE_YEAR,time.Year);
        DS1302Write(0x8e,0x80); //开写保护         
    }
}

//设置时间
void DS1302_SetTime(Time_def* pTime)
{
    DS1302Write(0x8e,0x00); //写保护关
    DS1302Write(WRITE_SEC,pTime->Second);
    DS1302Write(WRITE_MIN,pTime->Minute );
    DS1302Write(WRITE_HOUR,pTime->Hour);
    DS1302Write(WRITE_DAY,pTime->Day);      
    DS1302Write(WRITE_MON,pTime->Month);
    DS1302Write(WRITE_WEEK,pTime->Week);
    DS1302Write(WRITE_YEAR,pTime->Year);
    DS1302Write(0x8e,0x80); //开写保护    
}


//获取时间
void DS1302_GetTime(Time_def* pTime)
{
//     pTime->Second = DS1302Read(READ_SEC);
//     pTime->Minute = DS1302Read(READ_MIN);
//     pTime->Hour = DS1302Read(READ_HOUR);
//     pTime->Day = DS1302Read(READ_DAY);      
//     pTime->Month = DS1302Read(READ_MON);
//     pTime->Week = DS1302Read(READ_WEEK);
//     pTime->Year = DS1302Read(READ_YEAR);    
}








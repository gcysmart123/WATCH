#include <REG_MA82G5B32.H>
#include "MA82G5B32_IAP.h"
#include "ds1302.h"
#include "define.h"
#include "intrins.h"

#define  DS1302_CLK  P44              //ʵʱʱ��ʱ�������� 
#define  DS1302_IO   P45              //ʵʱʱ������������ 
#define  DS1302_RST  P14              //ʵʱʱ�Ӹ�λ������

void delayus(unsigned int n)
{
    unsigned char i;
    while(n--)
    {
        for(i=0;i<10;i++);
    }
}

 /*************************************************
 ����:ucharDS1302Read()*˵��:��д��ַ���������
*����:��cmd��Ӧ��ַ�ж�ȡһ���ֽڵ�����
*����:DS1302WriteByte(),DS1302ReadByte()
*����:cmd:Ҫд��Ŀ����ֽ�
*���:dat:��ȡ������
*************************************************/
unsigned char DS1302Read(unsigned char cmd)
{
    unsigned char dat;
    RST=0;  //��ʼCE����Ϊ0
    SCLK=0;//��ʼʱ������Ϊ0
    _nop_();
    _nop_();    
    RST=1;  //��ʼCE��Ϊ1�����俪ʼ
    DS1302WriteByte(cmd);   //���������֣�Ҫ��ȡ��ʱ��/������ַ
    dat=DS1302ReadByte();   //��ȡҪ�õ���ʱ��/����
    SCLK=1;     //ʱ��������
    
    RST=0;      //��ȡ������CE��Ϊ0���������ݵĴ���
    return dat; //���صõ���ʱ��/����
}


/*************************************************����:DS1302Write
*˵��:��д��ַ����д����
*����:��cmd��Ӧ��ַ��дһ���ֽڵ�����
*����:DS1302WriteByte()
*����:cmd:Ҫд��Ŀ�����,dat:Ҫд�������*���:��
*************************************************/
void DS1302Write(unsigned char cmd,unsigned char dat)
{
    RST=0;      //��ʼCE����Ϊ0
    _nop_();
    _nop_();    
    SCLK=0;     //��ʼʱ������Ϊ0
    _nop_();
    _nop_();    
    RST=1;      //��ʼCE��Ϊ1�����俪ʼ
    DS1302WriteByte(cmd);   //���������֣�Ҫд���ʱ��/������ַ
    DS1302WriteByte(dat);   //д��Ҫ�޸ĵ�ʱ��/����
    SCLK=1;     //ʱ��������
    _nop_();
    _nop_();    
    RST=0;      //��ȡ������CE��Ϊ0���������ݵĴ���
}


/*************************************************
����:DS1302WriteByte
*˵��:��
*����:д��8bit����*����:delayus()
*����:dat:Ҫд�������*���:��
*************************************************/
void DS1302WriteByte(unsigned char dat)
{
    unsigned char i;
    SCLK=0;             //��ʼʱ������Ϊ0
    delayus(2);
    for(i=0;i<8;i++)    //��ʼ����8���ֽڵ�����
    {
        SDA=dat&0x01;   //ȡ���λ��ע��DS1302�����ݺ͵�ַ���Ǵ����λ��ʼ�����
        delayus(2);
        SCLK=1;         //ʱ�������ߣ����������أ�SDA�����ݱ�����
        delayus(2);
        SCLK=0;         //ʱ�������ͣ�Ϊ��һ����������׼��
        dat>>=1;        //��������һλ��׼��������һλ����
    }
}


/*************************************************
����:DS1302ReadByte()
*˵��:��
*����:��ȡ8bit������
*����:delayus()
*����:��
*���:dat:��ȡ������
*************************************************/
unsigned char DS1302ReadByte()
{
    unsigned char i,dat;
    delayus(2);
    for(i=0;i<8;i++)
    {
        dat>>=1;        //Ҫ���ص���������һλ
        if(SDA==1)      //��������Ϊ��ʱ��֤����λ����Ϊ1
        {
            dat|=0x80;  //Ҫ�������ݵĵ�ǰֵ��Ϊ1,������,��Ϊ0
        }
        else
        {
            dat&=0x7f;
        }
        SCLK=1;         //����ʱ����
        delayus(2);
        SCLK=0;         //�����½���
        delayus(2);
    }
    return dat;         //���ض�ȡ��������
}

/*-----------------------------------------
��������:DS1302_NReadRam(unsigned char* rstr)
��������:���ֽ�ͻ��ģʽ��RAM��DS1302_NRRAMһ�οɽ���31��Ƭ��RAM��Ԫ��
�������:*rstr����Ŷ�����N������
�����������
------------------------------------------*/
void DS1302_burst_Read(unsigned char *rstr)
{
    unsigned char i;
    RST=0;
    _nop_();
    _nop_();    
    SCLK=0;
    RST=1;      //CE���ߣ����俪ʼ
    DS1302WriteByte(0XFF);  //д0XFF�����ֽ�ͻ����ʽ��RAM����ϸ�ڼ���һ��
    for(i=0;i<31;i++)   //������ȡ��31�ֽ�
    {
        *rstr=DS1302ReadByte(); //�˴���ADDRESSָ��������Ҫ����������ȡ�ĵ�ַ
        rstr++;
    }
    RST=0;  //CE�ź����ͣ��������
    SCLK=1;
}


/*---------------------
��������:DS1302_NReadRam(unsigned char *rstr)
��������:���ֽ�ͻ��ģʽдRAM��
DS1302_NRRAMһ�οɽ���31��Ƭ��RAM��Ԫд�������:
*wstr��Ҫ��д���N�����������������
-------------------------------------*/
void DS1302_burstWrite(unsigned char *wstr)
{
    unsigned char i;
    unsigned char* tmpstr;
    tmpstr=wstr;
    DS1302Write(0x8e,0x00); //д������
    RST=0;
    SCLK=0;
    RST=1;
    DS1302WriteByte(0XFE);  //д0XFE�����ֽ�ͻ����ʽдRAMRAM,,����ϸ�ڼ��Ͻ�
    for(i=0;i<31;i++)       //����д��31�ֽ�
    {
        DS1302WriteByte(*tmpstr);
        tmpstr++;
    }
    RST=0;
    SCLK=1;
    DS1302Write(0x8e,0x80); //��д����
}


void Initial_DS1302(void)
{
	unsigned char tmp;
    Time_def time;
    Page_P_SFR_Write(0x4c,0x00); //��P44,P45�л�ΪIO��
    P4M0|=1<<4;      //P44Ϊ���� DS1302 CLKΪ�������
    P4M0&=~(1<<5);   //P45Ϊ���� DS1302 SDAΪ�������
    PUCON0=0XFC;
    P1M0|=1<<4;      //P14Ϊ�������,RST
    
    P2M0|=1<<6;
    
    tmp=DS1302Read(0XC1);  //����0XC1 ��RAM��ַ
    
	if(tmp!=0XA5)
    {   
        DS1302Write(0X8E,0);     //��ֹд����
		DS1302Write(0XC0,0XA5);       
        
        time.Year=0x00;           //Ĭ��Ϊ2000-1-1,����6��0��0��0 
        time.Month=0x01;
        time.Day=0x1;
        time.Week=1;
        time.Hour=0x00;
        time.Minute=0x00;
        time.Second=0;         

        DS1302Write(0x8e,0x00); //д������
        DS1302Write(WRITE_SEC,time.Second);
        DS1302Write(WRITE_MIN,time.Minute );
        DS1302Write(WRITE_HOUR,time.Hour);
        DS1302Write(WRITE_DAY,time.Day);      
        DS1302Write(WRITE_MON,time.Month);
        DS1302Write(WRITE_WEEK,time.Week);
        DS1302Write(WRITE_YEAR,time.Year);
        DS1302Write(0x8e,0x80); //��д����         
    }
}

//����ʱ��
void DS1302_SetTime(Time_def* pTime)
{
    DS1302Write(0x8e,0x00); //д������
    DS1302Write(WRITE_SEC,pTime->Second);
    DS1302Write(WRITE_MIN,pTime->Minute );
    DS1302Write(WRITE_HOUR,pTime->Hour);
    DS1302Write(WRITE_DAY,pTime->Day);      
    DS1302Write(WRITE_MON,pTime->Month);
    DS1302Write(WRITE_WEEK,pTime->Week);
    DS1302Write(WRITE_YEAR,pTime->Year);
    DS1302Write(0x8e,0x80); //��д����    
}


//��ȡʱ��
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








#ifndef __DS1302_H
#define __DS1302_H


typedef struct _Time_
{
	unsigned char Second;
	unsigned char Minute;
	unsigned char Hour;
	unsigned char Week;
	unsigned char Day;    
	unsigned char Month;
	unsigned char Year;

}Time_def;


#define WRITE_SEC   0X80
#define WRITE_MIN   0X82
#define WRITE_HOUR  0X84
#define WRITE_DAY   0X86
#define WRITE_MON   0X88
#define WRITE_WEEK  0X8A
#define WRITE_YEAR  0X8C

#define READ_SEC   0X81
#define READ_MIN   0X83
#define READ_HOUR  0X85
#define READ_DAY   0X87
#define READ_MON   0X89
#define READ_WEEK  0X8B
#define READ_YEAR  0X8D



unsigned char DS1302Read(unsigned char cmd);
void DS1302Write(unsigned char cmd,unsigned char dat);
void DS1302WriteByte(unsigned char dat);
unsigned char DS1302ReadByte();
void DS1302_burst_Read(unsigned char *rstr);
// void DS1302_NReadRam(unsigned char *rstr);

void Initial_DS1302(void);
void DS1302_SetTime(Time_def* pTime);
void DS1302_GetTime(Time_def* pTime);




// typedef struct __SYSTEMTIME__
// {
// 	unsigned char Second;
// 	unsigned char Minute;
// 	unsigned char Hour;
// 	unsigned char Week;
// 	unsigned char Day;
// 	unsigned char Month;
// 	unsigned char  Year;
// 	unsigned char DateString[9];
// 	unsigned char TimeString[9];
// }SYSTEMTIME;	//定义的时间类型



// void DS1302InputByte(unsigned char d);
// unsigned char DS1302OutputByte(void); 	//实时时钟读取一字节(内部函数)
// void Write1302(unsigned char ucAddr, unsigned char ucDa);
// unsigned char Read1302(unsigned char ucAddr);
// void DS1302_SetProtect(bit flag) ;
// void DS1302_SetTime(unsigned char Address, unsigned char Value);
// void DS1302_GetTime(SYSTEMTIME *Time);
// void DateToStr(SYSTEMTIME *Time);
// void TimeToStr(SYSTEMTIME *Time);
// void Initial_DS1302(void);



#endif

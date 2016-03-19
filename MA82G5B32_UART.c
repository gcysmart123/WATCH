#include "MA82G5B32_UART.h"

/***********************************************************************
   Function:  void UART0_Init(void)
           
   Description: 串口0初始化，在头文件里可以调节波特率大小。
           
   Input:     None
              
   Output:    None
  
   Return:    None
************************************************************************/
void UART0_Init(void)
{

    S0CON = 0X50;      //串口0工作模式1。并且使能接收。
    
    PCON0 &= ~(1<<7);  //SMOD1=0,禁止双倍速波特率
    S0CFG &= ~(1<<6);  //SMOD2=0,禁止额外双倍速波特率
    
    S0CFG &= ~(1<<7);  // 模式1和模式3时选择定时器1或定时器2作波特率发生器
    
#if (UART0_BAUDRATE_MODE == 1)   //使用定时器1益处做波特率发生器。

	TMOD &= 0x0F;	  //使用定时器1 8位自动重载为波特率发生器
	TMOD |= 0x20;    
    AUXR2 |= 1<<3;    //设置定时器为1T，即SYSCLK。
#if ((SYSCLK/(32*UART0_BAUDRATE)<255)&&(SYSCLK/(32*UART0_BAUDRATE)>0))        //波特率不倍速
    TH1 = 256-SYSCLK/(32*UART0_BAUDRATE);                                   
    PCON0 &= ~(1<<7);  //SMOD1=0,禁止双倍速波特率
    S0CFG &= ~(1<<6);  //SMOD2=0,禁止额外双倍速波特率    
#elif ((SYSCLK/(64*UART0_BAUDRATE)<255) && (SYSCLK/(32*UART0_BAUDRATE)>255))   //波特率2倍速  
  
    TH1 = 256-SYSCLK/(64*UART0_BAUDRATE);
    PCON0 |= 1<<7;
    S0CFG &= ~(1<<6);
#elif (SYSCLK/(128*UART0_BAUDRATE)<255) && (SYSCLK/(64*UART0_BAUDRATE)>255))  //波特率4倍速
    TH1 = 256-SYSCLK/(128*UART0_BAUDRATE);
    PCON0 &= ~(1<<7);
    SOCFG |= 1<<6; 
#elif (SYSCLK/(256*UART0_BAUDRATE)<255) && (SYSCLK/(128*UART0_BAUDRATE)>255))  //波特率8倍速
    TH1 = 256-SYSCLK/(256*UART0_BAUDRATE);
    PCON0 |= (1<<7);
    S0CFG |= 1<<6; 
#else
#pragma message("定时器1无法达到所设定的波特率，请调整SYSCLK或UART0_BAUDRATE的值")
#endif    
    ET1 = 0;
    TR1 = 1;
   
#elif (UART0_BAUDRATE_MODE == 2)  //使用定时器2益处做波特率发生器

    T2CON=0X30;   //使用定时器2做为串口0的发送和接收波特率
    T2MOD=0X10;   //定时器工作在1T模式。
   
#if ((SYSCLK/(16*UART0_BAUDRATE)<65535)&&(SYSCLK/(16*UART0_BAUDRATE)>0))         //波特率不倍速   
    RCAP2L = ( 65536-SYSCLK/(16*UART0_BAUDRATE) ) & 0xff;
    RCAP2H = ( 65536-SYSCLK/(16*UART0_BAUDRATE) ) >> 8 ;
    PCON0 &= ~(1<<7);  //SMOD1=0,禁止双倍速波特率
    S0CFG &= ~(1<<6);  //SMOD2=0,禁止额外双倍速波特率     
#elif ((SYSCLK/(32*UART0_BAUDRATE)<65535) && (SYSCLK/(16*UART0_BAUDRATE)>65535))  //波特率2倍速  

    RCAP2L = ( 65536-SYSCLK/(64*UART0_BAUDRATE) ) & 0xff;
    RCAP2H = ( 65536-SYSCLK/(64*UART0_BAUDRATE) ) >> 8 ;
    PCON0 |= 1<<7;
    S0CFG &= ~(1<<6);

#elif (SYSCLK/(64*UART0_BAUDRATE)<65535) && (SYSCLK/(32*UART0_BAUDRATE)>65535))  //波特率4倍速
   
    RCAP2L = ( 65536-SYSCLK/(128*UART0_BAUDRATE) ) & 0xff;
    RCAP2H = ( 65536-SYSCLK/(128*UART0_BAUDRATE) ) >> 8 ;
    PCON0 &= ~(1<<7);
    SOCFG |= 1<<6; 


#elif (SYSCLK/(128*UART0_BAUDRATE)<65535) && (SYSCLK/(64*UART0_BAUDRATE)>65535))  //波特率8倍速
    RCAP2L = ( 65536-SYSCLK/(256*UART0_BAUDRATE) ) & 0xff;
    RCAP2H = ( 65536-SYSCLK/(256*UART0_BAUDRATE) ) >> 8 ;
    PCON0 |= (1<<7);
    S0CFG |= 1<<6;
#else
#pramga message("定时器2无法达到所设定的波特率，请调整SYSCLK或UART0_BAUDRATE的值")

#endif    
    ET2=0;     //禁止定时器2中断
	TR2 = 1;   //启动定时器2

#elif (UART0_BAUDRATE_MODE == 3)
    S0CFG|=1<<7;    //使用S1BRC来做波特率发生器,UTRS=1
    T2CON &= 0XCF;  //RCLK=0,TCLK=0   
    SFRPI = 1;
    S1CFG |= 1<<2;  //S1TX12=1,工作在1T
    SFRPI = 0;
    
#if ((SYSCLK/(32*UART0_BAUDRATE)<255)&&(SYSCLK/(32*UART0_BAUDRATE)>0))        //波特率不倍速
    SFRPI = 1;
    S1BRT = 256-SYSCLK/(32*UART0_BAUDRATE);
    SFRPI = 0;
    PCON0 &= ~(1<<7);  //SMOD1=0,禁止双倍速波特率
#elif ((SYSCLK/(64*UART0_BAUDRATE)<255) && (SYSCLK/(32*UART0_BAUDRATE)>255))   //波特率2倍速 
    SFRPI = 1;
    S1BRT = 256-SYSCLK/(64*UART0_BAUDRATE);
    SFRPI = 0;
    PCON0 |= 1<<7;  //SMOD1=1,开双倍速波特率
#else
#pragma message("独立波特率发生器无法达到此波特率，请调整SYSCLK或UART0_BAUDRATE的值");
#endif
    SFRPI = 1;
    S1CFG |= 1<<4;  //启动S1BRG定时器
    SFRPI = 0;

#else

#pragma message("请选择串口0的波特率发生器")         

#endif
    
    
    ES0 = 0;   //禁止串口中断
    
}


/***********************************************************************
   Function:  void UART0_SendByte(char chr)
           
   Description: 串口0发送一个字节
           
   Input:     要发送的字节
              
   Output:    None
  
   Return:    None
************************************************************************/
void UART0_SendByte(char chr)
{
	S0BUF = chr;
	while(!TI0);
	TI0 = 0;
}


/***********************************************************************
   Function:  void UART0_SendStr(const char *pStr)
           
   Description: 串口0发送一个字符串
           
   Input:     要发送字符串的起始地址
              
   Output:    None
  
   Return:    None
************************************************************************/
void UART0_SendStr(const char *pStr)
{
	while(*pStr)
	{
		UART0_SendByte(*pStr);		
		pStr++;
	}		
}








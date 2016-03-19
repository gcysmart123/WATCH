#include "MA82G5B32_UART.h"

/***********************************************************************
   Function:  void UART0_Init(void)
           
   Description: ����0��ʼ������ͷ�ļ�����Ե��ڲ����ʴ�С��
           
   Input:     None
              
   Output:    None
  
   Return:    None
************************************************************************/
void UART0_Init(void)
{

    S0CON = 0X50;      //����0����ģʽ1������ʹ�ܽ��ա�
    
    PCON0 &= ~(1<<7);  //SMOD1=0,��ֹ˫���ٲ�����
    S0CFG &= ~(1<<6);  //SMOD2=0,��ֹ����˫���ٲ�����
    
    S0CFG &= ~(1<<7);  // ģʽ1��ģʽ3ʱѡ��ʱ��1��ʱ��2�������ʷ�����
    
#if (UART0_BAUDRATE_MODE == 1)   //ʹ�ö�ʱ��1�洦�������ʷ�������

	TMOD &= 0x0F;	  //ʹ�ö�ʱ��1 8λ�Զ�����Ϊ�����ʷ�����
	TMOD |= 0x20;    
    AUXR2 |= 1<<3;    //���ö�ʱ��Ϊ1T����SYSCLK��
#if ((SYSCLK/(32*UART0_BAUDRATE)<255)&&(SYSCLK/(32*UART0_BAUDRATE)>0))        //�����ʲ�����
    TH1 = 256-SYSCLK/(32*UART0_BAUDRATE);                                   
    PCON0 &= ~(1<<7);  //SMOD1=0,��ֹ˫���ٲ�����
    S0CFG &= ~(1<<6);  //SMOD2=0,��ֹ����˫���ٲ�����    
#elif ((SYSCLK/(64*UART0_BAUDRATE)<255) && (SYSCLK/(32*UART0_BAUDRATE)>255))   //������2����  
  
    TH1 = 256-SYSCLK/(64*UART0_BAUDRATE);
    PCON0 |= 1<<7;
    S0CFG &= ~(1<<6);
#elif (SYSCLK/(128*UART0_BAUDRATE)<255) && (SYSCLK/(64*UART0_BAUDRATE)>255))  //������4����
    TH1 = 256-SYSCLK/(128*UART0_BAUDRATE);
    PCON0 &= ~(1<<7);
    SOCFG |= 1<<6; 
#elif (SYSCLK/(256*UART0_BAUDRATE)<255) && (SYSCLK/(128*UART0_BAUDRATE)>255))  //������8����
    TH1 = 256-SYSCLK/(256*UART0_BAUDRATE);
    PCON0 |= (1<<7);
    S0CFG |= 1<<6; 
#else
#pragma message("��ʱ��1�޷��ﵽ���趨�Ĳ����ʣ������SYSCLK��UART0_BAUDRATE��ֵ")
#endif    
    ET1 = 0;
    TR1 = 1;
   
#elif (UART0_BAUDRATE_MODE == 2)  //ʹ�ö�ʱ��2�洦�������ʷ�����

    T2CON=0X30;   //ʹ�ö�ʱ��2��Ϊ����0�ķ��ͺͽ��ղ�����
    T2MOD=0X10;   //��ʱ��������1Tģʽ��
   
#if ((SYSCLK/(16*UART0_BAUDRATE)<65535)&&(SYSCLK/(16*UART0_BAUDRATE)>0))         //�����ʲ�����   
    RCAP2L = ( 65536-SYSCLK/(16*UART0_BAUDRATE) ) & 0xff;
    RCAP2H = ( 65536-SYSCLK/(16*UART0_BAUDRATE) ) >> 8 ;
    PCON0 &= ~(1<<7);  //SMOD1=0,��ֹ˫���ٲ�����
    S0CFG &= ~(1<<6);  //SMOD2=0,��ֹ����˫���ٲ�����     
#elif ((SYSCLK/(32*UART0_BAUDRATE)<65535) && (SYSCLK/(16*UART0_BAUDRATE)>65535))  //������2����  

    RCAP2L = ( 65536-SYSCLK/(64*UART0_BAUDRATE) ) & 0xff;
    RCAP2H = ( 65536-SYSCLK/(64*UART0_BAUDRATE) ) >> 8 ;
    PCON0 |= 1<<7;
    S0CFG &= ~(1<<6);

#elif (SYSCLK/(64*UART0_BAUDRATE)<65535) && (SYSCLK/(32*UART0_BAUDRATE)>65535))  //������4����
   
    RCAP2L = ( 65536-SYSCLK/(128*UART0_BAUDRATE) ) & 0xff;
    RCAP2H = ( 65536-SYSCLK/(128*UART0_BAUDRATE) ) >> 8 ;
    PCON0 &= ~(1<<7);
    SOCFG |= 1<<6; 


#elif (SYSCLK/(128*UART0_BAUDRATE)<65535) && (SYSCLK/(64*UART0_BAUDRATE)>65535))  //������8����
    RCAP2L = ( 65536-SYSCLK/(256*UART0_BAUDRATE) ) & 0xff;
    RCAP2H = ( 65536-SYSCLK/(256*UART0_BAUDRATE) ) >> 8 ;
    PCON0 |= (1<<7);
    S0CFG |= 1<<6;
#else
#pramga message("��ʱ��2�޷��ﵽ���趨�Ĳ����ʣ������SYSCLK��UART0_BAUDRATE��ֵ")

#endif    
    ET2=0;     //��ֹ��ʱ��2�ж�
	TR2 = 1;   //������ʱ��2

#elif (UART0_BAUDRATE_MODE == 3)
    S0CFG|=1<<7;    //ʹ��S1BRC���������ʷ�����,UTRS=1
    T2CON &= 0XCF;  //RCLK=0,TCLK=0   
    SFRPI = 1;
    S1CFG |= 1<<2;  //S1TX12=1,������1T
    SFRPI = 0;
    
#if ((SYSCLK/(32*UART0_BAUDRATE)<255)&&(SYSCLK/(32*UART0_BAUDRATE)>0))        //�����ʲ�����
    SFRPI = 1;
    S1BRT = 256-SYSCLK/(32*UART0_BAUDRATE);
    SFRPI = 0;
    PCON0 &= ~(1<<7);  //SMOD1=0,��ֹ˫���ٲ�����
#elif ((SYSCLK/(64*UART0_BAUDRATE)<255) && (SYSCLK/(32*UART0_BAUDRATE)>255))   //������2���� 
    SFRPI = 1;
    S1BRT = 256-SYSCLK/(64*UART0_BAUDRATE);
    SFRPI = 0;
    PCON0 |= 1<<7;  //SMOD1=1,��˫���ٲ�����
#else
#pragma message("���������ʷ������޷��ﵽ�˲����ʣ������SYSCLK��UART0_BAUDRATE��ֵ");
#endif
    SFRPI = 1;
    S1CFG |= 1<<4;  //����S1BRG��ʱ��
    SFRPI = 0;

#else

#pragma message("��ѡ�񴮿�0�Ĳ����ʷ�����")         

#endif
    
    
    ES0 = 0;   //��ֹ�����ж�
    
}


/***********************************************************************
   Function:  void UART0_SendByte(char chr)
           
   Description: ����0����һ���ֽ�
           
   Input:     Ҫ���͵��ֽ�
              
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
           
   Description: ����0����һ���ַ���
           
   Input:     Ҫ�����ַ�������ʼ��ַ
              
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








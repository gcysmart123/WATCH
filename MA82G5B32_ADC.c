#include "MA82G5B32_ADC.H"
/***********************************************************************
   Function:  void ADC_Init(void)
           
   Description: ADC初始化
           
   Input:     None
              
   Output:    None
  
   Return:    None
************************************************************************/

void ADC_Init(void)
{
//     ADCFG0=0xb0;
//     
//     ADCFG0 &= 0XFC;  //ADTM1(B1),ADTM0(B0)为
//                      //00：时为ADCS置位就启动转换，01：定时器0溢出启动转换
//                      //10：全速模式。11：S1 BRG益处时启动转换 

    ADCFG0=0X94;
    
    P1AIO = 0x06;    //设置P11,P12为模拟输入。根据需要设定
    
//     ADCON0 |= 1<<7;  //开启ADC模块
    
    EIE1 &= ~(1<<1); //禁止ADC中断，即EADC(BIT1)清零

    ADCFG1=0X60;    //选择VREF+参考源为内部LD0，2.4V。    
}


unsigned int GetADC_Value(unsigned char channel)
{
 	uint16_t Result=0;	   
	ADCON0&=0xF8;
    ADCON0|=channel;
    ADCON0|=1<<3;             //启动ADC转换
    while(!(ADCON0&0X10)) ;   //等待AD转换结束,即等待中断标志ADCI（BIT4）=1
    ADCON0&=~(1<<4);          //清除ADC的中断标志
    Result = ADCDH;
    Result = (unsigned int)(Result<<8) + ADCDL;  
    return  Result;   
}










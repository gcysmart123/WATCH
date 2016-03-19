#include "MA82G5B32_ADC.H"
/***********************************************************************
   Function:  void ADC_Init(void)
           
   Description: ADC��ʼ��
           
   Input:     None
              
   Output:    None
  
   Return:    None
************************************************************************/

void ADC_Init(void)
{
//     ADCFG0=0xb0;
//     
//     ADCFG0 &= 0XFC;  //ADTM1(B1),ADTM0(B0)Ϊ
//                      //00��ʱΪADCS��λ������ת����01����ʱ��0�������ת��
//                      //10��ȫ��ģʽ��11��S1 BRG�洦ʱ����ת�� 

    ADCFG0=0X94;
    
    P1AIO = 0x06;    //����P11,P12Ϊģ�����롣������Ҫ�趨
    
//     ADCON0 |= 1<<7;  //����ADCģ��
    
    EIE1 &= ~(1<<1); //��ֹADC�жϣ���EADC(BIT1)����

    ADCFG1=0X60;    //ѡ��VREF+�ο�ԴΪ�ڲ�LD0��2.4V��    
}


unsigned int GetADC_Value(unsigned char channel)
{
 	uint16_t Result=0;	   
	ADCON0&=0xF8;
    ADCON0|=channel;
    ADCON0|=1<<3;             //����ADCת��
    while(!(ADCON0&0X10)) ;   //�ȴ�ADת������,���ȴ��жϱ�־ADCI��BIT4��=1
    ADCON0&=~(1<<4);          //���ADC���жϱ�־
    Result = ADCDH;
    Result = (unsigned int)(Result<<8) + ADCDL;  
    return  Result;   
}










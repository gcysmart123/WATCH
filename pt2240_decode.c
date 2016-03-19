#include <REG_MA82G5B32.H>
#include "define.h"
#include "typedef.h"

#define   RANGE    0u    //�ߵ͵�ƽ���������ֵ
int8_t bDecoding=0;

//PT2240��������ʼ��
void pt2240_init(void)
{
    P2M0&=~(1<<2);
//     PUCON0|=1<<4;  //����P22��������
    CMOD=0;
    CCAPM0=0X30;

    EIE1&=~(1<<2);   //PCA��ֹ�ж�.
    
    CCAPM0 |= 0X01;   //CCF0 = 1; ʹ�� CCON �Ĵ����еıȽ�/�����־λ CCF0 �����ж�
    CR=0;
    CH=0;
    CL=0;
    SDIN=1;
    
//     EIP1H|=1<<2;
//     EIP1L&= ~(1<<2);
    
}

static unsigned int CapTime =0;
bit B_complete=0;

//PCA�жϷ������
void PCA_ISR(void) interrupt 10  using 1
{
    if(CCF0)               //ģ��0�����䲶��
    {
        CCF0=0;            //����жϱ�־
        CapTime=CCAP0H;             //��ȡ����ֵ
        CapTime=(CapTime<<8)|CCAP0L;
        B_complete=1;        
    } 
     CF=0;
}

void PCA_StartCapture(void)   //��ʼPCA����
{
    CH=0;
    CL=0;
    CR=1;    //����PCA������
    B_complete = 0;
}


int8_t timout_ms=0;

//��ȡһ������timoutΪ��ʱʱ��
uint16_t Get_PulseWidth(int8_t timout)
{
    timout_ms=timout;         //��ʱû��׽���Զ��˳�
    PCA_StartCapture();      
    while(!B_complete)        //���񵽵͵�ƽ��ʱ��
    {
        if(timout_ms==0)      
        {
            return 0;
        }
    }
    return CapTime;
}

/***********************************************************************
  * Function��PT2440_Decode
  *         
  * Description:  2240����
  *         
  * Input:     ������յ���ֵ�ĵ�ַ
  *
  * Output:    None
  *
  * Return:    1���ɹ����롣0��δ�⵽��
  **********************************************************************/
unsigned char  PT2440_Decode(unsigned long *pCode)    //PT2440��ʼ����
{
    unsigned int HighTim,LowTim;    //�ߵ͵�ƽʱ��ֵ
    unsigned char i;
    unsigned long rcCode=0;    
    int8_t stat=0;

    if(!SDIN)
    {        
        EIE1|=1<<2;         
                        //��һ����Ļ������280us*31+(120+360)*24 
                        //���500us*31+(500+1500)*24
        LowTim=Get_PulseWidth(18);   //���񵽵͵�ƽ��ʱ��     
        if(LowTim>1200)         
        {
            bDecoding=1;   //ָʾ���ڽ���
            for(i=0;i<24;i++)
            {
                HighTim =Get_PulseWidth(5);      //�ߵ�ƽ��ʱ��
                LowTim = Get_PulseWidth(5);

                if((HighTim+LowTim)<500)    //�͵�ƽ+�ߵ�ƽС����͵�450us���˳�����
                {
                    goto END_DECODE;       
                }
                if(HighTim<LowTim)
                {
//                      rcCode &= ~(1UL<<i);           //��0���ȴ��λ  
                     rcCode &= ~(1UL<<(23-i));         //��0���ȴ��λ					
                }
                else
                {
//                      rcCode |= 1UL<<i;              //��1���ȴ��λ
					rcCode |= 1UL<<(23-i);             //��1���ȴ��λ
                }
            }
            Get_PulseWidth(10);         //�����ڶ���ͬ����,�����ߵ�ƽ 
            
            LowTim=Get_PulseWidth(20);  //����һ�����ĵ͵�ƽ
            
            if(LowTim>1800) 
            {
                *pCode=rcCode;
                stat =1;
                POWER_CS=1; 
            }  
        }        
    }
END_DECODE: 
    EIE1&=~(1<<2);   //PCA��ֹ�ж�   
    bDecoding=0;
    B_complete=0;
    CR=0;
    CF=0;      
    return stat;
}





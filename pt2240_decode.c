#include <REG_MA82G5B32.H>
#include "define.h"
#include "typedef.h"

#define   RANGE    0u    //高低电平的允许误差值
int8_t bDecoding=0;

//PT2240解码器初始化
void pt2240_init(void)
{
    P2M0&=~(1<<2);
//     PUCON0|=1<<4;  //启用P22上拉电阻
    CMOD=0;
    CCAPM0=0X30;

    EIE1&=~(1<<2);   //PCA禁止中断.
    
    CCAPM0 |= 0X01;   //CCF0 = 1; 使能 CCON 寄存器中的比较/捕获标志位 CCF0 产生中断
    CR=0;
    CH=0;
    CL=0;
    SDIN=1;
    
//     EIP1H|=1<<2;
//     EIP1L&= ~(1<<2);
    
}

static unsigned int CapTime =0;
bit B_complete=0;

//PCA中断服务程序
void PCA_ISR(void) interrupt 10  using 1
{
    if(CCF0)               //模块0脚跳变捕获
    {
        CCF0=0;            //清除中断标志
        CapTime=CCAP0H;             //获取捕获值
        CapTime=(CapTime<<8)|CCAP0L;
        B_complete=1;        
    } 
     CF=0;
}

void PCA_StartCapture(void)   //开始PCA捕获
{
    CH=0;
    CL=0;
    CR=1;    //启动PCA计数据
    B_complete = 0;
}


int8_t timout_ms=0;

//获取一个脉宽，timout为超时时间
uint16_t Get_PulseWidth(int8_t timout)
{
    timout_ms=timout;         //超时没捕捉到自动退出
    PCA_StartCapture();      
    while(!B_complete)        //捕获到低电平的时间
    {
        if(timout_ms==0)      
        {
            return 0;
        }
    }
    return CapTime;
}

/***********************************************************************
  * Function：PT2440_Decode
  *         
  * Description:  2240解码
  *         
  * Input:     保存接收到码值的地址
  *
  * Output:    None
  *
  * Return:    1，成功解码。0，未解到码
  **********************************************************************/
unsigned char  PT2440_Decode(unsigned long *pCode)    //PT2440开始解码
{
    unsigned int HighTim,LowTim;    //高低电平时间值
    unsigned char i;
    unsigned long rcCode=0;    
    int8_t stat=0;

    if(!SDIN)
    {        
        EIE1|=1<<2;         
                        //解一串码的话最短是280us*31+(120+360)*24 
                        //最长是500us*31+(500+1500)*24
        LowTim=Get_PulseWidth(18);   //捕获到低电平的时间     
        if(LowTim>1200)         
        {
            bDecoding=1;   //指示正在解码
            for(i=0;i<24;i++)
            {
                HighTim =Get_PulseWidth(5);      //高电平的时间
                LowTim = Get_PulseWidth(5);

                if((HighTim+LowTim)<500)    //低电平+高电平小于最低的450us，退出解码
                {
                    goto END_DECODE;       
                }
                if(HighTim<LowTim)
                {
//                      rcCode &= ~(1UL<<i);           //码0，先存高位  
                     rcCode &= ~(1UL<<(23-i));         //码0，先存低位					
                }
                else
                {
//                      rcCode |= 1UL<<i;              //码1，先存高位
					rcCode |= 1UL<<(23-i);             //码1，先存低位
                }
            }
            Get_PulseWidth(10);         //解析第二个同步码,丢弃高电平 
            
            LowTim=Get_PulseWidth(20);  //捕获到一个长的低电平
            
            if(LowTim>1800) 
            {
                *pCode=rcCode;
                stat =1;
                POWER_CS=1; 
            }  
        }        
    }
END_DECODE: 
    EIE1&=~(1<<2);   //PCA禁止中断   
    bDecoding=0;
    B_complete=0;
    CR=0;
    CF=0;      
    return stat;
}





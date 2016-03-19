#include "24c16.h"
#include "string.h"
#include "lcd12864.h"
#include "font.h"

int8_t I2CErr=0;

//��ʱ
void DELAY_t(unsigned int t) 
{ 
    while(t!=0) 
    t--; 
} 

/***********************************************************************
�������ƣvvoid I2C_Start(void) 
����������I2C����
������    None
���أ�    None
************************************************************************/
void I2C_Start(void)
{
	SDA=1;
    DELAY_t(DELAY_TIME); 
	SCL=1; 
	DELAY_t(DELAY_TIME); 
	SDA=0; 
	DELAY_t(DELAY_TIME); 
	SCL=0; 
	DELAY_t(DELAY_TIME); 	
}

/***********************************************************************
�������ƣvvoid I2C_Stop(void) 
����������I2Cֹͣ
������    None
���أ�    None
************************************************************************/
void I2C_Stop(void)
{
	SDA=0;
    DELAY_t(DELAY_TIME); 
	SCL=1; 
	DELAY_t(DELAY_TIME); 
	SDA=1; 
	DELAY_t(DELAY_TIME); 
	SCL=0; 
	DELAY_t(DELAY_TIME);		
}

/***********************************************************************
�������ƣvvoid I2C_Ack(void) 
����������I2CӦ��
������    None
���أ�    None
************************************************************************/
void I2C_Ack(void)
{
	SDA=0; 
	DELAY_t(DELAY_TIME); 
	SCL=1; 
	DELAY_t(DELAY_TIME);
	SCL=0;
	DELAY_t(DELAY_TIME);
	SDA=1;
}

/***********************************************************************
�������ƣvvoid I2C_NoAck(void) 
����������I2C��Ӧ��
������    None
���أ�    None
************************************************************************/
void I2C_NoAck(void)
{
	SDA=1; 
	DELAY_t(DELAY_TIME); 
	SCL=1; 
	DELAY_t(DELAY_TIME); 
	SCL=0;	
}


/***********************************************************************
�������ƣ�bit I2C_WaitAck(void)
����������I2C�ȴ���Ӧ
������    None
���أ�    None
************************************************************************/
int8_t I2C_WaitAck(void)
{
	uint16_t times=60000;
	SDA=1;
	SCL=1;
	while(SDA)
	{
		if(!times--);
		{
			I2C_Stop();
			I2CErr=TRUE;
			return FALSE;			
		}	
	}
	SCL=0;
	I2CErr=FALSE;
	return TRUE;
}

/************************************************************************
�������ƣ�void I2C_WriteByte(uint8_t dat)
����������I2Cд��һ���ֽ�
������    dat,Ҫд����ֽ�
���أ�    None
************************************************************************/
void I2C_WriteByte(uint8_t dat)
{
	uint8_t BitCnt;
	for(BitCnt=0;BitCnt<8;BitCnt++)
	{
		if( (dat<<BitCnt)&0x80 )
		{
			 SDA=1;
		}
		else
		{
			 SDA=0;
		}
		DELAY_t(DELAY_TIME); 
		SCL=1;
		DELAY_t(DELAY_TIME); 
		SCL=0;
	}
	I2C_WaitAck();
// 	DELAY(DELAY_TIME);
}

/***********************************************************************
�������ƣ�int8_t I2C_ReadByte(void)
����������I2C����һ���ֽ�
������    None
���أ�    ���ض�����һ���ֽ�
************************************************************************/
int8_t I2C_ReadByte(void)
{
	uint8_t i;
	int8_t tmp=0;
	SDA = 1;
	for(i=0;i<8;i++)
	{
		SCL = 1;
// 		DELAY_t(DELAY_TIME); 
		tmp <<= 1;
		tmp |= SDA;
		SCL = 0;
// 		DELAY_t(DELAY_TIME); 
        DELAY_t(1);
	}
	return tmp;	
}


/***********************************************************************
�������ƣ�int8_t EEP_WriteBytes(void)
����������д������������
������    None
���أ�    None
************************************************************************/
void EEP_WriteBytes(uint16_t addr,uint8_t *pbuf,uint16_t len)
{
    uint16_t PageCnt;
    uint16_t SendCnt;
 
    if( (addr%32) !=0 )
    {
        SendCnt = 32u-(addr%32u); 
        
        I2C_Start();                                 //˫��ַ
        I2C_WriteByte(EEP_DEVICEA_DDR_W);
        I2C_WriteByte((uint8_t)(addr>>8));
        I2C_WriteByte((uint8_t)(addr&0xff)); 

        while(SendCnt>0x00)
        {
            I2C_WriteByte(*pbuf);
            pbuf++;
            SendCnt--;
            len--;
            addr++;
            DELAY_t(DELAY_TIME);
            if(0==len) break;
        }
        I2C_Stop();
        DELAY_t(2000);      //ʱ����ʱ����Сֵ1000����Ҫ��С
        ClearWatchDog();  
    } 


    
    if(len>0)
    {
        PageCnt = len/32u;
        if((len%32u) !=0 )
        {
            PageCnt++;
        }
        
        for(;PageCnt>0;PageCnt--)
        {
            if(len<32u)
            {
                SendCnt=len;
            }
            else
            {
                SendCnt=32u;
            }
            I2C_Start();                                 //˫��ַ
            I2C_WriteByte(EEP_DEVICEA_DDR_W);
            I2C_WriteByte((uint8_t)(addr>>8));
            I2C_WriteByte((uint8_t)(addr&0xff)); 

            while(SendCnt>0x00)
            {
                I2C_WriteByte(*pbuf);
                pbuf++;
                SendCnt--;
                len--;
                addr++;
                DELAY_t(DELAY_TIME);
            }
            
            I2C_Stop();
            DELAY_t(2000);    //ʱ����ʱ����Сֵ1000����Ҫ��С
            ClearWatchDog();  
        }
    }
}



/***********************************************************************
�������ƣ�EEP_ReadBytes
����������I2C��������������
������    None
���أ�    None
************************************************************************/

void EEP_ReadBytes(uint16_t addr,uint8_t *pbuf,uint16_t len)
{
    I2C_Start();
    I2C_WriteByte(EEP_DEVICEA_DDR_W);  
    I2C_WriteByte((uint8_t)(addr>>8));
    I2C_WriteByte((uint8_t)(addr&0xff));  

    I2C_Start();
    I2C_WriteByte(EEP_DEVICEA_DDR_R);

    while(len--)
    {
        *pbuf = I2C_ReadByte();
        pbuf++;
        I2C_Ack();
        if(len==1) break;
    }
    *pbuf = I2C_ReadByte();
    I2C_NoAck();
    I2C_Stop();
      
}

//�洢��Ӧ���ŵļ�ֵ
//�洢��Ӧ���ŵļ�ֵ
void Store_Key_Fun(uint8_t rgn,uint8_t *Key)
{
    uint16_t addr=RF_KEY_EEPROM_ADDR;
    EEP_WriteBytes(addr+(uint16_t)(rgn-'0')*KEY_NUM,Key,KEY_NUM);    
}

//���Ҷ�Ӧ���ŵļ�ֵ
void Find_Key_Fun(uint8_t rgn,uint8_t *Key)
{
    uint16_t addr=RF_KEY_EEPROM_ADDR;
    EEP_ReadBytes(addr+(uint16_t)(rgn-'0')*KEY_NUM,Key,KEY_NUM);    
}

void Erase_all_EEPROM(void) 
{  
    uint8_t i;
    xdata int8_t buf[32];
    uint16_t addr=0;
    memset(buf,0xff,sizeof(buf));
    Fill_one_line(5,1,0xff,64);
	
	addr=0;
    for(i=0;i<127;i++)
    {
        EEP_WriteBytes(addr,buf,sizeof(buf));
        addr+=sizeof(buf);
        Fill_one_line(5,64-i/2,0,1);
    }
}


//ɾ��һ��RF��EEPROM�еĴ洢
int8_t Del_RF_EEPROM(uint8_t *p)
{
    RF_def RFtmp;
    uint16_t addr=RF_EEPROM_BASE_ADDR;
    uint16_t i;
    for(i=0;i<RF_COUNTER;i++)
    {
        EEP_ReadBytes(addr,(uint8_t *)&RFtmp,sizeof(RF_def));
        if(!(RFtmp.rf&0xff000000ul))
        {
			if(*(volatile uint32_t*)p==*(volatile uint32_t*)(RFtmp.region))
            {
                memset(&RFtmp,0xff,sizeof(RF_def));
                EEP_WriteBytes(addr,(uint8_t *)&RFtmp,sizeof(RF_def));
                EEP_RFCnt--;
                break;
            }
        }
        addr+=sizeof(RF_def);      
    }
    return 0;
}

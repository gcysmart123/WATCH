#include "MA82G5B32_PCA.H"

/***********************************************************************
  * Function��PCA_SourceClkConfig
  *         
  * Description:  ѡ��PCA��ʱ��Դ
  *         
  * Input:     PCA��ʱ��Դ���ο�PCA_ClkDef
  *
  * Output:    None
  *
  * Return:    None
  **********************************************************************/
void PCA_SourceClkConfig(PCA_ClkDef SourceClk)
{
	uint8_t tmp = CMOD;
	tmp &= 0X81;							
	CMOD = tmp | (uint8_t)(SourceClk<<1);	
}

/***********************************************************************
  * Function��PCA_ModelConfig
  *         
  * Description:  PCA����ģʽ����
  *         
  * Input:    Channel��PCA��ͨ��0~7, Mode:PCA������ģʽ���ο�PCA_Mode_def
  *
  * Output:    None
  *
  * Return:    None
  **********************************************************************/
bit PCA_ModelConfig(uint8_t Channel,PCA_Mode_def Mode )
{  
	if(Channel>7)  return 0;                
    if( (Channel>5)&&(Mode!=PCA_PWM_Output) ) return 0;    //6��7ͨ��ֻ�����ó�PWMģʽ
    switch(Channel)
	{
		 case 0:
		    CCAPM0 = (CCAPM0 & 0x40)|Mode;  
			break;
		 case 1:
			CCAPM1 = (CCAPM1 & 0x40)|Mode;  
			break;
		 case 2:
			CCAPM2 = (CCAPM2 & 0x40)|Mode;
			break;
		 case 3:
			CCAPM3 = (CCAPM3 & 0x40)|Mode;
			break;
		 case 4:
			CCAPM4 = (CCAPM4 & 0x40)|Mode;
			break;
		 case 5:
			CCAPM5 = (CCAPM5 & 0x40)|Mode;
            break;
         case 6:
            SFRPI =1;
            CCAPM6=0X02;
            SFRPI =0;
			break;            
         case 7:
            SFRPI=1;
            CCAPM7=0X02;
            SFRPI=0;            
            break;
		default:
			break;		
	}
    return 1;
}

/***********************************************************************
  * Function��Get_PCA_TimCnt
  *         
  * Description:  ��ȡPCA��ʱ���������ĵ�ǰֵ
  *         
  * Input:    None
  *
  * Output:    None
  *
  * Return:    PCA��ʱ�������ĵ�ǰֵ
  **********************************************************************/
// uint16_t Get_PCA_TimCnt(void)
// {
// 	uint16_t tmp=0;
// 	tmp=(CH<<8)+CL;
// 	return tmp;	
// }

/***********************************************************************
  * Function��PCA_Cnt_Reload
  *         
  * Description:  ����PCA����������ֵ����PCA���������ʱ����CH,CL��ֵ
  *         
  * Input:    16bit����ֵ
  *
  * Output:    None
  *
  * Return:    None
  **********************************************************************/
// void PCA_Cnt_Reload(uint16_t dat)
// {
//     CLRL = (uint8_t)dat;
//     CHRL = (uint8_t)(dat>>8);
// }

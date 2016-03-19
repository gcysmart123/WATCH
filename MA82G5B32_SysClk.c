#include "MA82G5B32_SYSCLK.h"

/***********************************************************************
  * Function��void Source_Clk_Select(sysclk_def sysclk)
  *         
  * Description:  ϵͳʱ��ѡ��
  *         
  * Input:     ϵͳʱ��Դ����,XTAL_CLK,ILRCO_CLK,ECKI_CLK,IHRCO_12M
  *            IHRCO_110592 
  *
  * Output:    None
  *
  * Return:    None
  **********************************************************************/
// void Source_Clk_Select(sysclk_def sysclk)
// {
// 	unsigned char temp=0;
// 	switch(sysclk)
// 	{
// 		case XTAL_CLK:                      //ʹ���ⲿ������Ϊϵͳʱ��
// 			temp = Page_P_SFR_Read(0X40); //����Pҳ��CKCON2(��ַ��0X40)
// 			temp |= (1<<6)|(1<<5);          //XTGS0=1,XTALE=1
// 			Page_P_SFR_Write(0X40,temp);   //д�뵽Pҳ��CKCON2(��ַ��0X40)
// 			while(CKCON1 & XTOR == 0x00);   //�ȴ�XTOR(CKCON1����λΪ1)
// 			
// 		    temp &= 0xFC;
// 			temp |= 0x01;       //OSCin Ϊ�ⲿʱ������
// 		
// 		    Page_P_SFR_Write(0X40,temp);   //д�뵽Pҳ��CKCON2(��ַ��0X40)
// 			break;
// 		
// 		case ILRCO_CLK:
// 			Page_P_SFR_Write(0X40,0x02);   //ʹ���ڲ���
// 			break;
//             
// 		case ECKI_CLK:
//             Page_P_SFR_Write(0X40,0x43);   //ʹ���ⲿ��ʱ�����룬2MHz ~ 25MHz������
// 			break;
//             
//         case IHRCO_12M:
//             CKCON0 &= 0X7F;                 //IHRCOѡ��12M            
//             Page_P_SFR_Write(0X40,0x50);  //ʹ���ڲ���12M RC������Ϊϵͳʱ��

//             break;
//             
// 		case IHRCO_110592:
//             CKCON0 |= 0x80;                 //IHRCOѡ��11.0592
//             Page_P_SFR_Write(0X40,0x50);  //ʹ���ڲ���12M RC������Ϊϵͳʱ��
//             break;	
//             
//         default:
//             break;		
// 	}	
// }


/***********************************************************************
  * Function��void OSCin_DIV_To_CKMI(OSCinDiv_def div)
  *         
  * Description:  ʱ��Դ��Ƶѡ��,�ⲿ����ʱ�������Ƶ������ֵ��MCK��Ƶ
  *         
  * Input:       div ,��Ƶ���ӣ��ο�OSCinDiv_def
  *            
  *
  * Output:    None
  *
  * Return:    None
  **********************************************************************/
void OSCin_DIV_To_CKMI(OSCinDiv_def div)
{

    unsigned char tmp;
    tmp = CKCON0&0XCF;
    tmp|=div<<4;
    Page_P_SFR_Write(0xC7,tmp); //д�뵽Pҳ��CKCON0(��ַ��0XC7)
//     CKCON0 = tmp;   // ��ͨҳд
}

/***********************************************************************
  * Function��void CKMI_Mul_To_MCK(MckDiv_def mul)
  *         
  * Description:  CKMI��Ƶ��õ�MCK������Դ�����ñ�Ƶ���ӡ�
  *         
  * Input:       
  *            
  *
  * Output:    None
  *
  * Return:    None
  **********************************************************************/
void CKMI_Mul_To_MCK(CkmiMul_def mul)
{
    unsigned char tmp;
    if(mul) 
    {
        tmp = CKCON0;
        tmp|=(1<<6);                  //ENCKM =1,ʹ��ʱ�ӱ�Ƶ��   
        Page_P_SFR_Write(0xC7,tmp); //д�뵽Pҳ��CKCON0(��ַ��0XC7)
//        CKCON0 = tmp;               //��ͨҳ��CKCOON0����
    }
    tmp = Page_P_SFR_Read(0X40);    //����Pҳ��CKCON2(��ַ��0X40)
    tmp &= 0XF3;
    tmp|= mul<<2;
    Page_P_SFR_Write(0X40 ,tmp);    //д��Pҳ��CKCON2
}

/***********************************************************************
  * Function��void MCK_Div_To_SysClk(MCK_Div_def mul)
  *         
  * Description:  MCKʱ�ӷ�Ƶ����Ϊϵͳʱ��SYSCLK.
  *         
  * Input:      ��Ƶ���ӣ�1��1024��Ƶ���ֱ�ΪMCK_Div1��MCK_Div2��MCK_Div4������MCK_Div1024 
  *            
  *
  * Output:    None
  *
  * Return:    None
  **********************************************************************/
void MCK_Div_To_SysClk(MCK_Div_def mul)
{
    unsigned char tmp;
    tmp=Page_P_SFR_Read(0XC7);  //����Pҳ��CKCON0(��ַ:0XC7)
    tmp&=0xF8;  
    tmp|=(mul&0x07);    
    Page_P_SFR_Write(0XC7,tmp); //дPҳ��CKCON0
    
    tmp=Page_P_SFR_Read(0X41);  //����Pҳ��CKCON3(��ַ��0X41)
    tmp&=0x23;
    tmp|=(mul>>1)&0xFE;
    Page_P_SFR_Write(0X41,tmp); //д�뵽Pҳ��CKCON3��
}  
    
/***********************************************************************
  * Function��void SysClk_Div_To_CPUClk(CPUCLK_Def cpuclk) 
  *         
  * Description:   CPUʱ��Դ��Ƶѡ�񣬿ɲ���Ƶ�����Ƶ
  *         
  * Input:    cpuclk = SysClk_Div1����Ƶ��= SysClk_Div2Ϊϵͳʱ�Ӷ���Ƶ��ΪCPUʱ��   
  *            
  *
  * Output:    None
  *
  * Return:    None
  **********************************************************************/  
void SysClk_Div_To_CPUClk(CPUCLK_Def cpuclk)
{
    unsigned char tmp;
    tmp=CKCON0;
    tmp&=~(1<<3);
    tmp|=cpuclk<<3;
    Page_P_SFR_Write(0XC7,tmp); //д�뵽Pҳ��CKCON0(��ַ:0XC7)
//     CKCON0 = tmp;            //д��ͨҳ��CKCON0
} 
  
/***********************************************************************
  * Function��void Set_MCK_Outut(MCK_Output_def mod)
  *         
  * Description: ����P60��XTAL2�����MCK��ģʽ
  *         
  * Input:    MCK_Output_None��P60Ϊ��ͨIO��
  *           MCK_Output_Div1��MCK��P60���
  *           MCK_Output_Div2��MCK����Ƶ��P60��� 
  *           MCK_Output_Div4��MCK�ķ�Ƶ��P60���
  *
  * Output:    None
  *
  * Return:    None
  **********************************************************************/  
// void Set_MCK_Outut(MCK_Output_def mod)
// {
//     AUXR0&=0X3F;
//     AUXR0|=mod<<6;
// }  
  
  
  
/***********************************************************************
  * Function��void SysCLK_Init(void) 
  *         
  * Description:   ϵͳʱ�ӳ�ʼ��
  *         
  * Input:    None   
  *            
  *
  * Output:    None
  *
  * Return:    None
  **********************************************************************/   
void SysCLK_Init(void)
{
//     Source_Clk_Select(IHRCO_12M);   //ϵͳʱ��ѡ��Ϊ�ڲ�12M��RC������

    CKCON0 &= 0X7F;                 //IHRCOѡ��12M            
    Page_P_SFR_Write(0X40,0x50);  //ʹ���ڲ���12M RC������Ϊϵͳʱ��

    OSCin_DIV_To_CKMI(OSCin_Div2);     //ʱ��Դ�����Ƶ��5-6.5M  --��12 /2  = 6M
    CKMI_Mul_To_MCK(CKMI_Mul4);        //4��Ƶ                  --��6 * 4 = 24M
    MCK_Div_To_SysClk(MCK_Div4);       //MCK4��Ƶ               ---6M
    SysClk_Div_To_CPUClk(SysClk_Div1); //CPU��ʱ��              ---3M    
}  





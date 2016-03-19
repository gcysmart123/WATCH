#include "MA82G5B32_SYSCLK.h"

/***********************************************************************
  * Function：void Source_Clk_Select(sysclk_def sysclk)
  *         
  * Description:  系统时钟选择
  *         
  * Input:     系统时钟源类型,XTAL_CLK,ILRCO_CLK,ECKI_CLK,IHRCO_12M
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
// 		case XTAL_CLK:                      //使用外部晶振做为系统时钟
// 			temp = Page_P_SFR_Read(0X40); //读出P页的CKCON2(地址：0X40)
// 			temp |= (1<<6)|(1<<5);          //XTGS0=1,XTALE=1
// 			Page_P_SFR_Write(0X40,temp);   //写入到P页的CKCON2(地址：0X40)
// 			while(CKCON1 & XTOR == 0x00);   //等待XTOR(CKCON1第七位为1)
// 			
// 		    temp &= 0xFC;
// 			temp |= 0x01;       //OSCin 为外部时钟输入
// 		
// 		    Page_P_SFR_Write(0X40,temp);   //写入到P页的CKCON2(地址：0X40)
// 			break;
// 		
// 		case ILRCO_CLK:
// 			Page_P_SFR_Write(0X40,0x02);   //使用内部的
// 			break;
//             
// 		case ECKI_CLK:
//             Page_P_SFR_Write(0X40,0x43);   //使用外部的时钟输入，2MHz ~ 25MHz的增益
// 			break;
//             
//         case IHRCO_12M:
//             CKCON0 &= 0X7F;                 //IHRCO选择12M            
//             Page_P_SFR_Write(0X40,0x50);  //使用内部的12M RC振荡器做为系统时钟

//             break;
//             
// 		case IHRCO_110592:
//             CKCON0 |= 0x80;                 //IHRCO选择11.0592
//             Page_P_SFR_Write(0X40,0x50);  //使用内部的12M RC振荡器做为系统时钟
//             break;	
//             
//         default:
//             break;		
// 	}	
// }


/***********************************************************************
  * Function：void OSCin_DIV_To_CKMI(OSCinDiv_def div)
  *         
  * Description:  时钟源分频选择,外部晶振时钟输入分频到合理值给MCK倍频
  *         
  * Input:       div ,分频因子，参考OSCinDiv_def
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
    Page_P_SFR_Write(0xC7,tmp); //写入到P页的CKCON0(地址：0XC7)
//     CKCON0 = tmp;   // 普通页写
}

/***********************************************************************
  * Function：void CKMI_Mul_To_MCK(MckDiv_def mul)
  *         
  * Description:  CKMI倍频后得到MCK的输入源，设置倍频因子。
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
        tmp|=(1<<6);                  //ENCKM =1,使能时钟倍频器   
        Page_P_SFR_Write(0xC7,tmp); //写入到P页的CKCON0(地址：0XC7)
//        CKCON0 = tmp;               //普通页的CKCOON0操作
    }
    tmp = Page_P_SFR_Read(0X40);    //读出P页的CKCON2(地址：0X40)
    tmp &= 0XF3;
    tmp|= mul<<2;
    Page_P_SFR_Write(0X40 ,tmp);    //写入P页的CKCON2
}

/***********************************************************************
  * Function：void MCK_Div_To_SysClk(MCK_Div_def mul)
  *         
  * Description:  MCK时钟分频后做为系统时钟SYSCLK.
  *         
  * Input:      分频因子，1到1024分频，分别为MCK_Div1，MCK_Div2，MCK_Div4……，MCK_Div1024 
  *            
  *
  * Output:    None
  *
  * Return:    None
  **********************************************************************/
void MCK_Div_To_SysClk(MCK_Div_def mul)
{
    unsigned char tmp;
    tmp=Page_P_SFR_Read(0XC7);  //读出P页的CKCON0(地址:0XC7)
    tmp&=0xF8;  
    tmp|=(mul&0x07);    
    Page_P_SFR_Write(0XC7,tmp); //写P页的CKCON0
    
    tmp=Page_P_SFR_Read(0X41);  //读出P页的CKCON3(地址：0X41)
    tmp&=0x23;
    tmp|=(mul>>1)&0xFE;
    Page_P_SFR_Write(0X41,tmp); //写入到P页的CKCON3的
}  
    
/***********************************************************************
  * Function：void SysClk_Div_To_CPUClk(CPUCLK_Def cpuclk) 
  *         
  * Description:   CPU时钟源分频选择，可不分频或二分频
  *         
  * Input:    cpuclk = SysClk_Div1不分频，= SysClk_Div2为系统时钟二分频做为CPU时钟   
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
    Page_P_SFR_Write(0XC7,tmp); //写入到P页的CKCON0(地址:0XC7)
//     CKCON0 = tmp;            //写普通页的CKCON0
} 
  
/***********************************************************************
  * Function：void Set_MCK_Outut(MCK_Output_def mod)
  *         
  * Description: 设置P60（XTAL2）输出MCK的模式
  *         
  * Input:    MCK_Output_None，P60为普通IO口
  *           MCK_Output_Div1，MCK从P60输出
  *           MCK_Output_Div2，MCK二分频从P60输出 
  *           MCK_Output_Div4，MCK四分频从P60输出
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
  * Function：void SysCLK_Init(void) 
  *         
  * Description:   系统时钟初始化
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
//     Source_Clk_Select(IHRCO_12M);   //系统时钟选择为内部12M的RC振荡器。

    CKCON0 &= 0X7F;                 //IHRCO选择12M            
    Page_P_SFR_Write(0X40,0x50);  //使用内部的12M RC振荡器做为系统时钟

    OSCin_DIV_To_CKMI(OSCin_Div2);     //时钟源输入分频到5-6.5M  --即12 /2  = 6M
    CKMI_Mul_To_MCK(CKMI_Mul4);        //4倍频                  --即6 * 4 = 24M
    MCK_Div_To_SysClk(MCK_Div4);       //MCK4分频               ---6M
    SysClk_Div_To_CPUClk(SysClk_Div1); //CPU的时钟              ---3M    
}  





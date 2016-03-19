#include "MA82G5B32_IAP.H"

#include <REG_MA82G5B32.H>

#define IAPLB    0X03



/***********************************************************************
  * Function��Page_P_SFR_Write
  *         
  * Description:  дPҳ�Ĵ���
  *         
  * Input:     SFR,PҳSFR�ĵ�ַ��  dat��Ҫд�������
  *
  * Output:    None
  *
  * Return:    None
  **********************************************************************/

void Page_P_SFR_Write(uint8_t SFR,uint8_t dat)
{
//     bit EA_tmp = EA;
//     EA = 0;
    IFADRH=0X00;
    ISPCR = ISPEN;
    IFMT = Ppage_Wirte_Cmd;
    IFADRL = SFR;
    IFD = dat;
    SCMD = 0X46;
    SCMD = 0XB9;
    IFMT = Standby_Cmd;
    ISPCR &= ~ISPEN;
    IFADRH = 0XFF;
//     EA = EA_tmp;
}

/***********************************************************************
  * Function��Page_P_SFR_Read
  *         
  * Description:  ��Pҳ�Ĵ���
  *         
  * Input:     SFR,PҳSFR�ĵ�ַ
  *
  * Output:    None
  *
  * Return:    ������PҳSFR�Ĵ�����ֵ
  **********************************************************************/

uint8_t Page_P_SFR_Read(uint8_t SFR)
{
//     bit EA_tmp = EA;
//     EA = 0;
    IFADRH=0X00;
    ISPCR = ISPEN;
    IFMT = Ppage_Read_Cmd;
    IFADRL = SFR;
    SCMD = 0X46;
    SCMD = 0XB9;
    IFMT = Standby_Cmd;
    ISPCR &= ~ISPEN; 
    IFADRH = 0XFF;
//     EA = EA_tmp;
    return IFD;
}

#if 0

/***********************************************************************
  * Function��IAP_FlashErease
  *         
  * Description:  IAP����
  *         
  * Input:     addr��Ҫ����Flash����ʼ��ַ��len��Ҫ�����ĳ���
  *
  * Output:    None
  *
  * Return:    None
  **********************************************************************/   
void IAP_FlashErease(uint16_t addr,uint16_t len)
{
	uint16_t BaseAddr=addr;
	do{
		IFADRH = addr>>8;
		IFADRL = addr&0xFF;
		IFMT=IAP_PageErase_Cmd;
		ISPCR |= ISPEN;
		SCMD=0X46;
		SCMD=0XB9;
		addr += 512;
	} while((addr-BaseAddr)<len) ;
	ISPCR = 0;
	IFMT =Standby_Cmd;
}

/***********************************************************************
  * Function��IAP_ReadByte
  *         
  * Description:  IAP��ȡһ���ֽ�
  *         
  * Input:     Ҫ��ȡ��Flash�ĵ�ַ
  *
  * Output:    None
  *
  * Return:    �������ֽ�
  **********************************************************************/
uint8_t IAP_ReadByte(uint16_t addr)
{
    uint8_t Dat;
	bit EA_tmp = EA;
    EA = 0;   
	IFADRH = addr>>8;
	IFADRL = addr&0xFF;
	IFMT = IAP_Read_Cmd;
	ISPCR |= ISPEN;
	SCMD = 0X46;
	SCMD = 0XB9;
	Dat = IFD;
	ISPCR = 0;
	IFMT = Standby_Cmd;
    EA = EA_tmp;
	return Dat;
}

/***********************************************************************
  * Function��IAP_ReadBytes
  *         
  * Description:  IAP��ȡһ�ֳ��ȵ����ݵ��ڴ�
  *         
  * Input:     addr,Ҫ��ȡ��Flash����ʼ��ַ;buf,�ڴ滺������lenҪ��ȡ�ĳ���
  *
  * Output:    None
  *
  * Return:    �ɹ�����1
  **********************************************************************/
bit IAP_ReadBytes(uint16_t addr,void *buf,uint16_t len)
{
	uint8_t *pbuf = buf;
	while(len--)
	{
		*pbuf = IAP_ReadByte(addr);
		pbuf++;
		addr++;
	}
	return 1;
}


/***********************************************************************
  * Function��IAP_WirteByte
  *         
  * Description:  IAPд��һ���ֽ�
  *         
  * Input:     addr,Ҫд��Flash�ĵ�ַ;dat��д�������
  *
  * Output:    None
  *
  * Return:    �ɹ�����1
  **********************************************************************/
bit IAP_WirteByte(uint16_t addr,uint8_t dat)
{
    bit EA_tmp = EA;
    EA = 0;
	IFADRH = addr>>8;
	IFADRL = addr&0xFF;
	IFD = dat;
	IFMT = IAP_Write_Cmd;
	ISPCR |= ISPEN;
	SCMD=0X46;
	SCMD=0XB9;
	ISPCR = 0;
	IFMT = Standby_Cmd;
    EA = EA_tmp;
	return 1;
}


/***********************************************************************
  * Function��IAP_WirteBytes
  *         
  * Description:  IAPд��һ�ֳ��ȵ����ݵ��ڴ�
  *         
  * Input:     addr,Ҫд���Flash����ʼ��ַ;buf,�ڴ滺������lenд��ĳ���
  *
  * Output:    None
  *
  * Return:    �ɹ�����1
  **********************************************************************/
bit IAP_WirteBytes(uint16_t addr,void *pdat,uint16_t len)
{
	uint8_t *pbuf = pdat;
	while(len--)
	{
		IAP_WirteByte(addr,*pbuf);
		pbuf++;
		addr++;
	}
	return 1;
}


/***********************************************************************
  * Function��SetIAP_StartAddr
  *         
  * Description:  ����IAP����ʼ��ַ
  *         
  * Input:     addr��IAP��ʼ��ַ
  *
  * Output:    None
  *
  * Return:    None
  **********************************************************************/

 void SetIAP_StartAddr(uint16_t addr)
 {
     uint8_t tmp=addr>>8;      //IAP��ʼ��ַ = IAPLB * 256
     addr &= 0xFE;             //IAPLB�����λ����Ϊ0
     Page_P_SFR_Write(IAPLB,addr>>8);
 }

 #endif

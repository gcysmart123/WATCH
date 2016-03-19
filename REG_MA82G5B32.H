#ifndef __REG_MA82G5B32_H
#define __REG_MA82G5B32_H

#define GF_7         0x80
#define GF_6         0x40
#define GF_5         0x20
#define GF_4         0x10
#define GF_3         0x08
#define GF_2         0x04
#define GF_1         0x02
#define GF_0         0x01

sfr SP             = 0x81;

sfr DPL            = 0x82;

sfr DPH            = 0x83;

sfr SPSTAT         = 0x84;
// (bits in SPSTAT)
#define SPIF         0x80
#define WCOL         0x40
#define THRF         0x20
#define SPIBSY       0x10
#define MODF         0x08
#define SPR2         0x01

sfr SPCON          = 0x85;
// (bits in SPCON)
#define SSIG         0x80
#define SPEN         0x40
#define DORD         0x20
#define MSTR         0x10
#define CPOL         0x08
#define CPHA         0x04
#define SPR1         0x02
#define SPR0         0x01

sfr SPDAT          = 0x86;

sfr PCON0          = 0x87;
// (bits in PCON0)
#define SMOD1        0x80
#define SMOD0        0x40
#define POF0         0x10
#define GF1          0x08
#define GF0          0x04
#define PD           0x02
#define IDL          0x01

sfr TCON           = 0x88;
// (bits in TCON)
sbit TF1           = TCON^7;
sbit TR1           = TCON^6;
sbit TF0           = TCON^5;
sbit TR0           = TCON^4;
sbit IE1           = TCON^3;
sbit IT1           = TCON^2;
sbit IE0           = TCON^1;
sbit IT0           = TCON^0;

sfr TMOD           = 0x89;
// (bits in TMOD)
#define T1GATE       0x80
#define T1CT         0x40
#define T1M1         0x20
#define T1M0         0x10
#define T0GATE       0x08
#define T0CT         0x04
#define T0M1         0x02
#define T0M0         0x01

sfr TL0            = 0x8A;

sfr TL1            = 0x8B;

sfr TH0            = 0x8C;

sfr TH1            = 0x8D;

sfr SFIE           = 0x8E;
// (bits in SFIE)
#define SIDFIE       0x80
#define MCDRE        0x40
#define MCDFIE       0x20
#define RTCFIE       0x10
#define BOF1IE       0x04
#define BOF0IE       0x02
#define WDTFIE       0x01

sfr P1             = 0x90;
// (bits in P1)
sbit P17           = P1^7;
sbit P16           = P1^6;
sbit P15           = P1^5;
sbit P14           = P1^4;
sbit P13           = P1^3;
sbit P12           = P1^2;
sbit P11           = P1^1;
sbit P10           = P1^0;

sfr P1M0           = 0x91;

sfr P1AIO          = 0x92;
// (bits in P1AIO)
#define P17AIO       0x80
#define P16AIO       0x40
#define P15AIO       0x20
#define P14AIO       0x10
#define P13AIO       0x08
#define P12AIO       0x04
#define P11AIO       0x02
#define P10AIO       0x01

sfr P2M0           = 0x95;
// (bits in P2M0)
#define P2M07        0x80
#define P2M06        0x40
#define P2M05        0x20
#define P2M04        0x10
#define P2M03        0x08
#define P2M02        0x04
#define P2M01        0x02
#define P2M00        0x01

sfr BOREV          = 0x96;
// (bits in BOREV)
#define BOREV7       0x80
#define BOREV6       0x40
#define BOREV5       0x20
#define BOREV4       0x10
#define BOREV3       0x08
#define BOREV2       0x04
#define BOREV1       0x02
#define BOREV0       0x01

sfr PCON1          = 0x97;
// (bits in PCON1)
#define SWRF         0x80
#define EXRF         0x40
#define MCDF         0x20
#define RTCF         0x10
#define BOF1         0x04
#define BOF0         0x02
#define WDTF         0x01

sfr S0CON          = 0x98;
// (bits in S0CON)
sbit SM00_FE       = S0CON^7;
sbit SM10          = S0CON^6;
sbit SM20          = S0CON^5;
sbit REN0          = S0CON^4;
sbit TB80          = S0CON^3;
sbit RB80          = S0CON^2;
sbit TI0           = S0CON^1;
sbit RI0           = S0CON^0;

sfr S1CON          = 0x98;
sfr S2CON          = 0x98;
// (bits in S1CON)
sbit SM01          = S1CON^7;
sbit SM11          = S1CON^6;
sbit SM21          = S1CON^5;
sbit REN1          = S1CON^4;
sbit TB81          = S1CON^3;
sbit RB81          = S1CON^2;
sbit TI1           = S1CON^1;
sbit RI1           = S1CON^0;

sfr S0BUF          = 0x99;
sfr S1BUF          = 0x99;
sfr S2BUF          = 0x99;

sfr S1BRT          = 0x9A;
sfr S2BRT          = 0x9A;
// (bits in S1BRT)
#define S1BRT7       0x80
#define S1BRT6       0x40
#define S1BRT5       0x20
#define S1BRT4       0x10
#define S1BRT3       0x08
#define S1BRT2       0x04
#define S1BRT1       0x02
#define S1BRT0       0x01

sfr S1BRC          = 0x9B;
sfr S2BRC          = 0x9B;
// (bits in S1BRC)
#define S1BRC7       0x80
#define S1BRC6       0x40
#define S1BRC5       0x20
#define S1BRC4       0x10
#define S1BRC3       0x08
#define S1BRC2       0x04
#define S1BRC1       0x02
#define S1BRC0       0x01

sfr S0CFG          = 0x9C;
// (bits in S0CFG)
#define URTS         0x80
#define S0MOD2       0x40
#define URM0X3       0x20
#define SM30         0x10
#define S0DOR        0x08
#define BTI          0x04
#define UTIE         0x02

sfr S1CFG          = 0x9C;
sfr S2CFG          = 0x9C;
// (bits in S1CFG)
#define SM31         0x80
#define EVPS         0x40
#define S1DOR        0x20
#define S1TR         0x10
#define S1MOD1       0x08
#define S1TX12       0x04
#define S1CKOE       0x02
#define S1TME        0x01

sfr S1CFG1         = 0x9D;
// (bits in S1CFG1)
#define SBF          0x80
#define TXER         0x40
#define SB16         0x20
#define ATBR         0x10
#define TXRX         0x08
#define SYNC         0x04

sfr S2CFG1         = 0x9D;
// (bits in S2CFG1)
#define ESDE         0x80
#define ESDF         0x40
#define PEF          0x20
#define ESOE         0x10
#define S1BRS3       0x08
#define S1BRS2       0x04
#define S1BRS1       0x02
#define S1BRS0       0x01

sfr P2             = 0xA0;
// (bits in P2)
sbit P27           = P2^7;
sbit P26           = P2^6;
sbit P25           = P2^5;
sbit P24           = P2^4;
sbit P23           = P2^3;
sbit P22           = P2^2;
sbit P21           = P2^1;
sbit P20           = P2^0;

sfr AUXR0          = 0xA1;
// (bits in AUXR0)
#define P60OC1       0x80
#define P60OC0       0x40
#define P60FD        0x20
#define T0XL         0x10
#define P4FS1        0x08
#define P4FS0        0x04
#define INT1H        0x02
#define INT0H        0x01

sfr AUXR1          = 0xA2;
// (bits in AUXR1)
#define P1KBIH       0x80
#define P3KBIL       0x40
#define P4SPI        0x20
#define P3S1         0x10
#define P3S1MI       0x08
#define P6TWI        0x04
#define P3CEX        0x02
#define DPS          0x01

sfr AUXR2          = 0xA3;
// (bits in AUXR2)
#define INT3IS1      0x80
#define INT3IS0      0x40
#define INT2IS1      0x20
#define INT2IS0      0x10
#define T1X12        0x08
#define T0X12        0x04
#define T1CKOE       0x02
#define T0CKOE       0x01

sfr AUXR3          = 0xA4;
// (bits in AUXR3)
#define STAF         0x80
#define STOF         0x40
#define BPOC1        0x20
#define BPOC0        0x10
//#define GF_3         0x08
#define P1S0MI       0x04
#define P3ECI        0x02
#define P3TWI1       0x01

sfr IE             = 0xA8;
// (bits in IE)
sbit EA            = IE^7;
sbit GF4           = IE^6;
sbit ET2           = IE^5;
sbit ES0           = IE^4;
sbit ET1           = IE^3;
sbit EX1           = IE^2;
sbit ET0           = IE^1;
sbit EX0           = IE^0;

sfr SADDR          = 0xA9;

sfr SFRPI          = 0xAC;
// (bits in SFRPI)
#define IDX3         0x08
#define IDX2         0x04
#define IDX1         0x02
#define IDX0         0x01

sfr EIE1           = 0xAD;
// (bits in EIE1)
#define ETWI1        0x80
#define ETWSI        0x40
#define EKB          0x20
#define ES1          0x10
#define ESF          0x08
#define EPCA         0x04
#define EADC         0x02
#define ESPI         0x01

sfr EIP1L          = 0xAE;
// (bits in EIP1L)
#define PTWI1L       0x80
#define PTWIL        0x40
#define PKBL         0x20
#define PS1L         0x10
#define PSFL         0x08
#define PPCAL        0x04
#define PADCL        0x02
#define PSPIL        0x01

sfr EIP1H          = 0xAF;
// (bits in EIP1H)
#define PTWI1H       0x80
#define PTWIH        0x40
#define PKBH         0x20
#define PS1H         0x10
#define PSFH         0x08
#define PPCAH        0x04
#define PADCH        0x02
#define PSPIH        0x01

sfr P3             = 0xB0;
// (bits in P3)
sbit P37           = P3^7;
sbit P36           = P3^6;
sbit P35           = P3^5;
sbit P34           = P3^4;
sbit P33           = P3^3;
sbit P32           = P3^2;
sbit P31           = P3^1;
sbit P30           = P3^0;

sfr P3M0           = 0xB1;

sfr P3M1           = 0xB2;

sfr P4M0           = 0xB3;

sfr PUCON0         = 0xB4;
// (bits in PUCON0)
#define P4PU1        0x80
#define P4PU0        0x40
#define P2PU1        0x20
#define P2PU0        0x10
#define P1PU1        0x08
#define P1PU0        0x04
#define P0PU1        0x02
#define P0PU0        0x01

sfr PUCON1         = 0xB4;
// (bits in PUCON1)
#define P6PU1        0x08
#define P6PU0        0x04
#define P5PU1        0x02
#define P5PU0        0x01

sfr P5M0           = 0xB5;

sfr P6M0           = 0xB5;

sfr RTCTM          = 0xB6;
// (bits in RTCTM)
#define RTCCS1       0x80
#define RTCCS0       0x40
#define RTCCT5       0x20
#define RTCCT4       0x10
#define RTCCT3       0x08
#define RTCCT2       0x04
#define RTCCT1       0x02
#define RTCCT0       0x01

sfr IP0H           = 0xB7;
// (bits in IP0H)
#define PX3H         0x80
#define PX2H         0x40
#define PT2H         0x20
#define PSH          0x10
#define PT1H         0x08
#define PX1H         0x04
#define PT0H         0x02
#define PX0H         0x01

sfr IP0L           = 0xB8;
// (bits in IP0L)
sbit PX3L          = IP0L^7;
sbit PX2L          = IP0L^6;
sbit PT2L          = IP0L^5;
sbit PSL           = IP0L^4;
sbit PT1L          = IP0L^3;
sbit PX1L          = IP0L^2;
sbit PT0L          = IP0L^1;
sbit PX0L          = IP0L^0;

sfr SADEN          = 0xB9;

sfr ADCFG1         = 0xBB;
// (bits in ADCFG1)
#define GF           0x80
#define VRS2         0x40
#define VRS1         0x20
#define SIGN         0x10
#define AOS3         0x08
#define AOS2         0x04
#define AOS1         0x02
#define AOS0         0x01

sfr PWMCR          = 0xBC;
// (bits in PWMCR)
#define PCAE         0x80
#define EXDT         0x40
#define PFCF         0x10
#define PFCM         0x08
#define PFCS2        0x04
#define PFCS1        0x02
#define PFCS0        0x01

sfr PDTCR          = 0xBD;
// (bits in PDTCR)
#define DTPS1        0x80
#define DTPS0        0x40
#define DT5          0x20
#define DT4          0x10
#define DT3          0x08
#define DT2          0x04
#define DT1          0x02
#define DT0          0x01

sfr RTCCR          = 0xBE;
// (bits in RTCCR)
#define RTCE         0x80
#define RTCO         0x40
#define RTCRL5       0x20
#define RTCRL4       0x10
#define RTCRL3       0x08
#define RTCRL2       0x04
#define RTCRL1       0x02
#define RTCRL0       0x01

sfr CKCON1         = 0xBF;
// (bits in CKCON1)
#define XTOR         0x80
#define XCKS5        0x20
#define XCKS4        0x10
#define XCKS3        0x08
#define XCKS2        0x04
#define XCKS1        0x02
#define XCKS0        0x01

sfr XICON          = 0xC0;
// (bits in XICON)
sbit INT3H         = XICON^7;
sbit EX3           = XICON^6;
sbit IE3           = XICON^5;
sbit IT3           = XICON^4;
sbit INT2H         = XICON^3;
sbit EX2           = XICON^2;
sbit IE2           = XICON^1;
sbit IT2           = XICON^0;

sfr XICFG          = 0xC1;
// (bits in XICFG)
#define INT1IS1      0x80
#define INT1IS0      0x40
#define INT0IS1      0x20
#define INT0IS0      0x10
#define X3FLT        0x08
#define X2FLT        0x04
#define X1FLT        0x02
#define X0FLT        0x01

sfr ADCFG0         = 0xC3;
// (bits in ADCFG0)
#define ADCKS2       0x80
#define ADCKS1       0x40
#define ADCKS0       0x20
#define ADRJ         0x10
#define ADPS         0x08
#define VRS0         0x04
#define ADTM1        0x02
#define ADTM0        0x01

sfr ADCON0         = 0xC4;
// (bits in ADCON0)
#define ADCEN        0x80
#define CH3          0x20
#define ADCI         0x10
#define ADCS         0x08
#define CHS2         0x04
#define CHS1         0x02
#define CHS0         0x01

sfr ADCDL          = 0xC5;
// (bits in ADCDL)
#define ADCV1        0x80
#define ADCV0        0x40

sfr ADCDH          = 0xC6;
// (bits in ADCDH)
#define ADCV9        0x80
#define ADCV8        0x40
#define ADCV7        0x20
#define ADCV6        0x10
#define ADCV5        0x08
#define ADCV4        0x04
#define ADCV3        0x02
#define ADCV2        0x01

sfr CKCON0         = 0xC7;
// (bits in CKCON0)
#define AFS          0x80
#define ENCKM        0x40
#define CKMIS1       0x20
#define CKMIS0       0x10
#define CCKS         0x08
#define SCKS2        0x04
#define SCKS1        0x02
#define SCKS0        0x01

sfr T2CON          = 0xC8;
// (bits in T2CON)
sbit TF2           = T2CON^7;
sbit EXF2          = T2CON^6;
sbit RCLK          = T2CON^5;
sbit TCLK          = T2CON^4;
sbit EXEN2         = T2CON^3;
sbit TR2           = T2CON^2;
sbit C_T2          = T2CON^1;
sbit CP_RL2        = T2CON^0;

sfr T2MOD          = 0xC9;
// (bits in T2MOD)
#define T2EXH        0x20
#define T2X12        0x10
#define T2OE         0x02
#define DCEN2        0x01

sfr RCAP2L         = 0xCA;

sfr RCAP2H         = 0xCB;

sfr TL2            = 0xCC;

sfr TH2            = 0xCD;

sfr CLRL           = 0xCE;

sfr CHRL           = 0xCF;

sfr PSW            = 0xD0;
// (bits in PSW)
sbit CY            = PSW^7;
sbit AC            = PSW^6;
sbit F0            = PSW^5;
sbit RS1           = PSW^4;
sbit RS0           = PSW^3;
sbit OV            = PSW^2;
sbit F1            = PSW^1;
sbit P             = PSW^0;

sfr SIADR          = 0xD1;
// (bits in SIADR)
#define GC           0x01

sfr SI1ADR         = 0xD1;
// (bits in SI1ADR)
#define GC1          0x01

sfr SIDAT          = 0xD2;

sfr SI1DAT         = 0xD2;

sfr SISTA          = 0xD3;

sfr SI1STA         = 0xD3;

sfr SICON          = 0xD4;
// (bits in SICON)
#define CR2          0x80
#define ENSI         0x40
#define STA          0x20
#define STO          0x10
#define SI           0x08
#define AA           0x04
#define CR1          0x02
#define CR0          0x01

sfr SI1CON         = 0xD4;
// (bits in SI1CON)
#define CR21         0x80
#define ENSI1        0x40
#define STA1         0x20
#define STO1         0x10
#define SI1          0x08
#define AA1          0x04
#define CR11         0x02
#define CR01         0x01

sfr KBPATN         = 0xD5;

sfr KBCON          = 0xD6;
// (bits in KBCON)
#define PATNS        0x02
#define KBIF         0x01

sfr KBMASK         = 0xD7;

sfr CCON           = 0xD8;
// (bits in CCON)
sbit CF            = CCON^7;
sbit CR            = CCON^6;
sbit CCF5          = CCON^5;
sbit CCF4          = CCON^4;
sbit CCF3          = CCON^3;
sbit CCF2          = CCON^2;
sbit CCF1          = CCON^1;
sbit CCF0          = CCON^0;

sfr CMOD           = 0xD9;
// (bits in CMOD)
#define CIDL         0x80
#define BME4         0x40
#define BME2         0x20
#define BME0         0x10
#define CPS2         0x08
#define CPS1         0x04
#define CPS0         0x02
#define ECF          0x01

sfr CCAPM0         = 0xDA;
// (bits in CCAPM0)
#define DTE0         0x80
#define ECOM0        0x40
#define CAPP0        0x20
#define CAPN0        0x10
#define MAT0         0x08
#define TOG0         0x04
#define PWM0         0x02
#define ECCF0        0x01

sfr CCAPM6         = 0xDA;
// (bits in CCAPM6)
#define BME6         0x80
#define PWM6         0x02

sfr CCAPM1         = 0xDB;
// (bits in CCAPM1)
#define ECOM1        0x40
#define CAPP1        0x20
#define CAPN1        0x10
#define MAT1         0x08
#define TOG1         0x04
#define PWM1         0x02
#define ECCF1        0x01

sfr CCAPM7         = 0xDB;
// (bits in CCAPM7)
#define PWM7         0x02

sfr CCAPM2         = 0xDC;
// (bits in CCAPM2)
#define DTE2         0x80
#define ECOM2        0x40
#define CAPP2        0x20
#define CAPN2        0x10
#define MAT2         0x08
#define TOG2         0x04
#define PWM2         0x02
#define ECCF2        0x01

sfr CCAPM3         = 0xDD;
// (bits in CCAPM3)
#define ECOM3        0x40
#define CAPP3        0x20
#define CAPN3        0x10
#define MAT3         0x08
#define TOG3         0x04
#define PWM3         0x02
#define ECCF3        0x01

sfr CCAPM4         = 0xDE;
// (bits in CCAPM4)
#define DTE4         0x80
#define ECOM4        0x40
#define CAPP4        0x20
#define CAPN4        0x10
#define MAT4         0x08
#define TOG4         0x04
#define PWM4         0x02
#define ECCF4        0x01

sfr CCAPM5         = 0xDF;
// (bits in CCAPM5)
#define ECOM5        0x40
#define CAPP5        0x20
#define CAPN5        0x10
#define MAT5         0x08
#define TOG5         0x04
#define PWM5         0x02
#define ECCF5        0x01

sfr ACC            = 0xE0;

sfr WDTCR          = 0xE1;
// (bits in WDTCR)
#define WREN         0x80
#define NSW          0x40
#define ENW          0x20
#define CLW          0x10
#define WIDL         0x08
#define PS2          0x04
#define PS1          0x02
#define PS0          0x01

sfr IFD            = 0xE2;

sfr IFADRH         = 0xE3;

sfr IFADRL         = 0xE4;

sfr IFMT           = 0xE5;
// (bits in IFMT)
//#define GF           0x80   // Replace by GF_7
//#define GF           0x40	  // Replace by GF_6
//#define GF           0x20   // Replace by GF_5
//#define GF           0x10   // Replace by GF_4
//#define GF           0x08   // Replace by GF_3
#define MS2          0x04
#define MS1          0x02
#define MS0          0x01

sfr SCMD           = 0xE6;

sfr ISPCR          = 0xE7;
// (bits in ISPCR)
#define ISPEN        0x80
#define BS           0x40
#define SRST         0x20
#define CFAIL        0x10
//#define GF           0x08   // Replace by GF_3
#define DATM2        0x04
#define DATM1        0x02
#define DATM0        0x01

sfr P4             = 0xE8;
// (bits in P4)
sbit P47           = P4^7;
sbit P46           = P4^6;
sbit P45           = P4^5;
sbit P44           = P4^4;
sbit P43           = P4^3;
sbit P42           = P4^2;
sbit P41           = P4^1;
sbit P40           = P4^0;

sfr CL             = 0xE9;

sfr CCAP0L         = 0xEA;

sfr CCAP6L         = 0xEA;

sfr CCAP1L         = 0xEB;

sfr CCAP7L         = 0xEB;

sfr CCAP2L         = 0xEC;

sfr CCAP3L         = 0xED;

sfr CCAP4L         = 0xEE;

sfr CCAP5L         = 0xEF;

sfr B              = 0xF0;

sfr PAOE           = 0xF1;
// (bits in PAOE)
#define P47OP4       0x80
#define P41OP2       0x40
#define P40OP2       0x20
#define P24OP2       0x10
#define P26OP4       0x08
#define P21OP0       0x04
#define P20OP0       0x02
#define P22OP0       0x01

sfr PCAPWM0        = 0xF2;
// (bits in PCAPWM0)
#define P0RS1        0x80
#define P0RS0        0x40
#define P0PS2        0x20
#define P0PS1        0x10
#define P0PS0        0x08
#define P0INV        0x04
#define EPC0H        0x02
#define EPC0L        0x01

sfr PCAPWM6        = 0xF2;
// (bits in PCAPWM6)
#define P6RS1        0x80
#define P6RS0        0x40
#define P6PS2        0x20
#define P6PS1        0x10
#define P6PS0        0x08
#define P6INV        0x04
#define EPC6H        0x02
#define EPC6L        0x01

sfr PCAPWM1        = 0xF3;
// (bits in PCAPWM1)
#define P1RS1        0x80
#define P1RS0        0x40
#define P1PS2        0x20
#define P1PS1        0x10
#define P1PS0        0x08
#define P1INV        0x04
#define EPC1H        0x02
#define EPC1L        0x01

sfr PCAPWM7        = 0xF3;
// (bits in PCAPWM7)
#define P7RS1        0x80
#define P7RS0        0x40
#define P7PS2        0x20
#define P7PS1        0x10
#define P7PS0        0x08
#define P7INV        0x04
#define EPC7H        0x02
#define EPC7L        0x01

sfr PCAPWM2        = 0xF4;
// (bits in PCAPWM2)
#define P2RS1        0x80
#define P2RS0        0x40
#define P2PS2        0x20
#define P2PS1        0x10
#define P2PS0        0x08
#define P2INV        0x04
#define EPC2H        0x02
#define EPC2L        0x01

sfr PCAPWM3        = 0xF5;
// (bits in PCAPWM3)
#define P3RS1        0x80
#define P3RS0        0x40
#define P3PS2        0x20
#define P3PS1        0x10
#define P3PS0        0x08
#define P3INV        0x04
#define EPC3H        0x02
#define EPC3L        0x01

sfr PCAPWM4        = 0xF6;
// (bits in PCAPWM4)
#define P4RS1        0x80
#define P4RS0        0x40
#define P4PS2        0x20
#define P4PS1        0x10
#define P4PS0        0x08
#define P4INV        0x04
#define EPC4H        0x02
#define EPC4L        0x01

sfr PCAPWM5        = 0xF7;
// (bits in PCAPWM5)
#define P5RS1        0x80
#define P5RS0        0x40
#define P5PS2        0x20
#define P5PS1        0x10
#define P5PS0        0x08
#define P5INV        0x04
#define EPC5H        0x02
#define EPC5L        0x01

sfr P5             = 0xF8;
// (bits in P5)
sbit P57           = P5^7;
sbit P56           = P5^6;
sbit P55           = P5^5;
sbit P54           = P5^4;
sbit P53           = P5^3;
sbit P52           = P5^2;
sbit P51           = P5^1;
sbit P50           = P5^0;

sfr P6             = 0xF8;
// (bits in P6)
sbit P67           = P6^7;
sbit P66           = P6^6;
sbit P65           = P6^5;
sbit P64           = P6^4;
sbit P63           = P6^3;
sbit P62           = P6^2;
sbit P61           = P6^1;
sbit P60           = P6^0;

sfr CH             = 0xF9;

sfr CCAP0H         = 0xFA;

sfr CCAP6H         = 0xFA;

sfr CCAP1H         = 0xFB;

sfr CCAP7H         = 0xFB;

sfr CCAP2H         = 0xFC;

sfr CCAP3H         = 0xFD;

sfr CCAP4H         = 0xFE;

sfr CCAP5H         = 0xFF;

#endif

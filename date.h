/******************** (C) COPYRIGHT 2009 www.armjishu.com ************************
* File Name          : date.h
* Author             : www.armjishu.com Team
* Version            : V1.0
* Date               : 12/1/2009
* Description        : 日期相关函数
*******************************************************************************/
#ifndef __DATE_H
#define __DATE_H

#include "date.h"

extern xdata struct rtc_time SysTime;

struct rtc_time {
// 	unsigned char  tm_sec;
// 	unsigned char  tm_min;
// 	unsigned char  tm_hour;
// 	unsigned char  tm_mday;
// 	char  tm_mon;
// 	unsigned int   tm_year;
// 	unsigned char  tm_wday;
    
    char  tm_sec;
	char  tm_min;
	char  tm_hour;
	char  tm_mday;
	char  tm_mon;
	int   tm_year;
	char  tm_wday;
};
    
void GregorianDay(struct rtc_time * tm);
unsigned long mktimev(struct rtc_time *tm);
void to_tm(unsigned long tim, struct rtc_time * tm) ;


#endif


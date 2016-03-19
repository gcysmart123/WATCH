#ifndef __MA82G5B32_ADC_H
#define __MA82G5B32_ADC_H

#include <reg_ma82g5b32.h>
#include "typedef.h"

void ADC_Init(void);
unsigned int GetADC_Value(unsigned char channel);

#endif
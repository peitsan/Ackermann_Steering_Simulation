
#ifndef __LQ_ADC_H_
#define __LQ_ADC_H_
#include "include.h"
u16 ADC_Read(uint8_t ADC_Channel_x);
u16 ADC_Read_Average(uint8_t ADC_Channel_x,uint8_t times);
void ADC_Init (void);
void Get_Adc(void);
void Test_ADC(void);
void InductorNormal(void);
extern int16_t LnowADC[4];
extern int16_t ad_max[4]; // 新板子放到赛道标定最大值,会被程序刷新
extern int16_t ad_min[4]; // 新板子据需要标定最小值,会被程序刷新
extern int16_t left, right;
#endif


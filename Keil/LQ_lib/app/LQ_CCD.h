/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 ��ƽ    ̨�������������ܿƼ�STC32G12K128 LQFP64���İ�
 ����    д��chiusir
 ��E-mail  ��chiusir@163.com
 ������汾��V1.1 ��Ȩ���У���λʹ��������ϵ��Ȩ
 �������¡�2022��11��24��
 �������Ϣ�ο����е�ַ��
 ����    վ��http://www.lqist.cn
 ���Ա����̡�http://longqiu.taobao.com
 ------------------------------------------------
 ��IDE��STC32
 ��Target �� keil5.2������
 ��SYS PLL�� 35MHzʹ���ڲ�����ͨ��
=================================================================
stc-isp����ʱ, ѡ��ʱ�� 35MHZ
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#ifndef __LQ_CCD_H_
#define __LQ_CCD_H_

#include "include.h"


#define  CCD1_AO_CHANNEL     4   //ADCͨ��4 P14����
#define  CCD1_CLK_PIN    P40
#define  CCD1_SI_PIN     P66

#define  CCD1_AO_INIT    P1M0 &= 0xef; P1M1 |= 0x10;ADC_Init();
#define  CCD1_AO_GET()   ADC_Read(CCD1_AO_CHANNEL);
#define  CCD1_CLK_INIT   P4M0 |= 0x01; P4M1 &= ~0x01; 
#define  CCD1_SI_INIT    P6M0 |= 0x40; P6M1 &= ~0x40; 

#define  CCD1_CLK_H      CCD1_CLK_PIN = 1
#define  CCD1_CLK_L      CCD1_CLK_PIN = 0

#define  CCD1_SI_H       CCD1_SI_PIN = 1 
#define  CCD1_SI_L       CCD1_SI_PIN = 0


#define  CCD2_AO_CHANNEL     5   //ADCͨ��5 P15����
#define  CCD2_CLK_PIN    P65
#define  CCD2_SI_PIN     P67

#define  CCD2_AO_INIT    P1M0 &= 0xdf; P1M1 |= 0x20;ADC_Init();
#define  CCD2_AO_GET()   ADC_Read(CCD2_AO_CHANNEL);
#define  CCD2_CLK_INIT   P6M0 |= 0x20; P6M1 &= ~0x20; 
#define  CCD2_SI_INIT    P6M0 |= 0x80; P6M1 &= ~0x80; 

#define  CCD2_CLK_H      CCD2_CLK_PIN = 1
#define  CCD2_CLK_L      CCD2_CLK_PIN = 0

#define  CCD2_SI_H       CCD2_SI_PIN = 1 
#define  CCD2_SI_L       CCD2_SI_PIN = 0

#define THRESHOLD          1500         //OLED����ʾ�������ֵ   ��Χ0-4095

void CCD_Init(char ccd_n);
void CCD_Collect(uint16_t *p,char ccd_n);
void Test_CCD(char ccd_n);

#endif


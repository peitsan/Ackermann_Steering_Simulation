/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 【平    台】北京龙邱智能科技STC32G12K128 LQFP64核心板
 【编    写】chiusir
 【E-mail  】chiusir@163.com
 【软件版本】V1.1 版权所有，单位使用请先联系授权
 【最后更新】2022年11月24日
 【相关信息参考下列地址】
 【网    站】http://www.lqist.cn
 【淘宝店铺】http://longqiu.taobao.com
 ------------------------------------------------
 【IDE】STC32
 【Target 】 keil5.2及以上
 【SYS PLL】 35MHz使用内部晶振，通过
=================================================================
stc-isp下载时, 选择时钟 35MHZ
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#ifndef __LQ_CCD_H_
#define __LQ_CCD_H_

#include "include.h"


#define  CCD1_AO_CHANNEL     4   //ADC通道4 P14引脚
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


#define  CCD2_AO_CHANNEL     5   //ADC通道5 P15引脚
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

#define THRESHOLD          1500         //OLED上显示亮灭的阈值   范围0-4095

void CCD_Init(char ccd_n);
void CCD_Collect(uint16_t *p,char ccd_n);
void Test_CCD(char ccd_n);

#endif


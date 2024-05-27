/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 【平    台】北京龙邱智能科技STC32G12K128 LQFP64核心板
 【编    写】chiusir
 【E-mail  】chiusir@163.com
 【软件版本】V1.1 版权所有，单位使用请先联系授权
 【最后更新】2022年3月8日
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
#ifndef __LQ_UART_BT_H_
#define __LQ_UART_BT_H_

#include "include.h"

void UART1_config(void);
void UART1_PutStr(char *p);
void UART1_PutChar(char dat);

char UART1_GetChar(void );

void UART4_config(u16 tmr);
void UART4_PutChar(char ch);
void UART4_PutStr(char *str);
char UART4_GetChar(void );

//匿名地面站上位机接口函数
void UART4_PutBuff(char *st, u16 len);
void UART1_PutBuff(char *st, u16 len);

#endif

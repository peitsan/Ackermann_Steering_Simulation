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

#ifndef __LQ_GPIO_H_
#define __LQ_GPIO_H_

#include "include.h"

typedef enum
{
	P0_0 = 0x00,
	P0_1 = 0x01,
	P0_2 = 0x02,
	P0_3 = 0x03,
	P0_4 = 0x04,
	P0_5 = 0x05,
	P0_6 = 0x06,
	P0_7 = 0x07,

	P1_0 = 0x10,
	//无P1.2
	P1_2 = 0x12,
	P1_3 = 0x13,
	P1_4 = 0x14,
	P1_5 = 0x15,
	P1_6 = 0x16,
	P1_7 = 0x17,

	P2_0 = 0x20,
	P2_1 = 0x21,
	P2_2 = 0x22,
	P2_3 = 0x23,
	P2_4 = 0x24,
	P2_5 = 0x25,
	P2_6 = 0x26,
	P2_7 = 0x27,

	P3_0 = 0x30,
	P3_1 = 0x31,
	P3_2 = 0x32,
	P3_3 = 0x33,
	P3_4 = 0x34,
	P3_5 = 0x35,
	P3_6 = 0x36,
	P3_7 = 0x37,

	P4_0 = 0x40,
	P4_1 = 0x41,
	P4_2 = 0x42,
	P4_3 = 0x43,
	P4_4 = 0x44,
	P4_5 = 0x45,
	P4_6 = 0x46,
	P4_7 = 0x47,
	

	P5_0 = 0x50,
	P5_1 = 0x51,
	P5_2 = 0x52,
	P5_3 = 0x53,
	P5_4 = 0x54,
	//无P5.5 5.6 5.7

	P6_0 = 0x60,
	P6_1 = 0x61,
	P6_2 = 0x62,
	P6_3 = 0x63,
	P6_4 = 0x64,
	P6_5 = 0x65,
	P6_6 = 0x66,
	P6_7 = 0x67,

	P7_0 = 0x70,
	P7_1 = 0x71,
	P7_2 = 0x72,
	P7_3 = 0x73,
	P7_4 = 0x74,
	P7_5 = 0x75,
	P7_6 = 0x76,
	P7_7 = 0x77,
	
}GPIO_Pin;

typedef enum
{
	OUT_IN = 0,						//准双向IO
	OUT_PUT = 1,					//推挽输出
	HIGH_impedance = 2,		//高阻输入（电流既不能流入也不能流出）
	OPEN_OUT = 3,					//开露输出
	
}Port_Mode;



void PIN_InitConfig(GPIO_Pin pin, Port_Mode mode);

#endif


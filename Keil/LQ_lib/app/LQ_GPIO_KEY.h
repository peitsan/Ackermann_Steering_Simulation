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
#ifndef __LQ_KEY_H_
#define __LQ_KEY_H_

#include "include.h"

//定义模块号
typedef enum
{
    KEY0 = 0,  	//母板上按键0
    KEY1 = 1,		//母板上按键1
    KEY2 = 2,		//母板上按键2
	  KEY3 = 3,		//无
    DSW0 = 4,   //母板上拨码开关1
    DSW1 = 5,		//母板上拨码开关2
} KEYn_e;

typedef enum
{
    LOW = 0,  		//按下
    HIGH = 1, 		//松开
    FAIL = 0xff, 	//错误
} KEYs_e;

typedef enum //
{
    NOKEYDOWN = 0, 			//没有按键
		KEY0DOWN = 0x01,  	//母板上按键0
    KEY1DOWN = 0x02,		//母板上按键1
    KEY2DOWN = 0x04,		//母板上按键2
    KEY01DOWN = 0x03,  	//母板上按键0&1
    KEY02DOWN = 0x05,		//母板上按键0&2
    KEY12DOWN = 0x06,		//母板上按键1&2
    KEY012DOWN = 0x07,	//母板上按键0&1&2
    KEYError = 0xFF,		//错误
} KEYdown_e;

//定义的管脚要对应实际按键
#define KEY0p      P70 //母板上按键0
#define KEY1p      P71 //母板上按键1
#define KEY2p      P72 //母板上按键2
#define KEY3p       //母板上按键3（无此按键）

#define DSW0p      P32 //母板上拨码开关
#define DSW1p      P33 //母板上拨码开关

#define REEDp      P37 //母板上干簧管

void GPIO_KEY_Init (void);
unsigned char KEY_Read (KEYn_e KEYno);
unsigned char KEY_Read_All (void);
void Test_GPIO_KEY (void);
void Test_ComKEY_Tft (void);

#endif




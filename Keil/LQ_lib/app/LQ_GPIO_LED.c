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

#include "include.h"
#include "LQ_GPIO.h"


/*************************************************************************
*  函数名称：void LED_Init(void)
*  功能说明：GPIO初始化函数 LED灯所用P42 P62初始化
*  参数说明：无
*  函数返回：无
*  修改时间：2022年3月8日
*  备    注：P62引脚作为OLED灯的引脚，不适合在使用OLED屏幕的情况下使用led灯，此处未初始化
*************************************************************************/
void GPIO_LED_Init(void)
{
	P4M1 = 0x0;
	P4M0 = 0x0;		//P4口设置为准双向口
	P6M1 = 0x0;
	P6M0 = 0x0;		//P6口设置为准双向口
}

/*************************************************************************
*  函数名称：void LED_Ctrl(LEDn_e LEDno, LEDs_e sta)
*  功能说明：LED控制
*  参数说明：LEDn_e LEDno编号,LEDs_e sta状态亮/灭/翻转
*  函数返回：无
*  修改时间：2022年3月8日
*  备    注：
*************************************************************************/
void LED_Ctrl(LEDn_e LEDno, LEDs_e sta)
{
    switch(LEDno)
    {
    case LED0:
      if(sta==ON)        LED0p=0;
      else if(sta==OFF)  LED0p=1;
      else if(sta==RVS) (LED0p)?(LED0p=0):(LED0p=1);
    break;

    case LED1:
      if(sta==ON)        LED1p=0;
      else if(sta==OFF)  LED1p=1;
      else if(sta==RVS) (LED1p)?(LED1p=0):(LED1p=1);
    break;

    case LED2:
      if(sta==ON)        LED2p=0;
      else if(sta==OFF)  LED2p=1;
      else if(sta==RVS) (LED2p)?(LED2p=0):(LED2p=1);
    break;

    case LED3:
      if(sta==ON)        LED3p=0;
      else if(sta==OFF)  LED3p=1;
      else if(sta==RVS) (LED3p)?(LED3p=0):(LED3p=1);
    break;
    case LEDALL:
      if(sta==ON)
      {
    	  LED0p=0;
    	  LED1p=0;
    	  LED2p=0;
				LED3p=0;
      }
      else if(sta==OFF)
      {
    	  LED0p=1;
    	  LED1p=1;
    	  LED2p=1;
				LED3p=1;
      }
      else if(sta==RVS)
      {
    	  (LED0p)?(LED0p=0):(LED0p=1);
    	  (LED1p)?(LED1p=0):(LED1p=1);
    	  (LED2p)?(LED2p=0):(LED2p=1);
    	  (LED3p)?(LED3p=0):(LED3p=1);
      }
    break;
    default:
    break;
    }
}
/**************************************************************************
函数功能：LED闪烁
入口参数：闪烁频率 
返回  值：无
**************************************************************************/
void Led_Flash(u16 time)
{
	  static int temp;
	  if(0==time) LED0p=0;
	  else if(++temp>time)	LED0p=~LED0p,temp=0;
}

/*************************************************************************
*  函数名称：void Test_GPIO_LED(void)
*  功能说明：测试程序
*  参数说明：无
*  函数返回：无
*  修改时间：2022年3月8日
*  备    注：核心板上的蓝色LED灯闪烁，P62引脚作为OLED灯的引脚和核心板绿灯，不适
						 合在使用OLED屏幕的情况下使用led灯，此处未使用，如需要可自行添加
*************************************************************************/

void Test_GPIO_LED(void)
{
	GPIO_LED_Init();
 	while(1)
 	{
		LED_Ctrl(LED0,RVS);        //LED闪烁
 		LED_Ctrl(LED1,RVS);        //LED闪烁
		delayms(200);              
 	}
}

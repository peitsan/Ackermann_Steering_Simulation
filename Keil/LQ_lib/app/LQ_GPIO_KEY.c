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

/*************************************************************************
*  函数名称：void GPIO_KEY_Init(void)
*  功能说明：GPIO初始化函数
*  参数说明：无
*  函数返回：无
*  修改时间：2020年3月10日
*  备    注：
*************************************************************************/
void GPIO_KEY_Init(void)
{
  P7M1 = 0;   P7M0 = 0;   //P7设置为准双向口
	P3M1 = 0;   P3M0 = 0;   //设置为准双向口
}

/*************************************************************************
*  函数名称：unsigned char KEY_Read(KEYn_e KEYno)
*  功能说明：读取按键状态
*  参数说明：KEYn_e KEYno按键编号
*  函数返回：按键状态，0/1
*  修改时间：2022年3月10日
*  备    注：
*************************************************************************/
unsigned char KEY_Read(KEYn_e KEYno)
{
	switch(KEYno)
	{
		case KEY0:
        return P70;				//母板上按键0
				break;

		case KEY1:
        return P71;				//母板上按键1
				break;

		case KEY2:
				if(P72)
					return 1;				//母板上按键2
				else
				{
					while(P72==0);  //等待松开按键
					return 0;	
				}
				break;

		case DSW0:
        return P32;				//母板上拨码开关0
				break;

		case DSW1:
        return P33;				//母板上拨码开关1
				break;
		default:
        return 0XFF;
    }
    return 0;
}

/*************************************************************************
*  函数名称：unsigned char KEY_Read_All(void)
*  功能说明：读取全部按键状态
*  参数说明：无
*  函数返回：按键组合状态，0--7八种状态
*  修改时间：2022年3月8日
*  备    注：读取三个按键状态，方便组合键使用
*************************************************************************/
unsigned char KEY_Read_All(void)
{
	unsigned char tm=0;
	unsigned char k0, k1, k2;
	k0 = P70;		//获取引脚状态
	k1 = P71;
	k2 = P72;
	tm = k0 | (k1<<1) | (k2 << 2);//读取各个按键状态并编码	
	if(tm==0x07)		//没有按键按下0111
	{
		return 0;
	}
	return  (~tm)&0X07;
}
\
/*************************************************************************
*  函数名称：void Test_GPIO_KEY(void)
*  功能说明：测试程序
*  参数说明：无
*  函数返回：无
*  修改时间：2022年3月10日
*  备    注：按下KEY0/1/2任意键 LED全亮
*************************************************************************/
void Test_GPIO_KEY(void)
{
	unsigned char k=0xFF, j=0xff;
	
	GPIO_LED_Init();
	GPIO_KEY_Init();
 	while(1)
 	{
 		k= KEY_Read(KEY0)& KEY_Read(KEY1)& KEY_Read(KEY2);	//判断按键是否按下
		j = KEY_Read(DSW0)& KEY_Read(DSW1);									//判断拨码开关是否打开

 		if(k==0) LED_Ctrl(LED0, ON); 		//按下KEY0/1/2任意键 LE0亮
 		else     LED_Ctrl(LED0, OFF);		//松开KEY0/1/2任意键 LE0灭

		if(j==0) LED_Ctrl(LED1, ON); 		//拨码开关1/2打开 LE0亮
 		else     LED_Ctrl(LED1, OFF);		//拨码开关1/2打开 LE0灭
		
 		delayms(100);                		//延时等待
 	}
 }


/*************************************************************************
*  函数名称：void Test_ComKEY_Tft(void)
*  功能说明：测试组合按键并在彩屏显示
*  参数说明：无
*  函数返回：无
*  修改时间：2022年3月8日
*  备    注：按键状态彩屏显示
*************************************************************************/
void Test_ComKEY_Tft(void)
{
	unsigned char k=0xFF;
	 
  OLED_Init();                                              //OLED初始化
	OLED_CLS();                                               //清屏		
	OLED_P14x16Str(0,0,(unsigned char*)"北京龙邱智能科技");		//字符串显示
	OLED_P6x8Str(0,2,"Long Qiu i.s.t.");	                  	//字符串显示
	delayms(1000);                                            //延时等待
	OLED_CLS();                                               //清屏		
	OLED_P6x8Str(0,1,"KEY Pressed:  ");	                    	//字符串显示
	GPIO_KEY_Init();
 	while(1)
 	{
 		k= KEY_Read_All();
 		switch(k)
 		{
 		  case NOKEYDOWN:
 			 OLED_P6x8Str(35,3,"NO key!  ");		//字符串显示
 		  break;
 		  case KEY0DOWN:
 		 	 OLED_P6x8Str(35,3,"KEY0!    ");		  //字符串显示
 		  break;
 		  case KEY1DOWN:
 		 	 OLED_P6x8Str(35,3,"KEY1!    ");		  //字符串显示
 		  break;
 		  case KEY2DOWN:
 		 	 OLED_P6x8Str(35,3,"KEY2!    ");		  //字符串显示
 		  break;
 		  case KEY01DOWN:
 			 OLED_P6x8Str(35,3,"KEY0&1!  ");		  //字符串显示
 		  break;
 		  case KEY02DOWN:
 		 	 OLED_P6x8Str(35,3,"KEY0&2!  ");		  //字符串显示
 		  break;
 		  case KEY12DOWN:
 		 	 OLED_P6x8Str(35,3,"KEY1&2!  ");		  //字符串显示
 		  break;
 		  case KEY012DOWN:
 		 	 OLED_P6x8Str(35,3,"KEY0&1&2!");	  	//字符串显示
 		  break;
 		  default:
 		  break;
 		 }
 		LED_Ctrl(LED0,RVS);                     //四个LED同时闪烁
 		delayms(100);                           //延时等待
 	}
 }




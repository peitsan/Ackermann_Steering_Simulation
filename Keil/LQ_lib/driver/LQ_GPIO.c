/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】北京龙邱智能科技STC32G12K128
【编    写】chiusir
【E-mail  】chiusir@163.com
【软件版本】V1.0 版权所有，单位使用请先联系授权
【最后更新】2022年3月3日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://longqiu.taobao.com
------------------------------------------------
【dev.env.】Keil5.2及以上版本
【Target 】 STC32G12K128
【SYS PLL】 35MHz
------------------------------------------------
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#include "include.h"
#include "LQ_GPIO.h"

/*************************************************************************
*  函数名称：void PIN_InitConfig(GPIO_Name_t pin, IfxPort_Mode mode, unsigned char output)
*  功能说明：GPIO初始化函数
*  参数说明：pin      ： 管脚标号    lq_gpio.h中定义
*            mode     ： GPIO 模式   lq_gpio.h中的宏定义中选择
*            output   ： 输出模式时，输出电平 1：高电平  0：低电平
*  函数返回：无
*  修改时间：2020年3月10日
*  举    例： PIN_InitConfig(P00_0, PIN_MODE_OUTPUT, 1);  //P00_0初始化推挽输出 高电平
*************************************************************************/
void PIN_InitConfig(GPIO_Pin pin, Port_Mode mode)
{
	unsigned int pin_num, port_num;
	
	pin_num = (pin&0xF0)>>4;
	port_num = pin & 0x0F;
	
	switch(pin_num)
	{
		case 0:
			if(mode == 0){P0M1 &= ~(0x01<<port_num);	P0M0 &= ~(0x01<<port_num);}
			if(mode == 1){P0M1 &= ~(0x01<<port_num);	P0M0 |= ~(0x01<<port_num);}
			if(mode == 2){P0M1 |= ~(0x01<<port_num);	P0M0 &= ~(0x01<<port_num);}
			if(mode == 3){P0M1 |= ~(0x01<<port_num);	P0M0 |= ~(0x01<<port_num);}
			break;
			
			case 1:
			if(mode == 0){P1M1 &= ~(0x01<<port_num);	P1M0 &= ~(0x01<<port_num);}
			if(mode == 1){P1M1 &= ~(0x01<<port_num);	P1M0 |= ~(0x01<<port_num);}
			if(mode == 2){P1M1 |= ~(0x01<<port_num);	P1M0 &= ~(0x01<<port_num);}
			if(mode == 3){P1M1 |= ~(0x01<<port_num);	P1M0 |= ~(0x01<<port_num);}
			break;
			
			case 2:
			if(mode == 0){P4M1 &= ~(0x01<<port_num);	P4M0 &= ~(0x01<<port_num);}
			if(mode == 1){P4M1 &= ~(0x01<<port_num);	P4M0 |= ~(0x01<<port_num);}
			if(mode == 2){P4M1 |= ~(0x01<<port_num);	P4M0 &= ~(0x01<<port_num);}
			if(mode == 3){P4M1 |= ~(0x01<<port_num);	P4M0 |= ~(0x01<<port_num);}
			break;
			
			case 3:
			if(mode == 0){P3M1 &= ~(0x01<<port_num);	P3M0 &= ~(0x01<<port_num);}
			if(mode == 1){P3M1 &= ~(0x01<<port_num);	P3M0 |= ~(0x01<<port_num);}
			if(mode == 2){P3M1 |= ~(0x01<<port_num);	P3M0 &= ~(0x01<<port_num);}
			if(mode == 3){P3M1 |= ~(0x01<<port_num);	P3M0 |= ~(0x01<<port_num);}
			break;
			
		case 4:
			if(mode == 0){P4M1 &= ~(0x01<<port_num);	P4M0 &= ~(0x01<<port_num);}
			if(mode == 1){P4M1 &= ~(0x01<<port_num);	P4M0 |= ~(0x01<<port_num);}
			if(mode == 2){P4M1 |= ~(0x01<<port_num);	P4M0 &= ~(0x01<<port_num);}
			if(mode == 3){P4M1 |= ~(0x01<<port_num);	P4M0 |= ~(0x01<<port_num);}
			break;
		
		case 5:
			if(mode == 0){P5M1 &= ~(0x01<<port_num);	P5M0 &= ~(0x01<<port_num);}
			if(mode == 1){P5M1 &= ~(0x01<<port_num);	P5M0 |= ~(0x01<<port_num);}
			if(mode == 2){P5M1 |= ~(0x01<<port_num);	P5M0 &= ~(0x01<<port_num);}
			if(mode == 3){P5M1 |= ~(0x01<<port_num);	P5M0 |= ~(0x01<<port_num);}
			break;
			
		case 6:
			if(mode == 0){P6M1 &= ~(0x01<<port_num);	P6M0 &= ~(0x01<<port_num);}
			if(mode == 1){P6M1 &= ~(0x01<<port_num);	P6M0 |= ~(0x01<<port_num);}
			if(mode == 2){P6M1 |= ~(0x01<<port_num);	P6M0 &= ~(0x01<<port_num);}
			if(mode == 3){P6M1 |= ~(0x01<<port_num);	P6M0 |= ~(0x01<<port_num);}
			break;
			
		case 7:
			if(mode == 0){P7M1 &= ~(0x01<<port_num);	P7M0 &= ~(0x01<<port_num);}
			if(mode == 1){P7M1 &= ~(0x01<<port_num);	P7M0 |= ~(0x01<<port_num);}
			if(mode == 2){P7M1 |= ~(0x01<<port_num);	P7M0 &= ~(0x01<<port_num);}
			if(mode == 3){P7M1 |= ~(0x01<<port_num);	P7M0 |= ~(0x01<<port_num);}
			break;
			
	}

}








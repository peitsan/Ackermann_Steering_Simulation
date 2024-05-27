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
#include "LQ_CCD.h"


/**
  * @brief    CCD管脚初始化
  *
  * @param    ccd_n:CCD序号，一个两个CCD接口,输入1、2
  *
  * @return   无
  *
  * @note     
  *
  * @example  
  *
  * @date     2023/11/24
*/
void CCD_Init(char ccd_n) 
{
    if(ccd_n == 1)
    {
        CCD1_AO_INIT;
        CCD1_CLK_INIT;
        CCD1_SI_INIT;
    }
    else if(ccd_n == 2)
    {
        CCD2_AO_INIT;
        CCD2_CLK_INIT;
        CCD2_SI_INIT;
    }
}

void CCD_Delayus(uint16_t us)
{
    delayus(us);
}

/**
  * @brief    CCD采集函数
  *
  * @param    p  ：  存放采集数据的首地址
  *
  * @return   无
  *
  * @note     
  *
  * @example  uint16_t ccd_buf[128];
  *           CCD_Collect(ccd_buf); 
  *
*/
void CCD_Collect(uint16_t *p,char ccd_n)
{
	unsigned char i;
	unsigned int  temp = 0;
    if(ccd_n == 1)
    {
        CCD1_SI_H;              //SI  = 1 
        CCD_Delayus(1);
        CCD1_CLK_H;             // CLK = 1 
        CCD_Delayus(1);
        CCD1_SI_L;              // SI  = 0 
        CCD_Delayus(1);

        for(i=0; i<128; i++) 
        {
            CCD_Delayus(1);
            CCD1_CLK_H;         // CLK = 1 
            CCD_Delayus(1);
            temp = CCD1_AO_GET();       
            *p++ = temp;     
            CCD1_CLK_L;        // CLK = 0 
        }
        CCD_Delayus(1);
        CCD1_CLK_H;            // CLK = 1 
        CCD_Delayus(1);
        CCD1_CLK_L;            // CLK = 0 
    }
    else if(ccd_n == 2)
    {
        CCD2_SI_H;              //SI  = 1 
        CCD_Delayus(1);
        CCD2_CLK_H;             // CLK = 1 
        CCD_Delayus(1);
        CCD2_SI_L;              // SI  = 0 
        CCD_Delayus(1);

        for(i=0; i<128; i++) 
        {
            CCD_Delayus(1);
            CCD2_CLK_H;         // CLK = 1 
            CCD_Delayus(1);
            temp = CCD2_AO_GET();       
            *p++ = temp;     
            CCD2_CLK_L;        // CLK = 0 
        }
        CCD_Delayus(1);
        CCD2_CLK_H;            // CLK = 1 
        CCD_Delayus(1);
        CCD2_CLK_L;            // CLK = 0 
    }
}

/**
  * @brief    测试CCD  OLED画图
  *
  * @param    无
  *
  * @return   无
  *
  * @note     
  *
  * @example  
  *
*/
void Test_CCD(char ccd_n)
{
	uint32_t xdata now_time = 0;
	uint16_t xdata ccd_buf[128];
	static uint8_t xdata count = 0;
	uint8_t xdata i = 0;
	
	GPIO_LED_Init();					//LED初始化
	OLED_Init();
    OLED_CLS();
	CCD_Init(ccd_n);
	
	OLED_P8x16Str(15,0,"LQ CCD Test");  	
	
	while(1)
	{
        /* 采集ccd图像 */
        CCD_Collect(ccd_buf,ccd_n);
        
        count++;
        /* OLED 显示 */
        if(count % 3 == 0)
        {
            for(i=0; i<128; i++)
            {
                OLED_Set_Pos(i , 7); 
                if(ccd_buf[i] > THRESHOLD)	      			
                    OLED_WrDat(0xFF);
                else
                    OLED_WrDat(0x00);	
            }
        
        }
        if(count == 240)
            count = 0;
        delayms(80);
    }
}













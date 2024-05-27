/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】北京龙邱智能科技自平衡摩托车
【编    写】chiusir
【E-mail  】chiusir@163.com
【软件版本】V1.1 版权所有，单位使用请先联系授权
【最后更新】2020年4月10日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://longqiu.taobao.com
------------------------------------------------
【dev.env.】Keil5.2及以上版本
【Target 】 STC16F40K
【SYS PLL】 36MHz
------------------------------------------------
下载时, 选择时钟 30MHZ (用户可自行修改频率).
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/


#include "include.h"


int16_t ad_max[4] = {500,500};  // 新板子放到赛道标定最大值,会被程序刷新
int16_t ad_min[4] = { 0,0};   	// 新板子据需要标定最小值,会被程序刷新
int16_t left = 0, right = 0;
int16_t LnowADC[4];
u16 Voltage; 
/********************************************************************************************************
**函数信息 ：ADC1_SingleChannel_Get(uint8_t ADC_Channel_x)       
**功能描述 ：获取ADC1转换数据
**输入参数 ：ADC_Channel_x , x为0~11
**输出参数 ：puiADData为ADC读到的值，默认是12位
********************************************************************************************************/
u16 ADC_Read(uint8 ADC_Channel_x)
{ 
    ADC_RES = 0;
    ADC_RESL = 0;

    ADC_CONTR = (ADC_CONTR & 0xF0) | 0x40 | ADC_Channel_x;    //启动 AD 转换
    _nop_();
    _nop_();
    _nop_();
    _nop_();

    while((ADC_CONTR & 0x20) == 0)  ;   //wait for ADC finish
    ADC_CONTR &= ~0x20;     																	//清除ADC结束标志
    return  (((u16)ADC_RES << 8) | ADC_RESL);//默认是12位
}

/********************************************************************************************************
**函数信息 ：Get_Adc_Average(uint8_t ADC_Channel_x,uint8_t times)     
**功能描述 ：获取几次ADC1采样值的平均值
**输入参数 ：ADC_Channel_x , x为0~8
**输出参数 ：puiADData为ADC读到的值
********************************************************************************************************/
u16 ADC_Read_Average(uint8 ADC_Channel_x,uint8 times)
{
    u32 temp_val=0;
    u8 t;
    u8 delay;
    for(t=0;t<times;t++)
    {
        temp_val+=ADC_Read(ADC_Channel_x);
        for(delay=0;delay<100;delay++);
    }
    return temp_val/times;
} 
/*************************************************************************
 *  函数名称：void InductorInit (void)
 *  功能说明：四个电感ADC初始化函数；
 *  参数说明：无
 *  函数返回：无
 *  修改时间：2020年11月18日
 *  备    注：
 *************************************************************************/
void ADC_Init (void)
{
		P0M1 |= 0x0f;
	  P0M0 &= ~0x0f;
		P1M1 |= 0x03;
		P1M0 &= ~0x03;
	
    P_SW2 |= 0x80;
    ADCTIM = 0x3f;					//设置 ADC 内部时序，ADC采样时间建议设最大值
    P_SW2 &= 0x7f;
    ADCCFG = 0x2f;					//设置 ADC 时钟为系统时钟/2/16/16
    ADC_CONTR = 0x80; 			//使能 ADC 模块
}

/**************************************************************************
函数功能：读取电感值 
入口参数：无
返回  值：无
**************************************************************************/
void Get_Adc(void)
{	
	LnowADC[0]=ADC_Read(8);      //读取ADC11通道值
	LnowADC[1]=ADC_Read(9);			//读取ADC10通道值
	LnowADC[2]=ADC_Read(10); 			//读取ADC9通道值
	LnowADC[3]=ADC_Read(11);				//读取ADC8通道值	
}
/*************************************************************************
 *  函数名称：void InductorNormal(void)
 *  功能说明：采集电感电压并并归一化；
 *  函数返回：无
 *  修改时间：2020年11月26日
 *  备    注： 
 *************************************************************************/
void InductorNormal(void)
{
    Get_Adc();  //读取ADC
	  if (LnowADC[0] < ad_min[0])            ad_min[0] = LnowADC[0];     // 刷新最小值
    else if (LnowADC[0] > ad_max[0])       ad_max[0] = LnowADC[0];     // 刷新最大值
    if (LnowADC[1] < ad_min[1])            ad_min[1] = LnowADC[1];
    else if (LnowADC[1] > ad_max[1])       ad_max[1] = LnowADC[1]; 
    if (LnowADC[2] < ad_min[2])            ad_min[2] = LnowADC[2];
    else if (LnowADC[2] > ad_max[2])       ad_max[2] = LnowADC[2];
    if (LnowADC[3] < ad_min[3])            ad_min[3] = LnowADC[3];
    else if (LnowADC[3] > ad_max[3])       ad_max[3] = LnowADC[3];	
	
	  left = (LnowADC[0] - ad_min[0]) * 100 / (ad_max[0] - ad_min[0]);     // 各偏移量归一化到0--1000以内
    right = (LnowADC[1] - ad_min[1]) * 100 / (ad_max[1] - ad_min[1]);
}
/*************************************************************************
*  函数名称：void Test_ADC(void)
*  功能说明：测试程序
*  参数说明：无
*  函数返回：无
*  修改时间：2020年3月10日
*  备    注：屏幕显示ADC采集的电感值
*************************************************************************/
void Test_ADC(void)
 {
	char txt[30]; 
	OLED_Init();    //OLED初始化
	OLED_CLS();  	 	//清屏		
	OLED_P14x16Str(0,2,(unsigned char*)"北京龙邱智能科技");		//字符串显示
	OLED_P6x8Str(0,4,"Long Qiu i.s.t.");		//字符串显示
	delayms(1000);  //延时等待
	OLED_CLS();     //清屏		
	ADC_Init();			//ADC初始化
 	while(1)
 	{
		Get_Adc();	
		sprintf(txt,"AD0:%04d ",LnowADC[0]);  //显示ADC11的值
		OLED_P8x16Str(0,0,(u8*)txt);
		UART4_PutStr(txt);
		
		sprintf(txt,"AD1:%04d ",LnowADC[1]);	//显示ADC10的值
		OLED_P8x16Str(0,2,(u8*)txt);
		UART4_PutStr(txt);
		
		sprintf(txt,"AD2:%04d ",LnowADC[2]);	//显示ADC9的值
		OLED_P8x16Str(0,4,(u8*)txt);
		UART4_PutStr(txt);
		
		sprintf(txt,"AD3:%04d ",LnowADC[3]);	//显示ADC8的值
		OLED_P8x16Str(0,6,(u8*)txt);
		UART4_PutStr(txt);UART4_PutStr("\n ");
		}
}
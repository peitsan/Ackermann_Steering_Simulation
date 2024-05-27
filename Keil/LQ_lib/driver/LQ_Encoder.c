#include "include.h"


/**************************************************************************
函数功能：单位时间读取编码器计数
入口参数：定时器
返回  值：脉冲数，注意读取之间周期内，不要超过16位最大值
**************************************************************************/
short Read_Encoder(u8 encno)
{
  int32 tm=0;
  if(encno==1)
  {
		T4T3M &= ~(1<<7);
		tm = T4H;
		if(P07)//编码器２计数A:P06  DIR:P07 	
			tm=(tm<<8)|T4L;	
		else 
			tm=0-((tm<<8)|T4L);	
		
		
		T4H = 0;
		T4L = 0;
		T4T3M |= (1<<7);		
  }
  else if(encno==2)
  {
		tm = T3H;
		if(P05)//编码器２计数A:P04  DIR:P05 	
			tm=(tm<<8)|T3L;	
		else
			tm=0-((tm<<8)|T3L);	
		
		T4T3M &= ~(1<<3);
		T3H = 0;
		T3L = 0;
		T4T3M |= (1<<3);		
  }	
  return tm;
}

/*************************************************************************
*  函数名称：void Timer4EncInit(void)
*  功能说明：编码器初始化函数
*  参数说明：无
*  函数返回：无
*  修改时间：2020年12月1日
*  备    注：驱动2个编码器
*************************************************************************/
void Timer34EncInit (void)
{   
	T3L = 0; 
	T3H = 0;
	
	T4L = 0;
	T4H = 0;
	T4T3M |= 0xCC; //1100 1100 定时器4开始计数  定时器4计数模式  12分频  关闭时钟输出  定时器3开始计数  定时器3计数模式 12分频 关闭时钟输出

}

/*************************************************************************
 *  函数名称：void TestEncoder(void)
 *  功能说明：测试程序，OLED显示
 *  参数说明：无
 *  函数返回：无
 *  修改时间：2022年3月10日
 *  备    注：
 *************************************************************************/

void TestEncoder(void)
{
    char txt[16];
    short duty = 20;
	  short enc1,enc2;

		OLED_Init();
    OLED_CLS();   	                         	//清屏
    OLED_P6x8Str(0, 0, "Test Encoder");       //字符串显示
    Timer34EncInit();                          //编码器接口初始化
//    MotorInit(MOTOR_FREQUENCY);    
    while (1)
    {
        if (KEY_Read(KEY0) == 0)      //按下KEY0键，占空比减小
        {
            if (duty > -PWMB_CLOCK/MOTOR_FREQUENCY)//满占空比为10M/12.5k=800
                duty -= 50;
        }
        if (KEY_Read(KEY2) == 0)      //按下KEY2键，占空比加大
        {
            if (duty < PWMB_CLOCK/MOTOR_FREQUENCY)  //满占空比为10M/12.5k=800
                duty += 50;
        }
        if (KEY_Read(KEY1) == 0)      //按下KEY1键，占空比中值
        {
            duty = 20;
        }

//        MotorCtrl(duty,duty);

        /* 获取编码器值 */
        enc1 = Read_Encoder(1); 			        //左电机 母板上编码器1，小车前进为负值
        enc2 = Read_Encoder(2); 			        //右电机 母板上编码器2，小车前进为正值

        sprintf(txt, "Enc1: %04d  ", enc1);
        OLED_P6x8Str(0, 3, txt);			        //字符串显示
				UART4_PutStr(txt);
        sprintf(txt, "PWM: %05d;", duty);
        OLED_P6x8Str(0, 4, txt);   //字符串显示
        sprintf(txt, "Enc2: %04d \n", enc2);
        OLED_P6x8Str(0, 5, txt);			        //字符串显示
				UART4_PutStr(txt);

        LED_Ctrl(LED0, RVS);       		        //电平翻转,LED闪烁
        delayms(50);							            //延时等待
    }
}



/*************************************************************************
 *  函数名称：void Encoder(void)
 *  功能说明：读取编码器，OLED显示
 *  参数说明：无
 *  函数返回：无
 *  修改时间：2024年5月16日
 *  备    注：
 *************************************************************************/
void Encoder(void)
{
    char txt[16];
	short enc1,enc2;

	/* 获取编码器值 */
	enc1 = Read_Encoder(1); 			        //左电机 母板上编码器1，小车前进为负值
	enc2 = Read_Encoder(2); 			        //右电机 母板上编码器2，小车前进为正值
	
	sprintf(txt, "Enc1: %04d", enc1);
	OLED_P6x8Str(0, 1, txt);			        //字符串显示

	sprintf(txt, "Enc2: %04d", enc2);
	OLED_P6x8Str(0, 3, txt);			        //字符串显示

}



#include "include.h"

#define Timer0_Int_Freq  200
#define Timer1_Int_Freq  1000
#define Timer2_Int_Freq  100

#define Timer0_Reload   (MAIN_Fosc /Timer0_Int_Freq)      //Timer 0 中断频率, n次/秒
#define Timer1_Reload   (MAIN_Fosc /Timer1_Int_Freq)      //Timer 1 中断频率, n次/秒
#define Timer2_Reload   (MAIN_Fosc /Timer2_Int_Freq)      //Timer 2 中断频率, n次/秒
#define Timer3_Reload   (MAIN_Fosc / 400)                 //Timer 3 中断频率, 400次/秒
#define Timer4_Reload   (MAIN_Fosc / 500)                 //Timer 4 中断频率, 500次/秒

//延时函数
void delayms(u16 ms)
{
  u16 i;
  do{    
		i=MAIN_Fosc/6000;//大约1毫秒
    while(--i);
  }while(--ms);
}
//========================================================================
// 函数: void timer0_int (void) interrupt TMR0_VECTOR
// 描述: timer0中断函数.
// 参数: none.
// 返回: none.
// 版本: V1.0, 2015-1-12
//========================================================================
void timer0_int (void) interrupt 1
{
  LED_Ctrl(LED0, RVS);
}

//========================================================================
// 函数: void timer1_int (void) interrupt TMR1_VECTOR
// 描述: timer1中断函数.
// 参数: none.
// 返回: none.
// 版本: V1.0, 2015-1-12
//========================================================================
void timer1_int (void) interrupt 3 
{
	LED_Ctrl(LED0, RVS);
}

//========================================================================
// 函数: void timer2_int (void) interrupt TMR2_VECTOR
// 描述: timer2中断函数.
// 参数: none.
// 返回: none.
// 版本: V1.0, 2015-1-12
//========================================================================
void timer2_int (void) interrupt 12
{
  LED_Ctrl(LED0, RVS);//测试用
}

//========================================================================
// 函数: void timer3_int (void) interrupt TMR3_VECTOR
// 描述: timer2中断函数.
// 参数: none.
// 返回: none.
// 版本: V1.0, 2015-1-12
//========================================================================
void timer3_int (void) interrupt 19
{
	LED_Ctrl(LED0, RVS);
}

//========================================================================
// 函数: void timer4_int (void) interrupt TMR4_VECTOR
// 描述: timer4中断函数.
// 参数: none.
// 返回: none.
// 版本: V1.0, 2015-1-12
//========================================================================
void timer4_int (void) interrupt 20
{
	LED_Ctrl(LED0, RVS);
}

//========================================================================
// 函数: void   Timer0_init(void)
// 描述: timer0初始化函数.
// 参数: none.
// 返回: none.
// 版本: V1.0, 2015-1-12
//========================================================================
void Timer0_init(void)
{
  TR0 = 0;    //停止计数
  
#if (Timer0_Reload < 64)    						// 如果用户设置值不合适， 则不启动定时器
	#error "Timer0设置的中断过快!"
		
#elif ((Timer0_Reload/12) < 65536UL)    // 如果用户设置值不合适， 则不启动定时器
  ET0 = 1;    	                      	//允许中断
  //  PT0 = 1;                          //高优先级中断
  TMOD &= ~0x03;
  TMOD |= 0;                            //工作模式, 0: 16位自动重装, 1: 16位定时/计数, 2: 8位自动重装, 3: 16位自动重装, 不可屏蔽中断
  //  TMOD |=  0x04;                    //对外计数或分频
  TMOD &= ~0x04;  											//定时
  //  INTCLKO |=  0x01;                 //输出时钟
  INTCLKO &= ~0x01;                     //不输出时钟
  
	#if (Timer0_Reload < 65536UL)
		AUXR |=  0x80;                        //1T mode
		TH0 = (u8)((65536UL - Timer0_Reload) / 256);
		TL0 = (u8)((65536UL - Timer0_Reload) % 256);
	#else
		AUXR &= ~0x80;  //12T mode
		TH0 = (u8)((65536UL - Timer0_Reload/12) / 256);
		TL0 = (u8)((65536UL - Timer0_Reload/12) % 256);
	#endif
  
  TR0 = 1;                              //开始运行
  
#else
	#error "Timer0设置的中断过慢!"
#endif
}

//========================================================================
// 函数: void   Timer1_init(void)
// 描述: timer1初始化函数.
// 参数: none.
// 返回: none.
// 版本: V1.0, 2015-1-12
//========================================================================
void    Timer1_init(void)
{
  TR1 = 0;                               //停止计数
  
#if (Timer1_Reload < 64)                 // 如果用户设置值不合适， 则不启动定时器
#error "Timer1设置的中断过快!"
  
#elif ((Timer1_Reload/12) < 65536UL)     // 如果用户设置值不合适， 则不启动定时器
  ET1 = 1;    	                         //允许中断
  //  PT1 = 1;                           //高优先级中断
  TMOD &= ~0x30;
  TMOD |= (0 << 4);                      //工作模式, 0: 16位自动重装, 1: 16位定时/计数, 2: 8位自动重装
  //  TMOD |=  0x40;                     //对外计数或分频
  TMOD &= ~0x40;  //定时
  //  INTCLKO |=  0x02;                  //输出时钟
  INTCLKO &= ~0x02;  		                 //不输出时钟
  
#if (Timer1_Reload < 65536UL)
  AUXR |=  0x40;                         //1T mode
  TH1 = (u8)((65536UL - Timer1_Reload) / 256);
  TL1 = (u8)((65536UL - Timer1_Reload) % 256);
#else
  AUXR &= ~0x40;                         //12T mode
  TH1 = (u8)((65536UL - Timer1_Reload/12) / 256);
  TL1 = (u8)((65536UL - Timer1_Reload/12) % 256);
#endif
  
  TR1 = 1;                               //开始运行
  
#else
#error "Timer1设置的中断过慢!"
#endif
}

//========================================================================
// 函数: void   Timer2_init(void)
// 描述: timer2初始化函数.
// 参数: none.
// 返回: none.
// 版本: V1.0, 2015-1-12
//========================================================================
void    Timer2_init(void)
{
  AUXR &= ~0x1c;      			             //停止计数, 定时模式, 12T模式
   
#if (Timer2_Reload < 64)                 // 如果用户设置值不合适， 则不启动定时器
#error "Timer2设置的中断过快!"
  
#elif ((Timer2_Reload/12) < 65536UL)     // 如果用户设置值不合适， 则不启动定时器
//  ET2 = 1;    			                    	//允许中断
  //  INTCLKO |=  0x04;                  //输出时钟
  INTCLKO &= ~0x04;                      //不输出时钟
  
#if (Timer2_Reload < 65536UL)
  AUXR |=  (1<<2);                       //1T mode
  T2H = (u8)((65536UL - Timer2_Reload) / 256);
  T2L = (u8)((65536UL - Timer2_Reload) % 256);
#else
  T2H = (u8)((65536UL - Timer2_Reload/12) / 256);
  T2L = (u8)((65536UL - Timer2_Reload/12) % 256);
#endif
  
  AUXR |=  (1<<4);                       //开始运行
  
#else
#error "Timer2设置的中断过慢!"
#endif
}

//========================================================================
// 函数: void   Timer3_init(void)
// 描述: timer3初始化函数.
// 参数: none.
// 返回: none.
// 版本: V1.0, 2015-1-12
//========================================================================
void    Timer3_init(void)
{
  T4T3M &= ~0x0f;                         //停止计数, 定时模式, 12T模式, 不输出时钟
  
#if (Timer3_Reload < 64)                  // 如果用户设置值不合适， 则不启动定时器
#error "Timer3设置的中断过快!"
  
#elif ((Timer3_Reload/12) < 65536UL)      // 如果用户设置值不合适， 则不启动定时器
//  ET3 = 1;                                //允许中断
  //  T4T3M |=  0x01;                     //输出时钟
  //  T4T3M &= ~0x01;                     //不输出时钟
  
#if (Timer3_Reload < 65536UL)
  T4T3M |=  (1<<1);                       //1T mode
  T3H = (u8)((65536UL - Timer3_Reload) / 256);
  T3L = (u8)((65536UL - Timer3_Reload) % 256);
#else
  T3H = (u8)((65536UL - Timer3_Reload/12) / 256);
  T3L = (u8)((65536UL - Timer3_Reload/12) % 256);
#endif
  
  T4T3M |=  (1<<3);                       //开始运行
  
#else
#error "Timer3设置的中断过慢!"
#endif
}

//========================================================================
// 函数: void   Timer4_init(void)
// 描述: timer4初始化函数.
// 参数: none.
// 返回: none.
// 版本: V1.0, 2015-1-12
//========================================================================
void    Timer4_init(void)
{
  T4T3M &= ~0xf0;     			              //停止计数, 定时模式, 12T模式, 不输出时钟
  
#if (Timer4_Reload < 64)                  // 如果用户设置值不合适， 则不启动定时器
#error "Timer4设置的中断过快!"
  
#elif ((Timer4_Reload/12) < 65536UL)      // 如果用户设置值不合适， 则不启动定时器
//  ET4 = 1;    //允许中断
  //  T4T3M |=  0x10;                   	//输出时钟
  //  T4T3M &= ~0x10;    	                //不输出时钟
  
#if (Timer4_Reload < 65536UL)
  T4T3M |=  (1<<6);    			              //12T mode ;P0.6计数
  T4H = (u8)((65536UL - Timer4_Reload) / 256);
  T4L = (u8)((65536UL - Timer4_Reload) % 256);
#else
  T4H = (u8)((65536UL - Timer4_Reload/12) / 256);
  T4L = (u8)((65536UL - Timer4_Reload/12) % 256);
#endif
  
  T4T3M |=  (1<<7);    			              //开始运行
  
#else
#error "Timer4设置的中断过慢!"
#endif
}

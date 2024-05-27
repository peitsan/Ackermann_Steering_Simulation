#include "include.h"

#define USEDRV8701     //使用不同的驱动，定义不同的宏定义
//#define USE7843OR7971//使用不同的驱动，定义不同的宏定义

#define PWM1_0      0x00	//P:P1.0  N:P1.1
#define PWM1_1      0x01	//P:P2.0  N:P2.1 
#define PWM1_2      0x02	//P:P6.0  N:P6.1

#define PWM2_0      0x00	//P:P1.2/P5.4  N:P1.3
#define PWM2_1      0x04	//P:P2.2  N:P2.3
#define PWM2_2      0x08	//P:P6.2  N:P6.3

#define PWM3_0      0x00	//P:P1.4  N:P1.5
#define PWM3_1      0x10	//P:P2.4  N:P2.5
#define PWM3_2      0x20	//P:P6.4  N:P6.5

#define PWM4_0      0x00	//P:P1.6  N:P1.7
#define PWM4_1      0x40	//P:P2.6  N:P2.7
#define PWM4_2      0x80	//P:P6.6  N:P6.7
#define PWM4_3      0xC0	//P:P3.4  N:P3.3

#define PWM5_0      0x00	//P2.0
#define PWM5_1      0x01	//P1.7
#define PWM5_2      0x02	//P0.0
#define PWM5_3      0x03	//P7.4

#define PWM6_0      0x00	//P2.1
#define PWM6_1      0x04	//P5.4
#define PWM6_2      0x08	//P0.1
#define PWM6_3      0x0C	//P7.5

#define PWM7_0      0x00	//P2.2
#define PWM7_1      0x10	//P3.3
#define PWM7_2      0x20	//P0.2
#define PWM7_3      0x30	//P7.6

#define PWM8_0      0x00	//P2.3
#define PWM8_1      0x40	//P3.4
#define PWM8_2      0x80	//P0.3
#define PWM8_3      0xC0	//P7.7

#define ENO5P       0x01
#define ENO6P       0x04
#define ENO7P       0x10
#define ENO8P       0x40
#define ENO1P       0x01
#define ENO1N       0x02
#define ENO2P       0x04
#define ENO2N       0x08
#define ENO3P       0x10
#define ENO3N       0x20
#define ENO4P       0x40
#define ENO4N       0x80


u16 PWM1_Duty;
u16 PWM2_Duty;
u16 PWM3_Duty;
u16 PWM4_Duty;

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【函数名】void MotorInit(u16 freq)
【功  能】电机PWM初始化
【参数值】int Encoder 编码器脉采集的冲数
【参数值】int Target  期望脉冲数
【返回值】电机PWM 
【作  者】chiusir
【最后更新】2021年1月22日 
【软件版本】V1.0
【调用举例】驱动电机  TIM4_PWM_Init(1000-1);  //1KHZ周期
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#ifdef USE7843OR7971  		//不看
void MotorInit(u16 freq)  	//不看
{
	P_SW2 |= 0x80;
	PWMB_PS = 0xff;		//切换PWM引脚为 7.4 7.5 7.6 7.7
	PWMB_PSCRL = (MAIN_Fosc/PWMB_CLOCK)-1;   	//主频为22M,PWM分频到10M范围 
	PWMB_CCER1 = 0x00;												//写 CCMRx 前必须先清零 CCxE 关闭通道
  PWMB_CCER2 = 0x00;
  PWMB_CCMR1 = 0x60; 												//通道模式配置
  PWMB_CCMR2 = 0x60;
  PWMB_CCMR3 = 0x60;
  PWMB_CCMR4 = 0x60;
  PWMB_CCER1 = 0x33; 												//配置通道输出使能和极性
  PWMB_CCER2 = 0x33;
  
  PWMB_ARRH  = (u8)((PWMB_CLOCK/freq)>>8);  //设置周期时间，从0计数到设定数值
  PWMB_ARRL  = (u8)(PWMB_CLOCK/freq); //  
  
  PWMB_CCR1H = 0;
  PWMB_CCR1L = 20; 	//设置占空比时间
  PWMB_CCR2H = 0;
  PWMB_CCR2L = 20; 	//设置占空比时间	
  PWMB_CCR3H = 0;
  PWMB_CCR3L = 20; 	//设置占空比时间
  PWMB_CCR4H = 0;	
  PWMB_CCR4L = 20; 	//设置占空比时间
  
  PWMB_ENO = 0x00;
  PWMB_ENO |= ENO5P; //使能输出
  PWMB_ENO |= ENO6P; //使能输出 
  PWMB_ENO |= ENO7P; //使能输出
  PWMB_ENO |= ENO8P; //使能输出 
  PWMB_PS = 0x00;    //高级 PWM 通道输出脚选择位
  PWMB_PS |= PWM5_3; //选择 PWM5_3 通道
  PWMB_PS |= PWM6_3; //选择 PWM6_3 通道 //高级 PWM 通道 4P 输出脚选择位, 0x00:P1.6, 0x40:P2.6, 0x80:P6.6, 0xC0:P3.4
  PWMB_PS |= PWM7_3;
  PWMB_PS |= PWM8_3;
  PWMB_BKR = 0x80;   //使能主输出
  PWMB_CR1 |= 0x01;  //开始计时
  P_SW2 &= 0x7f;
}
#elif defined USEDRV8701
void MotorInit(u16 freq)
{

  P_SW2 |= 0x80;							//设置P_SW2寄存器的最高位，选择PWM输出引脚的
  PWMB_PSCRL = (MAIN_Fosc/PWMB_CLOCK)-1;   	//主频为35M,PWM分频到10M范围 MAIN_Fosc是主频，PWMB_CLOCK是PWM的工作频率 设置PWM的预分频器，通过除以预分频系数来调整PWM的频率。

  PWMB_CCER1 = 0x00; 						//写 CCMRx 前必须先清零 CCxE 关闭通道
  PWMB_CCER2 = 0x00;						//清零PWM控制寄存器1和2，关闭所有PWM输出通道。
	
  PWMB_CCMR1 = 0x64; 	//通道模式配置 PWM1模式
  PWMB_CCMR2 = 0x64;	//
  PWMB_CCMR3 = 0x60;	//
  PWMB_CCMR4 = 0x60;	//
	
  PWMB_CCER1 = 0x55; 	//配置通道输出使能和极性
  PWMB_CCER2 = 0x55;	//
  
  PWMB_ARRH  = (u8)((PWMB_CLOCK/freq)>>8);  //设置freq周期时间，从0计数到设定数值
  PWMB_ARRL  = (u8)(PWMB_CLOCK/freq); 		//  
  
  PWMB_CCR1H = 100;		//设置占空比时间
  PWMB_CCR1L = 200; 	//
	
  PWMB_CCR3H = 100;		//设置占空比时间
  PWMB_CCR3L = 200; 	//	
	
  PWMB_CR1 |= 0x80;		//自动预装载允许
  PWMB_CR1 &= ~0x70;	//边沿对齐  向上计数
  
  PWMB_ENO = 0x00;		//清空PWM使能输出寄存器
  PWMB_ENO |= ENO5P; 	//使能输出
  //PWMB_ENO |= ENO6P;	//使能输出 
  PWMB_ENO |= ENO7P; 	//使能输出
  //PWMB_ENO |= ENO8P;	//使能输出 
  
  PWMB_PS = 0x00;   	//高级 PWM 通道输出脚选择位  4P 输出脚选择位, 0x00:P1.6, 0x40:P2.6, 0x80:P6.6, 0xC0:P3.4
  PWMB_PS |= PWM5_3;	//选择 PWM5_3 通道
  //PWMB_PS |= PWM6_3;	//选择 PWM6_3 通道
  PWMB_PS |= PWM7_3;	//选择 PWM7_3 通道
  //PWMB_PS |= PWM8_3;	//选择 PWM8_3 通道
  
  PWMB_BKR = 0x80;			//使能主输出
  PWMB_CR1 |= 0x01;  		//开始计时
  
  P7M1 = 0;   				//P75,P77设置为推挽输出，强上拉，要加限流电阻
  P7M0 = 0xA0;				//
  P7PU |= 0xA0;				//使能P75和P77的上拉电阻
  
  P_SW2 &= 0x7f;			//关闭PWM输出引脚选择
}
#endif
//电机初始化


/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【函数名】void MotorCtrl(float motor1, float motor2)
【功  能】电机控制函数
【参数值】motor1   ： 电机1占空比
【参数值】motor2   ： 电机2占空比
【返回值】电机PWM 
【作  者】chiusir
【最后更新】2021年1月22日 
【软件版本】V1.0
【调用举例】 MotorCtrl (1000,-2000) //两个电机一正一反转动
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#ifdef  USE7843OR7971							//不看
void MotorCtrl (int16_t motor1,int16_t motor2)	//不看
{
  if (motor1 < 0)
  {	 
		motor1=0-motor1;
    P_SW2 |= 0x80;
    PWMB_CCR1H = 0; //设置占空比时间
    PWMB_CCR1L = 0;
    PWMB_CCR2H = (u8)(motor1 >> 8); //设置占空比时间
    PWMB_CCR2L = (u8)(motor1);
    P_SW2 &= 0x7f;
  }
  else
  {
    P_SW2 |= 0x80;
    PWMB_CCR1H = (u8)(motor1 >> 8); //设置占空比时间
    PWMB_CCR1L = (u8)(motor1);
    PWMB_CCR2H = 0; //设置占空比时间
    PWMB_CCR2L = 0;
    P_SW2 &= 0x7f;
  }  
  if (motor2 < 0)
  {	 
		motor2=0-motor2;
    P_SW2 |= 0x80;
    PWMB_CCR3H = 0; //设置占空比时间
    PWMB_CCR3L = 0;
    PWMB_CCR4H = (u8)(motor2 >> 8); //设置占空比时间
    PWMB_CCR4L = (u8)(motor2);
    P_SW2 &= 0x7f;
  }
  else
  {
    P_SW2 |= 0x80;
    PWMB_CCR3H = (u8)(motor2 >> 8); //设置占空比时间
    PWMB_CCR3L = (u8)(motor2);
    PWMB_CCR4H = 0; //设置占空比时间
    PWMB_CCR4L = 0;
    P_SW2 &= 0x7f;
  }  
}
#elif defined USEDRV8701
void MotorCtrl (int16_t motor1,int16_t motor2)
{
  if (motor1 < 0)
  {	 
	motor1=0-motor1;
    P_SW2 |= 0x80;
    PWMB_CCR1H = (u8)(motor1 >> 8); //设置占空比时间
    PWMB_CCR1L = (u8)(motor1);
    P75=1;
    
    P_SW2 &= 0x7f;
  }
  else
  {
    P_SW2 |= 0x80;
    PWMB_CCR1H = (u8)(motor1 >> 8); //设置占空比时间
    PWMB_CCR1L = (u8)(motor1);
    P75=0;
    P_SW2 &= 0x7f;
  }
  
  if (motor2 < 0)
  {	 
	motor2=0-motor2;
    P_SW2 |= 0x80;
    PWMB_CCR3H = (u8)(motor2 >> 8); //设置占空比时间
    PWMB_CCR3L = (u8)(motor2);
    P77=1;
    P_SW2 &= 0x7f;
  }
  else
  {
    P_SW2 |= 0x80;
    PWMB_CCR3H = (u8)(motor2 >> 8); //设置占空比时间
    PWMB_CCR3L = (u8)(motor2);
    P77=0;
    P_SW2 &= 0x7f;
  }  
}
#endif
//电机控制






























/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【函数名】void ServoInit(u16 freq)
【功  能】舵机PWM初始化
【参数值】u16 freq,期望频率，范围0--10000
【参数值】无
【返回值】无
【作  者】chiusir
【最后更新】2021年1月22日 
【软件版本】V1.0
【调用举例】驱动1个舵机 ServoInit(100);  //100HZ周期
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void ServoInit(u16 freq)
{
  P_SW2 |= 0x80;	
  //PWM1_PSCRL = 16; //分频范围  fcn_cnt=fck_psc/(PSCR[15:0]+1)
  PWMA_PSCRL = MAIN_Fosc/PWMA_CLOCK-1;      //主频为30M,PWM分频到1M范围  
  PWMA_CCER2 = 0x00; //写 CCMRx 前必须先清零 CCxE 关闭通道 清零PWM控制寄存器2，关闭所有PWM输出通道。
  PWMA_CCMR4 = 0x60; //设置 PWM4 模式1 输出
  PWMA_CCER2 = 0x55; //使能 CC4E 通道
  
  PWMA_ARRH  = (u8)((PWMA_CLOCK/freq)>>8);  //设置PWM的周期时间，从0计数到设定数值
  PWMA_ARRL  = (u8)(PWMA_CLOCK/freq);       //
  
  PWMA_CCR4H = (u8)((Servo_Center)>>8);     //舵机中值
  PWMA_CCR4L = (u8)(Servo_Center);          //设置PWM4通道的占空比，即舵机的中值位置
  
  PWMA_ENO |= 0x80; //使能 PWM4N 输出
  PWMA_PS  |= 0x00; //高级 PWM 通道 4N 输出脚选择位, 0x00:P1.7, 0x40:P2.7, 0x80:P6.7, 0xC0:P3.3
  
  PWMA_BKR = 0x80;  //使能主输出
  PWMA_CR1 |= 0x01; //开始计时
  P_SW2 &= 0x7f;		
}
//舵机初始化


/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【函数名】void ServoCtrl (uint16_t duty)
【功  能】舵机转角函数，由于小车拉杆范围限制，较小
【参数值】short duty，占空比，范围0--10000；
【参数值】int Target  期望脉冲数
【返回值】电机PWM 
【作  者】chiusir
【最后更新】2021年1月22日 
【软件版本】V1.0
【调用举例】ServoCtrl (1500);//舵机中值
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void ServoCtrl(uint16_t duty)
{  
	
  if (duty >= Servo_Left_Max)               //限制幅值
  duty = Servo_Left_Max;
    else if (duty < Servo_Right_Min)        //限制幅值
  duty = Servo_Right_Min;
  
  P_SW2 |= 0x80;
  PWMA_CCR4H = (u8)(duty >> 8); 			//设置占空比时间
  PWMA_CCR4L = (u8)(duty);
  P_SW2 &= 0x7f;
}
//舵机控制






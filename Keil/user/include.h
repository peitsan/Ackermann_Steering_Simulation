#ifndef __LQINCLUDES_H_
#define __LQINCLUDES_H_	  

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "stc32g128.h"
#include "intrins.h"

typedef 	unsigned char	u8;
typedef 	unsigned char	uint8;
typedef 	unsigned char	uint8_t;
typedef 	unsigned int	u16;
typedef 	unsigned int	uint16;
typedef 	unsigned int	uint16_t;
typedef 	unsigned long	u32;
typedef 	unsigned long	uint32;
typedef 	unsigned long	uint32_t;
typedef 	signed char	int8;
typedef 	signed char	int8_t;
typedef 	signed int	int16;
typedef 	signed int	int16_t;
typedef 	signed long	int32;
typedef 	signed long	int32_t;

#include <LQ_ADC.h>
#include "LQ_PWM.h"
#include <LQ_I2C.h>
#include <LQ_SPI.h>
#include "ANO_DT.h"
#include "LQ_GPIO.h"
#include <LQ_UART.h>
#include <LQ_TIMER.h>
#include <LQ_Encoder.h>
#include <LQ_SOFTI2C.h>
#include <LQ_OLED096.h>
#include <LQ_SPI_soft.h>
#include <LQ_GPIO_KEY.h>
#include "LQ_IIC_Gyro.h"
#include "LQ_SPI_Gyro.h"
#include "LQ_IIC_VL53.h"
#include "LQ_CCD.h"
#include <LQ_GPIO_LED.h>
#include <LQ_GPIO_EXIT.h>
#include <LQ_MotorServo.h>
#include "LQ_LSM6DSR.h"
#include "algorithm.h"
#include "balance.h"


#define EnableInterrupts (EA = 1)                 							//开总中断
#define DisableInterrupts (EA = 0)                							//关总中断
#define MAIN_Fosc 35000000UL

 
//extern u8 Way_Angle;                              							//获取角度的算法，1：四元数  2：卡尔曼  3：互补滤波 
//extern u8 Flag_Add,Flag_Dec,Flag_Left,Flag_Right; 							//蓝牙遥控相关的变量
//extern u8 Trace_BT;                               							//蓝牙遥控还是线阵CCD循迹 1：默认，蓝牙遥控；0：线阵CCD循迹
//extern u8 Flag_Stop,Flag_Show,Flag_Angle;         							//停止标志位和 显示标志位 默认停止 显示打开
//extern unsigned short data Encoder;            								//左右编码器的脉冲计数
//extern short data PWMMotor,PWMServo;              						  	//电机PWM变量  
//extern u16 Voltage;                              							//电池电压采样相关的变量                     
//extern float data Angle_Balance,Gyro_Balance,Gyro_Turn;						//平衡倾角 平衡陀螺仪 转向陀螺仪
//extern float Acceleration_Z;                      							//Z轴加速度计  
//extern float Balance_Kp,Balance_Kd,Balance_Ki,Velocity_Kp,Velocity_Ki;		//PID参数
//extern float Balance_Kcf;                        							 //向心力计算系数
//extern u8 Uart4_Receive;                          							//uart4接收数据
//extern short LeftRightFlag,ForwardBackFlag;
//extern float data Angle;
//extern float angle_dot;
//extern short data Velocity;                            					//显示速度 

#endif












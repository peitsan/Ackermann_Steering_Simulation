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


#define EnableInterrupts (EA = 1)                 							//�����ж�
#define DisableInterrupts (EA = 0)                							//�����ж�
#define MAIN_Fosc 35000000UL

 
//extern u8 Way_Angle;                              							//��ȡ�Ƕȵ��㷨��1����Ԫ��  2��������  3�������˲� 
//extern u8 Flag_Add,Flag_Dec,Flag_Left,Flag_Right; 							//����ң����صı���
//extern u8 Trace_BT;                               							//����ң�ػ�������CCDѭ�� 1��Ĭ�ϣ�����ң�أ�0������CCDѭ��
//extern u8 Flag_Stop,Flag_Show,Flag_Angle;         							//ֹͣ��־λ�� ��ʾ��־λ Ĭ��ֹͣ ��ʾ��
//extern unsigned short data Encoder;            								//���ұ��������������
//extern short data PWMMotor,PWMServo;              						  	//���PWM����  
//extern u16 Voltage;                              							//��ص�ѹ������صı���                     
//extern float data Angle_Balance,Gyro_Balance,Gyro_Turn;						//ƽ����� ƽ�������� ת��������
//extern float Acceleration_Z;                      							//Z����ٶȼ�  
//extern float Balance_Kp,Balance_Kd,Balance_Ki,Velocity_Kp,Velocity_Ki;		//PID����
//extern float Balance_Kcf;                        							 //����������ϵ��
//extern u8 Uart4_Receive;                          							//uart4��������
//extern short LeftRightFlag,ForwardBackFlag;
//extern float data Angle;
//extern float angle_dot;
//extern short data Velocity;                            					//��ʾ�ٶ� 

#endif












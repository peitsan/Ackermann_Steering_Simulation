#ifndef __LQ_MS_H_
#define __LQ_MS_H_

#include "include.h"

//电机频率
#define MOTOR_FREQUENCY    12500
#define PWMA_CLOCK         1000000	//1M
#define PWMB_CLOCK         35000000 //35M

#define Servo_Delta  140       	//舵机左右转动的差值，与舵机型号，拉杆和舵机臂长有关
#define Servo_Center 1530    	//舵机直行中值，前轮向左偏移要减小，向右偏移要加大，根据自己的车调整中值
#define Servo_Left_Max   (Servo_Center+Servo_Delta)      //舵机左转极限值
#define Servo_Right_Min  (Servo_Center-Servo_Delta)      //舵机右转极限值，此值跟舵机放置方式有关，立式

void MotorInit(u16 freq);
void MotorCtrl (int16_t motor1,int16_t motor2);
void ServoInit(u16 freq);
void ServoCtrl(uint16_t duty);
#endif
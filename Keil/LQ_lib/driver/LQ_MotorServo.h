#ifndef __LQ_MS_H_
#define __LQ_MS_H_

#include "include.h"

//���Ƶ��
#define MOTOR_FREQUENCY    12500
#define PWMA_CLOCK         1000000	//1M
#define PWMB_CLOCK         35000000 //35M

#define Servo_Delta  140       	//�������ת���Ĳ�ֵ�������ͺţ����˺Ͷ���۳��й�
#define Servo_Center 1530    	//���ֱ����ֵ��ǰ������ƫ��Ҫ��С������ƫ��Ҫ�Ӵ󣬸����Լ��ĳ�������ֵ
#define Servo_Left_Max   (Servo_Center+Servo_Delta)      //�����ת����ֵ
#define Servo_Right_Min  (Servo_Center-Servo_Delta)      //�����ת����ֵ����ֵ��������÷�ʽ�йأ���ʽ

void MotorInit(u16 freq);
void MotorCtrl (int16_t motor1,int16_t motor2);
void ServoInit(u16 freq);
void ServoCtrl(uint16_t duty);
#endif
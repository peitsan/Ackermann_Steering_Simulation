/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 ��ƽ    ̨�������������ܿƼ�STC32G12K128 LQFP64���İ�
 ����    д��chiusir
 ��E-mail  ��chiusir@163.com
 ������汾��V1.1 ��Ȩ���У���λʹ��������ϵ��Ȩ
 �������¡�2022��3��8��
 �������Ϣ�ο����е�ַ��
 ����    վ��http://www.lqist.cn
 ���Ա����̡�http://longqiu.taobao.com
 ------------------------------------------------
 ��IDE��STC32
 ��Target �� keil5.2������
 ��SYS PLL�� 35MHzʹ���ڲ�����ͨ��
=================================================================
stc-isp����ʱ, ѡ��ʱ�� 35MHZ
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#ifndef __CONTROL_H
#define __CONTROL_H
#include "include.h"


int SBB_Get_BalancePI(float angle,float gyro);
int SBB_Get_Servo_Turn(int servo_input);
int turn(int encoder_left,int encoder_right,float gyro);
void SBB_Set_Motor_Servo_Pwm(int moto1);
void Key(void);
void Xianfu_Pwm(void);
u8 Turn_Off(float angle, int volt);
void SBB_Get_Angle(u8 way);
int myabs(int a);
int Put_Down(float Angle,int encoder);
int SBB_Get_MotorPI (int Encoder,int Target);
void Demo_SBB_Car(void);
void OLED_Show(void);
#endif

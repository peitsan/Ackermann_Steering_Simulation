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

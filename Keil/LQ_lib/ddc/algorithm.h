#ifndef __ALGORITHNM_H
#define __ALGORITHNM_H

#define PI 3.14159265
extern float  angle, angle_dot; 	
extern float  ACCEL_Ang_Y;//�����м��ٶȼƵõ��Ƕ�ֵ
extern float  GYRO_Ang_Y;  //���ֽǶ�
extern float  ACC_GYRO_Ang_Y;//���ٶȼƺ������������ں�

void Kalman_Filter(float Accel,float Gyro);		
void Yijielvbo(float angle_m, float gyro_m);

void LQ_gyroY_accXZ(short a_x,short a_z,short g_y);

#endif

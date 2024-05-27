/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
��ƽ    ̨�������������ܿƼ���ƽ��Ħ�г�
����    д��chiusir
��E-mail  ��chiusir@163.com
������汾��V1.1 ��Ȩ���У���λʹ��������ϵ��Ȩ
�������¡�2020��4��10��
�������Ϣ�ο����е�ַ��
����    վ��http://www.lqist.cn
���Ա����̡�http://longqiu.taobao.com
------------------------------------------------
��dev.env.��Keil5.2�����ϰ汾
��Target �� STC16F40K
��SYS PLL�� 36MHz
------------------------------------------------
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#include "algorithm.h"
#include "math.h"

float data Angle;
float angle_dot; 	
float Q_angle=0.001;// ����������Э����
float Q_gyro=0.003; //0.003 ����������Э���� ����������Э����Ϊһ��һ�����о���
float R_angle=0.5;  // ����������Э���� �Ȳ���ƫ��
float dt=0.005;     //                 
char   C_0 = 1;
float Q_bias, Angle_err;
float PCt_0, PCt_1, E;
float K_0, K_1, t_0, t_1;
float Pdot[4] ={0,0,0,0};
float PP[2][2] = { { 1, 0 },{ 0, 1 } };

float ACCEL_Ang_Y=0;//�����м��ٶȼƵõ��Ƕ�ֵ
float GYRO_Ang_Y=0;  //���ֽǶ�
float ACC_GYRO_Ang_Y=0;//���ٶȼƺ������������ں�



/*************************************************************************
*  �������ƣ�void Kalman_Filter(float Accel,float Gyro)	
*  ����˵�������׿������˲�
*  ����˵�������ٶȡ����ٶ�
*  �������أ���
*  �޸�ʱ�䣺2020��4��20��
*  ��    ע��
*************************************************************************/
void Kalman_Filter(float Accel,float Gyro)		
{
  Angle+=(Gyro - Q_bias) * dt; //�������
  Pdot[0]=Q_angle - PP[0][1] - PP[1][0]; // Pk-����������Э�����΢��
  
  Pdot[1]=-PP[1][1];
  Pdot[2]=-PP[1][1];
  Pdot[3]=Q_gyro;
  PP[0][0] += Pdot[0] * dt;   // Pk-����������Э����΢�ֵĻ���
  PP[0][1] += Pdot[1] * dt;   // =����������Э����
  PP[1][0] += Pdot[2] * dt;
  PP[1][1] += Pdot[3] * dt;
  
  Angle_err = Accel - Angle;	//zk-�������
  
  PCt_0 = C_0 * PP[0][0];
  PCt_1 = C_0 * PP[1][0];
  
  E = R_angle + C_0 * PCt_0;
  
  K_0 = PCt_0 / E;
  K_1 = PCt_1 / E;
  
  t_0 = PCt_0;
  t_1 = C_0 * PP[0][1];
  
  PP[0][0] -= K_0 * t_0;		     //����������Э����
  PP[0][1] -= K_0 * t_1;
  PP[1][0] -= K_1 * t_0;
  PP[1][1] -= K_1 * t_1;
  
  Angle	  += K_0 * Angle_err;	   //�������
  Q_bias	+= K_1 * Angle_err;	   //�������
  angle_dot   = Gyro - Q_bias;	 //���ֵ(�������)��΢��=���ٶ�
}
/*************************************************************************
*  �������ƣ�
*  ����˵����һ�׻����˲�
*  ����˵�����Ƕ�/���ٶȡ����ٶ�
*  �������أ���
*  �޸�ʱ�䣺2020��4��20��
*  ��    ע��
*************************************************************************/
float K1 =0.01; 
void Yijielvbo(float angle_m, float gyro_m)
{
  Angle = K1 * angle_m + (1-K1) * (Angle + gyro_m / 16.4 * 0.005);
}

//***************���ٶȼƺ������������ں�*****************************//
void LQ_gyroY_accXZ(short a_x,short a_z,short g_y)      
{	
	float data tmv;
	float data zz;
	float data xx;
	zz=(float)a_z;
	xx=(float)a_x;
	
  ACCEL_Ang_Y=atan2(zz,xx)*180.0/PI;//�����м��ٶȼƵõ��Ƕ�ֵ
	
	tmv= (float)g_y*0.00032;  //���ֽǶ�
	GYRO_Ang_Y += tmv; //���ֽǶ�
	tmv=(ACCEL_Ang_Y-GYRO_Ang_Y)*0.025;//�ں�ϵ��  //0.025
  ACC_GYRO_Ang_Y+=tmv;//�ں�ϵ��  //0.025
}

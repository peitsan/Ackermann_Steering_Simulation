/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】北京龙邱智能科技自平衡摩托车
【编    写】chiusir
【E-mail  】chiusir@163.com
【软件版本】V1.1 版权所有，单位使用请先联系授权
【最后更新】2020年4月10日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://longqiu.taobao.com
------------------------------------------------
【dev.env.】Keil5.2及以上版本
【Target 】 STC16F40K
【SYS PLL】 36MHz
------------------------------------------------
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#include "algorithm.h"
#include "math.h"

float data Angle;
float angle_dot; 	
float Q_angle=0.001;// 过程噪声的协方差
float Q_gyro=0.003; //0.003 过程噪声的协方差 过程噪声的协方差为一个一行两列矩阵
float R_angle=0.5;  // 测量噪声的协方差 既测量偏差
float dt=0.005;     //                 
char   C_0 = 1;
float Q_bias, Angle_err;
float PCt_0, PCt_1, E;
float K_0, K_1, t_0, t_1;
float Pdot[4] ={0,0,0,0};
float PP[2][2] = { { 1, 0 },{ 0, 1 } };

float ACCEL_Ang_Y=0;//反正切加速度计得到角度值
float GYRO_Ang_Y=0;  //积分角度
float ACC_GYRO_Ang_Y=0;//加速度计和陀螺仪数据融合



/*************************************************************************
*  函数名称：void Kalman_Filter(float Accel,float Gyro)	
*  功能说明：简易卡尔曼滤波
*  参数说明：加速度、角速度
*  函数返回：无
*  修改时间：2020年4月20日
*  备    注：
*************************************************************************/
void Kalman_Filter(float Accel,float Gyro)		
{
  Angle+=(Gyro - Q_bias) * dt; //先验估计
  Pdot[0]=Q_angle - PP[0][1] - PP[1][0]; // Pk-先验估计误差协方差的微分
  
  Pdot[1]=-PP[1][1];
  Pdot[2]=-PP[1][1];
  Pdot[3]=Q_gyro;
  PP[0][0] += Pdot[0] * dt;   // Pk-先验估计误差协方差微分的积分
  PP[0][1] += Pdot[1] * dt;   // =先验估计误差协方差
  PP[1][0] += Pdot[2] * dt;
  PP[1][1] += Pdot[3] * dt;
  
  Angle_err = Accel - Angle;	//zk-先验估计
  
  PCt_0 = C_0 * PP[0][0];
  PCt_1 = C_0 * PP[1][0];
  
  E = R_angle + C_0 * PCt_0;
  
  K_0 = PCt_0 / E;
  K_1 = PCt_1 / E;
  
  t_0 = PCt_0;
  t_1 = C_0 * PP[0][1];
  
  PP[0][0] -= K_0 * t_0;		     //后验估计误差协方差
  PP[0][1] -= K_0 * t_1;
  PP[1][0] -= K_1 * t_0;
  PP[1][1] -= K_1 * t_1;
  
  Angle	  += K_0 * Angle_err;	   //后验估计
  Q_bias	+= K_1 * Angle_err;	   //后验估计
  angle_dot   = Gyro - Q_bias;	 //输出值(后验估计)的微分=角速度
}
/*************************************************************************
*  函数名称：
*  功能说明：一阶互补滤波
*  参数说明：角度/加速度、角速度
*  函数返回：无
*  修改时间：2020年4月20日
*  备    注：
*************************************************************************/
float K1 =0.01; 
void Yijielvbo(float angle_m, float gyro_m)
{
  Angle = K1 * angle_m + (1-K1) * (Angle + gyro_m / 16.4 * 0.005);
}

//***************加速度计和陀螺仪数据融合*****************************//
void LQ_gyroY_accXZ(short a_x,short a_z,short g_y)      
{	
	float data tmv;
	float data zz;
	float data xx;
	zz=(float)a_z;
	xx=(float)a_x;
	
  ACCEL_Ang_Y=atan2(zz,xx)*180.0/PI;//反正切加速度计得到角度值
	
	tmv= (float)g_y*0.00032;  //积分角度
	GYRO_Ang_Y += tmv; //积分角度
	tmv=(ACCEL_Ang_Y-GYRO_Ang_Y)*0.025;//融合系数  //0.025
  ACC_GYRO_Ang_Y+=tmv;//融合系数  //0.025
}

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
【dev.env.】c251v557及以上版本
【Target 】 STC16F40K
【SYS PLL】 30MHz
------------------------------------------------
调试过程：
1.开启主控板电源和驱动模块电源
2.手动将小车摆放到机械平衡位置
3.按下“走起”按键标定并启动小车
------------------------------------------------
遥控需要用到蓝牙模块及APP
L--左转 Left
R--右转 Right
A--加速 Accelerating
D--减速 Deceleration
P--停车 Parking
B--默认蓝牙遥控 Bluetooth
T--电磁循迹 Trace
BluetoothSPP--APP的结束符设置为“其它”
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#include "include.h"
#include <LQ_MotorServo.h>
#include "math.h"

float data Zhongzhi = 0.01;                   //当舵机向右偏，屏幕显示-5为平衡点，Zhongzhi为-5
u8  Flag_Target;
u8  Flag_Add,Flag_Dec,Flag_Left,Flag_Right;  	//蓝牙遥控相关的变量
u8  Trace_BT=1;                              	//蓝牙遥控还是线阵CCD循迹 1：默认，蓝牙遥控；0：电感循迹
u8  Flag_Stop=1,Flag_Show=0,Flag_Angle;      	//停止标志位和 显示标志位 默认停止 显示打开
short ForwardBackFlag=0,LeftRightFlag=0;
short Vbat;                            	     	//电池电压
float Acceleration_Z;                        	//Z轴加速度计  
float Balance_Kp = 29.5, Balance_Kd = 0.1, Balance_Ki = 0.001, Velocity_Kp = 0.8, Velocity_Ki = 0.88;//PID参数   
float Balance_Kcf = 0.05;                   	//向心力计算系数
float data Balance_Pwm,Servo_Pwm,Turn_Pwm,control_DJ=0;
float Pitch;

short data aacx,aacy,aacz;	        					//加速度传感器原始数据
short data gyrox,gyroy,gyroz;        					//陀螺仪原始数据 
float data Angle_Balance;
float data Gyro_Balance;
float data Gyro_Turn;  												//平衡倾角 平衡陀螺仪 转向陀螺仪
short data PWMMotor, PWMServo;               	//电机舵机PWM变量
unsigned short data Encoder;                 	//编码器的脉冲计数
int data Motor_Bias=1, Motor_Integration=100;	//电机控制PID
float data Servo_Bias, Servo_Last_Bias, Servo_Integration;	//舵机控制PID
float data Target_Servo;											//目标打角
short data Velocity = 60;                     //速度   

float Servo_And_Angle = 1.9;									//倾角与打角的关系
float Velocity_And_Angle = 0.16; 							//速度与倾角的关系系数
float F_Moca = 1;															//摩擦力除以质量

uint8_t Left_flag, Right_flag;
float Kalman_p[2];														// 不能删去，删去采值会错误。
/*************************************************************************
*  函数名称：void timer1_int (void) interrupt TIMER1_VECTOR
*  功能说明：timer1中断函数.5ms定时中断，严格保证采样和数据处理的时间同步,所有的控制代码都在这里面。
*  参数说明：无
*  函数返回：无
*  修改时间：2020年4月20日
*  备    注：严格保证采样和数据处理的时间同步,所有的控制代码都在这里面。
*************************************************************************/
void timer0_int(void) interrupt 1  //平衡车占用
{   
		float data ang;
	  float data tmv;
		float data Servo_PWM;		// 舵机PID
		int data Motor_PWM;			// 电机PI
		//10ms控制一次，为了保证M法测速的时间基准，首先读取编码器数据
  	
    ICM_Get_Raw_data(&aacx, &aacy, &aacz, &gyrox, &gyroy, &gyroz); 	    					//得到加速度传感器数据 
		Encoder = myabs(Read_Encoder(1));       
		Key();                                         //===扫描按键状态 单击双击可以改变小车运行状态	 
		
		ang = atan2(aacz, aacx) * 180.0 / PI;
		Yijielvbo(ang, gyroy);
		Angle_Balance = 90.0 - Angle;
	  //Gyro_Balance = Kalman_x[4][1];	
	
		// 判断左右倾，姿态
		if(Angle_Balance - Zhongzhi > 1)
		{
			Left_flag = 1;
			Right_flag = 0;
		} 
		else if(Angle_Balance - Zhongzhi < -1)
		{
			Left_flag = 0;
			Right_flag = 1;
		}
		else
		{ 
			Left_flag = 0;
			Right_flag = 0;
		}
		
		
		// 舵机位置式PID
		Target_Servo = Servo_And_Angle * (Angle_Balance - Zhongzhi);								// 设定打角与倾角的关系
		Servo_Bias = (Target_Servo - Servo_PWM);   																	// 目标打角与当前打角的偏差
		Servo_Integration += Servo_Bias;
		if(Servo_Integration < -900)      Servo_Integration = -900;	
		else if(Servo_Integration > 900)  Servo_Integration = 900;	 
		Servo_PWM = Balance_Kp * Servo_Bias + Balance_Ki * Servo_Integration + Balance_Kd * (Servo_Bias - Servo_Last_Bias);   //===计算平衡控制的电机PWM  PD控制   kp是P系数 kd是D系数 
		Servo_Last_Bias = Servo_Bias;						//更新上一次的偏差
		
    Balance_Pwm = Servo_PWM;         				//===平衡PID控制	
/*
		if(Angle_Balance - Zhongzhi > 0)
			Velocity = myabs(Angle_Balance - Zhongzhi)*3+25;
		else if(Angle_Balance - Zhongzhi < 0)
			Velocity = myabs(Angle_Balance - Zhongzhi)*2+25;
*/		
		if(Angle_Balance - Zhongzhi > 0)
			Velocity = Velocity_And_Angle * sqrt( (tan( +(Angle_Balance - Zhongzhi) * PI / 180.0 ) * 980 - F_Moca) * (-0.3 * Target_Servo + 122.33) )+30;
		else if(Angle_Balance - Zhongzhi < 0)
			Velocity = Velocity_And_Angle * sqrt( (tan( -(Angle_Balance - Zhongzhi) * PI / 180.0 ) * 980 - F_Moca) * (-0.3 * (-Target_Servo) + 122.33))+30;
		
    if(Velocity < 30)       Velocity = 30;	                          //===速度限幅最大
    else if(Velocity > 150)  Velocity = 150;	 												//===速度限幅最小
		
    PWMServo = Servo_Center + Balance_Pwm;	                      		//===舵机控制 		
    if(PWMServo < (Servo_Center - Servo_Delta)) PWMServo = Servo_Center - Servo_Delta;	              //===舵机角度限制
    else if(PWMServo > (Servo_Center + Servo_Delta))  PWMServo = Servo_Center + Servo_Delta;	 				//===舵机角度限制
    
    if(Turn_Off(Angle_Balance,Voltage) == 0)                          //===如果不存在异常
		{ 
			// 电机增量式PID
			Motor_Bias = Encoder - Velocity;             		 		//计算偏差
			Motor_PWM = 15 * Motor_Bias + 2 * Motor_Integration; //增量式PI控制器
			Motor_Integration += Motor_Bias;	                  //保存上一次偏差 
			if(Motor_PWM > 2000) 
				Motor_PWM = 2000;
			else if(Motor_PWM < -6000) 
				Motor_PWM = -6000;

  		PWMMotor = Motor_PWM - 70 ;                  				//===计算左轮电机最终PWM，死区70
      SBB_Set_Motor_Servo_Pwm(-PWMMotor);                 //===赋值给PWM寄存器 
		}		                                           
		Flag_Show++;
		Led_Flash(10);
		//LED_Ctrl(LED0, RVS);   //LED闪烁说明中断执行完毕
} 

/*************************************************************************
*  函数名称：
*  功能说明：
*  参数说明：无
*  函数返回：无
*  修改时间：2020年4月20日
*  备    注：
*************************************************************************/
void Demo_SBB_Car(void)
{ 
	OLED_CLS();   							       //清屏	
  MotorInit(MOTOR_FREQUENCY);        //=====初始化PWM 12.5KHZ，
	MotorCtrl(100,0);	
  ServoInit(10);   		               //=====初始化PWM100HZ驱动 舵机	
  ServoCtrl(Servo_Center);           //舵机中值	
  Timer34EncInit();                  //=====编码器接口	
  ADC_Init();                        //=====adc初始化  	
  ICM20689_Init();                   //=====ICM20689初始化
	//ICM42605_Init();	
  Timer0_init();                     // 200hz--->  5ms,平衡车用
	OLED_P6x8Str(0,7,"timer0 ok");		 //字符串显示	
	delayms(100);
  //GPIO_EXIT_Init();                //=====P3.7干簧管停车中断
	EnableInterrupts;
	OLED_CLS();   							       //清屏	
	OLED_Show();  
  while(1)
  {		
    //if(KEY_Read(KEY1)==0)   //使用SBB APP和OLED显示屏
		if(Flag_Show>20)  			
    {  
			Flag_Show=0;
      OLED_Show();            //===显示屏打开			
    }                         //===读取编码器的值
		//delayms(50);
	}	
}
/**************************************************************************
函数功能：OLED显示
入口参数：无
返回  值：无
**************************************************************************/
void OLED_Show(void)
{
	char txt[16];
	short Vbat=0;
  //=============第一行显示小车模式===================//	 
	OLED_P6x8Str(19,0,(u8*)"LQ e-motorcycle");		
  //=============第2行显示小车模式====================//	 
	sprintf(txt,"%0.2f  %03d  0x%02X",Zhongzhi,ForwardBackFlag,Uart4_Receive);
	OLED_P6x8Str(20,1,(u8*)txt);			
  //=============第三行===============================//	
  if(Flag_Stop) sprintf(txt,"Enc:%04d    Stop ",Encoder);
	else  sprintf(txt,"Enc:%04d    Run ",Encoder);
  OLED_P6x8Str(0,2,(u8*)txt);	
  //=============第四行===============================//
	sprintf(txt,"Ser:%04d mot:%04d ",PWMServo, PWMMotor);
	OLED_P6x8Str(0,3,(u8*)txt);
  //=============第五行显示电压=======================//	
	Vbat=ADC_Read(6)*4/5;  //10K和1K电阻分压 Voltage*3.3/4095*100*11	
	sprintf(txt,"%02d.%02dV  %03d SP:%04d ",Vbat/100,Vbat%100,(short)Zhongzhi,(short)Servo_Pwm);
	OLED_P6x8Str(0,4,(u8*)txt);
  //=============第六行显示角度=======================//
	sprintf(txt,"Ang:%03d.%d   BP:%04d ",(short)Angle_Balance,myabs((short)(Angle_Balance*10))%10,(short)Balance_Pwm);
	OLED_P6x8Str(0,5,(u8*)txt);
	//=============第7行显示读取电感值==================//
	//Show_TSL1401(1); 
	sprintf(txt,"x:%04d y:%04d z:%04d ",aacx, aacy, aacz);
  OLED_P6x8Str(0,6,(u8*)txt);
	//=============第8行显示读取电感值==================//
	sprintf(txt,"L:%04d R:%04d V:%03d ",left,right,Velocity);
	OLED_P6x8Str(0,7,(u8*)txt);
}
/*************************************************************************
*  函数名称：int SBB_Get_BalancePI(float angle,float Gyro)
*  功能说明：平衡环PI控制
*  参数说明：角度、角速度
*  函数返回：直立控制PWM
*  修改时间：2020年4月20日
*  备    注：
*************************************************************************/
int SBB_Get_BalancePI(float angle,float Gyro)
{  
  float Bias;
  static float Integration;
  int SBB_Get_BalancePID;
	
  Bias = angle - Zhongzhi;   //===求出平衡的角度中值和机械相关
  Integration += Bias;

	if(Integration<-900)      Integration = -900;	
  else if(Integration>900)  Integration = 900;	 

  SBB_Get_BalancePID = Balance_Kp * Bias + Integration * Balance_Ki + Gyro * Balance_Kd;   //===计算平衡控制的电机PWM  PD控制   kp是P系数 kd是D系数 
	return SBB_Get_BalancePID;
}
/*************************************************************************
*  函数名称：int SBB_Get_Servo_Turn(int servo_input)
*  功能说明：舵机角度控制，自平衡转向环
*  参数说明：舵机角度
*  函数返回：舵机控制PWM
*  修改时间：2020年4月20日
*  备    注：此处尚需加入转圈的向心力环节
*************************************************************************/
int SBB_Get_Servo_Turn(int servo_input)
{  
	
  static float speed,Encoder_Least,Encoder;
  static float Encoder_Integral;

	InductorNormal();
  //=============遥控转向或电感循迹=======================// 
	if(Trace_BT)// 蓝牙遥控
	{
	  if(Flag_Right)    	LeftRightFlag=-120;	       //===向右转 
		else if(Flag_Left)	LeftRightFlag= 120;        //===向左转
		else  LeftRightFlag=0;
	}/*
	else // 电感循迹
	{		
	if(LnowADC[0]<5&&LnowADC[1]<5)   Flag_Stop=1;	
	else
		{
			if(deviation<25&&deviation>-25)
			{
				LeftRightFlag=(left-right)*3/10;	
			  control_DJ = (right-left)/10;
			}
			if((deviation>25&&deviation<45)||(deviation<-25&&deviation>-45))
			{
				LeftRightFlag=(left-right)*3/10;	 
				control_DJ = (right-left)/10;
			}
			if(deviation>45||deviation<-45)
			{
				LeftRightFlag=(left-right)*6/5;	 
				control_DJ = (right-left)/2;
			}
	 }
 }
 */
  //=============速度PI控制器=======================//	
	Encoder_Least =servo_input-0;                    									//===获取最新偏差
  Encoder *= 0.8;		                                                //===一阶低通滤波器       
  Encoder += Encoder_Least*0.2;	                                    //===一阶低通滤波器    
  Encoder_Integral +=Encoder;                                       //===积分出位移 积分时间：10ms
  Encoder_Integral=Encoder_Integral-LeftRightFlag;                  //===接收遥控器数据，控制前进后退
  if(Encoder_Integral>4000)  	Encoder_Integral=4000;             		//===积分限幅
  else if(Encoder_Integral<-4000)	Encoder_Integral=-4000;           //===积分限幅	
  speed=Encoder*Velocity_Kp+Encoder_Integral*Velocity_Ki/100;    //===速度控制	
  if(Flag_Stop==1) Encoder_Integral=0;
  return speed;//有问题，平时这个数值不变，电机没法驱动的	
}
/*************************************************************************
*  函数名称：int SBB_Get_MotorPI (int Encoder,int Target)
*  功能说明：电机速度环增量式PI控制
*  参数说明：编码器值，目标速度
*  函数返回：电机PWM
*  修改时间：2020年4月20日
*  备    注：
*************************************************************************/
int SBB_Get_MotorPI (int Encoder,int Target)
{ 	
  static int Bias, Pwm, Last_bias;

  Bias = Encoder - Target;              //计算偏差
  Pwm += 18 * (Last_bias - Bias) + 22 * Bias;   //增量式PI控制器
	
  if(Pwm > 5200) Pwm = 5200;
  else if(Pwm < -5200) Pwm = -5200;
  Last_bias = Bias;	                    //保存上一次偏差 
  return Pwm;                         //增量输出
}
/*************************************************************************
*  函数名称：void SBB_Set_Motor_Servo_Pwm(int moto1)
*  功能说明：赋值给PWM寄存器，舵机和驱动电机
*  参数说明：PWM 电机范围：【-800，800】
*  函数返回：无
*  修改时间：2020年4月20日
*  备    注：
*************************************************************************/
void SBB_Set_Motor_Servo_Pwm(int moto1)
{
  //short siqu=1000;

   MotorCtrl (moto1,0); //用龙邱DRV8701或者BTN7971驱动板
   ServoCtrl (PWMServo);//舵机控制PWM 舵机范围：【0，10000】
}

/*************************************************************************
*  函数名称：void Key(void)
*  功能说明：按键修改小车运行状态 
*  参数说明：无
*  函数返回：无
*  修改时间：2020年4月20日
*  备    注：
*************************************************************************/
void Key(void)
{	
  if(KEY_Read(KEY3)==0)
	{
		Flag_Stop=!Flag_Stop;   //单击K3(启停键)控制小车的启停		
	}  
	else if(KEY_Read_All()==KEY02DOWN)
	{
		Zhongzhi=Angle_Balance,Flag_Angle=0;//同时按下K0 K2键标定
	}	
}
/*************************************************************************
*  函数名称：u8 Turn_Off(float angle, int voltage)
*  功能说明：异常关闭电机
*  参数说明：倾斜角度，电压
*  函数返回：1：异常  0：正常
*  修改时间：2020年4月20日
*  备    注：
*************************************************************************/
u8 Turn_Off(float angle, int volt)
{
  u8 temp;
  //if(angle<-40||angle>40||1==Flag_Stop||volt<710||Roll<-20)//电池电压低于7.1V关闭电机
	if(angle<-35 || angle>35|| 1==Flag_Stop)
  {	                                                 //===倾角大于40度关闭电机
    temp=1;                                          //===Flag_Stop置1关闭电机
    MotorCtrl(10,0); //用龙邱DRV8701或者BTN7971驱动板
    ServoCtrl(Servo_Center);//舵机控制PWM 舵机范围：【0，10000】
    Flag_Stop=1;	
		ForwardBackFlag=0,LeftRightFlag=0;
  }
  else
    temp=0;
  return temp;			
}
/*************************************************************************
*  函数名称：void SBB_Get_Angle(u8 way)
*  功能说明：获取角度 三种算法经过我们的调校，都非常理想 
*  参数说明：获取角度的算法 1：DMP  2：卡尔曼 3：互补滤波
*  函数返回：无
*  修改时间：2020年4月20日
*  备    注：
*************************************************************************/
void SBB_Get_Angle(u8 way)
{ 
	short data gyro_x,gyro_y,gyro_z, accel_x,accel_y,accel_z;
	
	ICM_Get_Raw_data(&accel_x,&accel_y,&accel_z,&gyro_x,&gyro_y,&gyro_z);	//得到加速度传感器数据  
	
  if(way==1)                         //===DMP的读取在数据采集中断提醒的时候，严格遵循时序要求
  {	
    //Read_DMP();                    //===读取加速度、角速度、倾角
    Angle_Balance= (0-Pitch);        //===更新平衡倾角
    Gyro_Balance = (0-gyro_y);      //===更新平衡角速度
    Gyro_Turn=gyro_z;               //===更新转向角速度
    Acceleration_Z=accel_z;         //===更新Z轴加速度计
  }	
	else if(way==2) 	
	{
		Kalman_Filter((float)accel_z,(float)(0-gyro_y)); //卡尔曼
	}
	else if(way==3) 	
	{
		Yijielvbo((float)accel_z,(float)(0-gyro_y));     //互补滤波
	}
	else if(way==4) 
	{
		LQ_gyroY_accXZ((short)accel_x,(short)accel_z,(short)gyro_y);
		Angle_Balance= 90.0-ACCEL_Ang_Y;        //===更新平衡倾角
    Gyro_Balance = (0-gyro_y);      //===更新平衡角速度
    Gyro_Turn=gyro_z;               //===更新转向角速度
    Acceleration_Z=accel_z;         //===更新Z轴加速度计		
	}	
}

/*************************************************************************
*  函数名称：int myabs(int a)
*  功能说明：绝对值函数
*  参数说明：int
*  函数返回：unsigned int
*  修改时间：2020年4月20日
*  备    注：
*************************************************************************/
int myabs(int a)
{   
  if(a<0) return(-a);  
  else    return a;
}
/*************************************************************************
*  函数名称：int Put_Down(float angle,int encoder)
*  功能说明：检测小车是否被放下或者摔倒
*  参数说明：倾斜角度，编码器值
*  函数返回：int
*  修改时间：2020年4月20日
*  备    注：
*************************************************************************/
int Put_Down(float angle,int encoder)
{ 		   
  static u16 flag,count;	 	                 
  if(flag==0)                                               
  {
    if(angle>(-40+Zhongzhi)&&angle<(40+Zhongzhi)&&encoder<2&&encoder>-2&&Flag_Stop==1)         //条件1，小车是在0度附近的
      flag=1; 
  } 
  else if(flag==1)                                               
  {
    if(++count>50)                //超时不再等待 500ms
    {
      count=0;flag=0;
    }
    if(encoder<-8)                //条件2，小车的轮胎在未上电的时候被人为转动  
    {      
      flag=0;
      return 1;                   //检测到小车被放下
    }
  }
  return 0;
}

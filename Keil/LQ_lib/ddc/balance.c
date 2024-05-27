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
��dev.env.��c251v557�����ϰ汾
��Target �� STC16F40K
��SYS PLL�� 30MHz
------------------------------------------------
���Թ��̣�
1.�������ذ��Դ������ģ���Դ
2.�ֶ���С���ڷŵ���еƽ��λ��
3.���¡����𡱰����궨������С��
------------------------------------------------
ң����Ҫ�õ�����ģ�鼰APP
L--��ת Left
R--��ת Right
A--���� Accelerating
D--���� Deceleration
P--ͣ�� Parking
B--Ĭ������ң�� Bluetooth
T--���ѭ�� Trace
BluetoothSPP--APP�Ľ���������Ϊ��������
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#include "include.h"
#include <LQ_MotorServo.h>
#include "math.h"

float data Zhongzhi = 0.01;                   //���������ƫ����Ļ��ʾ-5Ϊƽ��㣬ZhongzhiΪ-5
u8  Flag_Target;
u8  Flag_Add,Flag_Dec,Flag_Left,Flag_Right;  	//����ң����صı���
u8  Trace_BT=1;                              	//����ң�ػ�������CCDѭ�� 1��Ĭ�ϣ�����ң�أ�0�����ѭ��
u8  Flag_Stop=1,Flag_Show=0,Flag_Angle;      	//ֹͣ��־λ�� ��ʾ��־λ Ĭ��ֹͣ ��ʾ��
short ForwardBackFlag=0,LeftRightFlag=0;
short Vbat;                            	     	//��ص�ѹ
float Acceleration_Z;                        	//Z����ٶȼ�  
float Balance_Kp = 29.5, Balance_Kd = 0.1, Balance_Ki = 0.001, Velocity_Kp = 0.8, Velocity_Ki = 0.88;//PID����   
float Balance_Kcf = 0.05;                   	//����������ϵ��
float data Balance_Pwm,Servo_Pwm,Turn_Pwm,control_DJ=0;
float Pitch;

short data aacx,aacy,aacz;	        					//���ٶȴ�����ԭʼ����
short data gyrox,gyroy,gyroz;        					//������ԭʼ���� 
float data Angle_Balance;
float data Gyro_Balance;
float data Gyro_Turn;  												//ƽ����� ƽ�������� ת��������
short data PWMMotor, PWMServo;               	//������PWM����
unsigned short data Encoder;                 	//���������������
int data Motor_Bias=1, Motor_Integration=100;	//�������PID
float data Servo_Bias, Servo_Last_Bias, Servo_Integration;	//�������PID
float data Target_Servo;											//Ŀ����
short data Velocity = 60;                     //�ٶ�   

float Servo_And_Angle = 1.9;									//������ǵĹ�ϵ
float Velocity_And_Angle = 0.16; 							//�ٶ�����ǵĹ�ϵϵ��
float F_Moca = 1;															//Ħ������������

uint8_t Left_flag, Right_flag;
float Kalman_p[2];														// ����ɾȥ��ɾȥ��ֵ�����
/*************************************************************************
*  �������ƣ�void timer1_int (void) interrupt TIMER1_VECTOR
*  ����˵����timer1�жϺ���.5ms��ʱ�жϣ��ϸ�֤���������ݴ����ʱ��ͬ��,���еĿ��ƴ��붼�������档
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2020��4��20��
*  ��    ע���ϸ�֤���������ݴ����ʱ��ͬ��,���еĿ��ƴ��붼�������档
*************************************************************************/
void timer0_int(void) interrupt 1  //ƽ�⳵ռ��
{   
		float data ang;
	  float data tmv;
		float data Servo_PWM;		// ���PID
		int data Motor_PWM;			// ���PI
		//10ms����һ�Σ�Ϊ�˱�֤M�����ٵ�ʱ���׼�����ȶ�ȡ����������
  	
    ICM_Get_Raw_data(&aacx, &aacy, &aacz, &gyrox, &gyroy, &gyroz); 	    					//�õ����ٶȴ��������� 
		Encoder = myabs(Read_Encoder(1));       
		Key();                                         //===ɨ�谴��״̬ ����˫�����Ըı�С������״̬	 
		
		ang = atan2(aacz, aacx) * 180.0 / PI;
		Yijielvbo(ang, gyroy);
		Angle_Balance = 90.0 - Angle;
	  //Gyro_Balance = Kalman_x[4][1];	
	
		// �ж������㣬��̬
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
		
		
		// ���λ��ʽPID
		Target_Servo = Servo_And_Angle * (Angle_Balance - Zhongzhi);								// �趨�������ǵĹ�ϵ
		Servo_Bias = (Target_Servo - Servo_PWM);   																	// Ŀ�����뵱ǰ��ǵ�ƫ��
		Servo_Integration += Servo_Bias;
		if(Servo_Integration < -900)      Servo_Integration = -900;	
		else if(Servo_Integration > 900)  Servo_Integration = 900;	 
		Servo_PWM = Balance_Kp * Servo_Bias + Balance_Ki * Servo_Integration + Balance_Kd * (Servo_Bias - Servo_Last_Bias);   //===����ƽ����Ƶĵ��PWM  PD����   kp��Pϵ�� kd��Dϵ�� 
		Servo_Last_Bias = Servo_Bias;						//������һ�ε�ƫ��
		
    Balance_Pwm = Servo_PWM;         				//===ƽ��PID����	
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
		
    if(Velocity < 30)       Velocity = 30;	                          //===�ٶ��޷����
    else if(Velocity > 150)  Velocity = 150;	 												//===�ٶ��޷���С
		
    PWMServo = Servo_Center + Balance_Pwm;	                      		//===������� 		
    if(PWMServo < (Servo_Center - Servo_Delta)) PWMServo = Servo_Center - Servo_Delta;	              //===����Ƕ�����
    else if(PWMServo > (Servo_Center + Servo_Delta))  PWMServo = Servo_Center + Servo_Delta;	 				//===����Ƕ�����
    
    if(Turn_Off(Angle_Balance,Voltage) == 0)                          //===����������쳣
		{ 
			// �������ʽPID
			Motor_Bias = Encoder - Velocity;             		 		//����ƫ��
			Motor_PWM = 15 * Motor_Bias + 2 * Motor_Integration; //����ʽPI������
			Motor_Integration += Motor_Bias;	                  //������һ��ƫ�� 
			if(Motor_PWM > 2000) 
				Motor_PWM = 2000;
			else if(Motor_PWM < -6000) 
				Motor_PWM = -6000;

  		PWMMotor = Motor_PWM - 70 ;                  				//===�������ֵ������PWM������70
      SBB_Set_Motor_Servo_Pwm(-PWMMotor);                 //===��ֵ��PWM�Ĵ��� 
		}		                                           
		Flag_Show++;
		Led_Flash(10);
		//LED_Ctrl(LED0, RVS);   //LED��˸˵���ж�ִ�����
} 

/*************************************************************************
*  �������ƣ�
*  ����˵����
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2020��4��20��
*  ��    ע��
*************************************************************************/
void Demo_SBB_Car(void)
{ 
	OLED_CLS();   							       //����	
  MotorInit(MOTOR_FREQUENCY);        //=====��ʼ��PWM 12.5KHZ��
	MotorCtrl(100,0);	
  ServoInit(10);   		               //=====��ʼ��PWM100HZ���� ���	
  ServoCtrl(Servo_Center);           //�����ֵ	
  Timer34EncInit();                  //=====�������ӿ�	
  ADC_Init();                        //=====adc��ʼ��  	
  ICM20689_Init();                   //=====ICM20689��ʼ��
	//ICM42605_Init();	
  Timer0_init();                     // 200hz--->  5ms,ƽ�⳵��
	OLED_P6x8Str(0,7,"timer0 ok");		 //�ַ�����ʾ	
	delayms(100);
  //GPIO_EXIT_Init();                //=====P3.7�ɻɹ�ͣ���ж�
	EnableInterrupts;
	OLED_CLS();   							       //����	
	OLED_Show();  
  while(1)
  {		
    //if(KEY_Read(KEY1)==0)   //ʹ��SBB APP��OLED��ʾ��
		if(Flag_Show>20)  			
    {  
			Flag_Show=0;
      OLED_Show();            //===��ʾ����			
    }                         //===��ȡ��������ֵ
		//delayms(50);
	}	
}
/**************************************************************************
�������ܣ�OLED��ʾ
��ڲ�������
����  ֵ����
**************************************************************************/
void OLED_Show(void)
{
	char txt[16];
	short Vbat=0;
  //=============��һ����ʾС��ģʽ===================//	 
	OLED_P6x8Str(19,0,(u8*)"LQ e-motorcycle");		
  //=============��2����ʾС��ģʽ====================//	 
	sprintf(txt,"%0.2f  %03d  0x%02X",Zhongzhi,ForwardBackFlag,Uart4_Receive);
	OLED_P6x8Str(20,1,(u8*)txt);			
  //=============������===============================//	
  if(Flag_Stop) sprintf(txt,"Enc:%04d    Stop ",Encoder);
	else  sprintf(txt,"Enc:%04d    Run ",Encoder);
  OLED_P6x8Str(0,2,(u8*)txt);	
  //=============������===============================//
	sprintf(txt,"Ser:%04d mot:%04d ",PWMServo, PWMMotor);
	OLED_P6x8Str(0,3,(u8*)txt);
  //=============��������ʾ��ѹ=======================//	
	Vbat=ADC_Read(6)*4/5;  //10K��1K�����ѹ Voltage*3.3/4095*100*11	
	sprintf(txt,"%02d.%02dV  %03d SP:%04d ",Vbat/100,Vbat%100,(short)Zhongzhi,(short)Servo_Pwm);
	OLED_P6x8Str(0,4,(u8*)txt);
  //=============��������ʾ�Ƕ�=======================//
	sprintf(txt,"Ang:%03d.%d   BP:%04d ",(short)Angle_Balance,myabs((short)(Angle_Balance*10))%10,(short)Balance_Pwm);
	OLED_P6x8Str(0,5,(u8*)txt);
	//=============��7����ʾ��ȡ���ֵ==================//
	//Show_TSL1401(1); 
	sprintf(txt,"x:%04d y:%04d z:%04d ",aacx, aacy, aacz);
  OLED_P6x8Str(0,6,(u8*)txt);
	//=============��8����ʾ��ȡ���ֵ==================//
	sprintf(txt,"L:%04d R:%04d V:%03d ",left,right,Velocity);
	OLED_P6x8Str(0,7,(u8*)txt);
}
/*************************************************************************
*  �������ƣ�int SBB_Get_BalancePI(float angle,float Gyro)
*  ����˵����ƽ�⻷PI����
*  ����˵�����Ƕȡ����ٶ�
*  �������أ�ֱ������PWM
*  �޸�ʱ�䣺2020��4��20��
*  ��    ע��
*************************************************************************/
int SBB_Get_BalancePI(float angle,float Gyro)
{  
  float Bias;
  static float Integration;
  int SBB_Get_BalancePID;
	
  Bias = angle - Zhongzhi;   //===���ƽ��ĽǶ���ֵ�ͻ�е���
  Integration += Bias;

	if(Integration<-900)      Integration = -900;	
  else if(Integration>900)  Integration = 900;	 

  SBB_Get_BalancePID = Balance_Kp * Bias + Integration * Balance_Ki + Gyro * Balance_Kd;   //===����ƽ����Ƶĵ��PWM  PD����   kp��Pϵ�� kd��Dϵ�� 
	return SBB_Get_BalancePID;
}
/*************************************************************************
*  �������ƣ�int SBB_Get_Servo_Turn(int servo_input)
*  ����˵��������Ƕȿ��ƣ���ƽ��ת��
*  ����˵��������Ƕ�
*  �������أ��������PWM
*  �޸�ʱ�䣺2020��4��20��
*  ��    ע���˴��������תȦ������������
*************************************************************************/
int SBB_Get_Servo_Turn(int servo_input)
{  
	
  static float speed,Encoder_Least,Encoder;
  static float Encoder_Integral;

	InductorNormal();
  //=============ң��ת�����ѭ��=======================// 
	if(Trace_BT)// ����ң��
	{
	  if(Flag_Right)    	LeftRightFlag=-120;	       //===����ת 
		else if(Flag_Left)	LeftRightFlag= 120;        //===����ת
		else  LeftRightFlag=0;
	}/*
	else // ���ѭ��
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
  //=============�ٶ�PI������=======================//	
	Encoder_Least =servo_input-0;                    									//===��ȡ����ƫ��
  Encoder *= 0.8;		                                                //===һ�׵�ͨ�˲���       
  Encoder += Encoder_Least*0.2;	                                    //===һ�׵�ͨ�˲���    
  Encoder_Integral +=Encoder;                                       //===���ֳ�λ�� ����ʱ�䣺10ms
  Encoder_Integral=Encoder_Integral-LeftRightFlag;                  //===����ң�������ݣ�����ǰ������
  if(Encoder_Integral>4000)  	Encoder_Integral=4000;             		//===�����޷�
  else if(Encoder_Integral<-4000)	Encoder_Integral=-4000;           //===�����޷�	
  speed=Encoder*Velocity_Kp+Encoder_Integral*Velocity_Ki/100;    //===�ٶȿ���	
  if(Flag_Stop==1) Encoder_Integral=0;
  return speed;//�����⣬ƽʱ�����ֵ���䣬���û��������	
}
/*************************************************************************
*  �������ƣ�int SBB_Get_MotorPI (int Encoder,int Target)
*  ����˵��������ٶȻ�����ʽPI����
*  ����˵����������ֵ��Ŀ���ٶ�
*  �������أ����PWM
*  �޸�ʱ�䣺2020��4��20��
*  ��    ע��
*************************************************************************/
int SBB_Get_MotorPI (int Encoder,int Target)
{ 	
  static int Bias, Pwm, Last_bias;

  Bias = Encoder - Target;              //����ƫ��
  Pwm += 18 * (Last_bias - Bias) + 22 * Bias;   //����ʽPI������
	
  if(Pwm > 5200) Pwm = 5200;
  else if(Pwm < -5200) Pwm = -5200;
  Last_bias = Bias;	                    //������һ��ƫ�� 
  return Pwm;                         //�������
}
/*************************************************************************
*  �������ƣ�void SBB_Set_Motor_Servo_Pwm(int moto1)
*  ����˵������ֵ��PWM�Ĵ�����������������
*  ����˵����PWM �����Χ����-800��800��
*  �������أ���
*  �޸�ʱ�䣺2020��4��20��
*  ��    ע��
*************************************************************************/
void SBB_Set_Motor_Servo_Pwm(int moto1)
{
  //short siqu=1000;

   MotorCtrl (moto1,0); //������DRV8701����BTN7971������
   ServoCtrl (PWMServo);//�������PWM �����Χ����0��10000��
}

/*************************************************************************
*  �������ƣ�void Key(void)
*  ����˵���������޸�С������״̬ 
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2020��4��20��
*  ��    ע��
*************************************************************************/
void Key(void)
{	
  if(KEY_Read(KEY3)==0)
	{
		Flag_Stop=!Flag_Stop;   //����K3(��ͣ��)����С������ͣ		
	}  
	else if(KEY_Read_All()==KEY02DOWN)
	{
		Zhongzhi=Angle_Balance,Flag_Angle=0;//ͬʱ����K0 K2���궨
	}	
}
/*************************************************************************
*  �������ƣ�u8 Turn_Off(float angle, int voltage)
*  ����˵�����쳣�رյ��
*  ����˵������б�Ƕȣ���ѹ
*  �������أ�1���쳣  0������
*  �޸�ʱ�䣺2020��4��20��
*  ��    ע��
*************************************************************************/
u8 Turn_Off(float angle, int volt)
{
  u8 temp;
  //if(angle<-40||angle>40||1==Flag_Stop||volt<710||Roll<-20)//��ص�ѹ����7.1V�رյ��
	if(angle<-35 || angle>35|| 1==Flag_Stop)
  {	                                                 //===��Ǵ���40�ȹرյ��
    temp=1;                                          //===Flag_Stop��1�رյ��
    MotorCtrl(10,0); //������DRV8701����BTN7971������
    ServoCtrl(Servo_Center);//�������PWM �����Χ����0��10000��
    Flag_Stop=1;	
		ForwardBackFlag=0,LeftRightFlag=0;
  }
  else
    temp=0;
  return temp;			
}
/*************************************************************************
*  �������ƣ�void SBB_Get_Angle(u8 way)
*  ����˵������ȡ�Ƕ� �����㷨�������ǵĵ�У�����ǳ����� 
*  ����˵������ȡ�Ƕȵ��㷨 1��DMP  2�������� 3�������˲�
*  �������أ���
*  �޸�ʱ�䣺2020��4��20��
*  ��    ע��
*************************************************************************/
void SBB_Get_Angle(u8 way)
{ 
	short data gyro_x,gyro_y,gyro_z, accel_x,accel_y,accel_z;
	
	ICM_Get_Raw_data(&accel_x,&accel_y,&accel_z,&gyro_x,&gyro_y,&gyro_z);	//�õ����ٶȴ���������  
	
  if(way==1)                         //===DMP�Ķ�ȡ�����ݲɼ��ж����ѵ�ʱ���ϸ���ѭʱ��Ҫ��
  {	
    //Read_DMP();                    //===��ȡ���ٶȡ����ٶȡ����
    Angle_Balance= (0-Pitch);        //===����ƽ�����
    Gyro_Balance = (0-gyro_y);      //===����ƽ����ٶ�
    Gyro_Turn=gyro_z;               //===����ת����ٶ�
    Acceleration_Z=accel_z;         //===����Z����ٶȼ�
  }	
	else if(way==2) 	
	{
		Kalman_Filter((float)accel_z,(float)(0-gyro_y)); //������
	}
	else if(way==3) 	
	{
		Yijielvbo((float)accel_z,(float)(0-gyro_y));     //�����˲�
	}
	else if(way==4) 
	{
		LQ_gyroY_accXZ((short)accel_x,(short)accel_z,(short)gyro_y);
		Angle_Balance= 90.0-ACCEL_Ang_Y;        //===����ƽ�����
    Gyro_Balance = (0-gyro_y);      //===����ƽ����ٶ�
    Gyro_Turn=gyro_z;               //===����ת����ٶ�
    Acceleration_Z=accel_z;         //===����Z����ٶȼ�		
	}	
}

/*************************************************************************
*  �������ƣ�int myabs(int a)
*  ����˵��������ֵ����
*  ����˵����int
*  �������أ�unsigned int
*  �޸�ʱ�䣺2020��4��20��
*  ��    ע��
*************************************************************************/
int myabs(int a)
{   
  if(a<0) return(-a);  
  else    return a;
}
/*************************************************************************
*  �������ƣ�int Put_Down(float angle,int encoder)
*  ����˵�������С���Ƿ񱻷��»���ˤ��
*  ����˵������б�Ƕȣ�������ֵ
*  �������أ�int
*  �޸�ʱ�䣺2020��4��20��
*  ��    ע��
*************************************************************************/
int Put_Down(float angle,int encoder)
{ 		   
  static u16 flag,count;	 	                 
  if(flag==0)                                               
  {
    if(angle>(-40+Zhongzhi)&&angle<(40+Zhongzhi)&&encoder<2&&encoder>-2&&Flag_Stop==1)         //����1��С������0�ȸ�����
      flag=1; 
  } 
  else if(flag==1)                                               
  {
    if(++count>50)                //��ʱ���ٵȴ� 500ms
    {
      count=0;flag=0;
    }
    if(encoder<-8)                //����2��С������̥��δ�ϵ��ʱ����Ϊת��  
    {      
      flag=0;
      return 1;                   //��⵽С��������
    }
  }
  return 0;
}

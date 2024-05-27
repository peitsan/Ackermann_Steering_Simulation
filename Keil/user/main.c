#include "include.h"

/*****************************************************************************************************************/
/*****************************************************************************************************************/
/*****************************************************************************************************************/
//������sysinit()                                                                                       ��ʼ������
//���룺��
//�������
//˵�������ຯ����ʼ���������
/*****************************************************************************************************************/
void sysinit(void)  
{	
	WTST = 0; 				//��Ƭ����FLASHȡָ������Ҫ�ĵȴ�ʱ��  0 С��32MHZ  1 �ɴ�64MHZ
	P_SW2 |= 0x80;			//��չ RAM �� ���⹦�ܼĴ�����XFR�� ����XFR 
	CLKDIV = 0;				//��Ƶ����  ��XFR��1ʱ���ܷ���
	P_SW2 &= 0x7F;		
	RSTCFG|=1<<4;           //P5.4��λ�ܽ�
	P0M1 =0x40; P0M0 = 0;   //����Ϊ׼˫���,P0.6  0.7Ϊ����ڣ���������
	P0PU =0x40;             //P0.6  0.7ʹ���ڲ�4.1K��������
	P1M1 = 0;   P1M0 = 0;   //����Ϊ׼˫���
	P3M1 = 0;   P3M0 = 0;   //����Ϊ׼˫���
	P4M1 = 0;   P4M0 = 0;   //����Ϊ׼˫���
	P5M1 = 0;   P5M0 = 0;   //����Ϊ׼˫���
	P6M1 = 0;   P6M0 = 0;   //����Ϊ׼˫���
	P7M1 = 0;   P7M0 = 0;   //����Ϊ׼˫��� 
	P7PU =0xFF;	            //����Ϊ����,P7.012Ϊ����ڽӰ���,P7.4567Ϊ����ڽӵ������
	P40 = 0;				//��LED��Դ 
	EA=1;					//�����ж�
	
	//��ʼ���������
	GPIO_LED_Init();		//LED��ʼ�� ����IO��ģʽ
	GPIO_KEY_Init();		//������ʼ�� ����IO��ģʽ
	OLED_Init();            //OLED��ʼ��
	ServoInit(100);			//ת���ŷ������ʼ��
	MotorInit(100);			//���ֵ����ʼ��
	Timer34EncInit ();		//��������ʼ��
	MPU6050_Init();			//�����ǳ�ʼ��
	Timer2_init();
	
	//********
}
//

typedef struct
{
    float                kp = 6;         //P
    float                ki = 0;         //I δ��
    float                kd = 1.4;         //D
    float                imax;       //�����޷�
 
    float                out_p;  //KP���
    float                out_i;  //KI���
    float                out_d;  //KD���
    float                out;    //pid���
	float				 last_out;//pid��һ�����
	float				 out_diff;//pid��ǰ�������һ������Ĳ�ֵ
 
    float                integrator; //< ����ֵ
    float                last_error; //< �ϴ����
    float                last_derivative;//< �ϴ���������ϴ����֮��
 
    float                target;     //<  ���õ�����ֵ
	
}pid_param_t;

//λ��ʽ
float ServoPidLocCtrl(pid_param_t * pid,float error)
{	
    pid->out_p = pid->kp * error;
    pid->out_d = pid->kd * (error - pid->last_error);
 
    pid->last_error = error;
 
    pid->out = pid->out_p + pid->out_d;
 
    return pid->out;
}

/*****************************************************************************************************************/
/*****************************************************************************************************************/
/*****************************************************************************************************************/
//������main()                                                                                             ������
//���룺��
//�������
//˵����
/*****************************************************************************************************************/
void main()
{
	int Servo_duty;
	sysinit();					//���г�ʼ��
	pid_param_t Servo;			//�ŷ����pid��ʼ��
	Servo. target = 1650;		//���Ŀ������
	MotorCtrl (1800,1800);   	//������ƣ��ȿ�ĸ���ٿ������� ��Ϊδ��ʼ�����Ͷ�����1����2  ������ͬ��
		
    //����
    float Left_motor_kp = 1;//kp
    float Left_motor_ki = 2;//ki
    float Left_motor_kd = 0;//kd
    float Pid_Motor_Left_imax = 1000;//����������ֵ
    float Left_pluse= 100;//����ʵ���ٶȣ��ñ���Ϊ�����������ֵ��������ֱ�����õ�100�����ʹ��ʱ��һ��Ҫ���ñ�������Ϊ������ʵ�������ֵ
    float left_speed_loop_out = 0;//�����ٶȻ��ٶ����
   
    //����
    float Right_motor_kp = 1;
    float Right_motor_ki = 2;
    float Right_motor_kd = 0;
    float Pid_Motor_Right_imax = 1000;//����������ֵ
    float Right_pluse= 100;//����ʵ���ٶȣ��ñ���Ϊ�����������ֵ��������ֱ�����õ�100�����ʹ��ʱ��һ��Ҫ���ñ�������Ϊ������ʵ�������ֵ
    float right_speed_loop_out = 0;//�����ٶȻ��ٶ����

    //(1)����������������pid�ṹ��
    pid_param_t Pid_Left_Motor;//����PID�ṹ��
    pid_param_t Pid_Right_Motor;//�ҵ��PID�ṹ��
    
    //(2)��ʼ��pid�ṹ��
    //����pid����
	Pid_Init(&Pid_Left_Motor);//PID��ʼ��
	PidSet(&Pid_Left_Motor, Left_motor_kp, Left_motor_ki, Left_motor_kd, Pid_Motor_Left_imax);//pid��������
	pidSetTarget(&Pid_Left_Motor,0);//pid��������
 
    //�ҵ��pid����
	Pid_Init(&Pid_Right_Motor);//PID��ʼ��
	PidSet(&Pid_Right_Motor, Right_motor_kp, Right_motor_ki, Right_motor_kd, Pid_Motor_Right_imax);//pid��������
	pidSetTarget(&Pid_Right_Motor,0);//pid��������

	
	while(1)
	{
		Encoder();				//��������ֵ
		ServoPidLocCtrl(Servo,Servo.last_error);
		Servo_duty = Servo_Center+(int)Servo.out;
		ServoCtrl (Servo_duty);			//�������
		
		//(3.1) ����ʽPID
        left_speed_loop_out  += PidIncCtrl(&Pid_Left_Motor,Left_pluse);
        right_speed_loop_out  += PidIncCtrl(&Pid_Right_Motor,Right_pluse);
		MotorCtrl (left_speed_loop_out,right_speed_loop_out);//�������
	}
}




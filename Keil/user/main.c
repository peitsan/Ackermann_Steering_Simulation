#include "include.h"

/*****************************************************************************************************************/
/*****************************************************************************************************************/
/*****************************************************************************************************************/
//函数：sysinit()                                                                                       初始化函数
//输入：无
//输出：无
//说明：各类函数初始化在这添加
/*****************************************************************************************************************/
void sysinit(void)  
{	
	WTST = 0; 				//单片机到FLASH取指令所需要的等待时间  0 小于32MHZ  1 可达64MHZ
	P_SW2 |= 0x80;			//扩展 RAM 区 特殊功能寄存器（XFR） 访问XFR 
	CLKDIV = 0;				//分频设置  当XFR置1时才能访问
	P_SW2 &= 0x7F;		
	RSTCFG|=1<<4;           //P5.4复位管脚
	P0M1 =0x40; P0M0 = 0;   //设置为准双向口,P0.6  0.7为输入口，编码器用
	P0PU =0x40;             //P0.6  0.7使能内部4.1K上拉电阻
	P1M1 = 0;   P1M0 = 0;   //设置为准双向口
	P3M1 = 0;   P3M0 = 0;   //设置为准双向口
	P4M1 = 0;   P4M0 = 0;   //设置为准双向口
	P5M1 = 0;   P5M0 = 0;   //设置为准双向口
	P6M1 = 0;   P6M0 = 0;   //设置为准双向口
	P7M1 = 0;   P7M0 = 0;   //设置为准双向口 
	P7PU =0xFF;	            //设置为上拉,P7.012为输入口接按键,P7.4567为输出口接电机驱动
	P40 = 0;				//打开LED电源 
	EA=1;					//打开总中断
	
	//初始化函数添加
	GPIO_LED_Init();		//LED初始化 设置IO口模式
	GPIO_KEY_Init();		//按键初始化 设置IO口模式
	OLED_Init();            //OLED初始化
	ServoInit(100);			//转向伺服电机初始化
	MotorInit(100);			//后轮电机初始化
	Timer34EncInit ();		//编码器初始化
	MPU6050_Init();			//陀螺仪初始化
	Timer2_init();
	
	//********
}
//

typedef struct
{
    float                kp = 6;         //P
    float                ki = 0;         //I 未用
    float                kd = 1.4;         //D
    float                imax;       //积分限幅
 
    float                out_p;  //KP输出
    float                out_i;  //KI输出
    float                out_d;  //KD输出
    float                out;    //pid输出
	float				 last_out;//pid上一次输出
	float				 out_diff;//pid当前输出与上一次输出的差值
 
    float                integrator; //< 积分值
    float                last_error; //< 上次误差
    float                last_derivative;//< 上次误差与上上次误差之差
 
    float                target;     //<  设置的期望值
	
}pid_param_t;

//位置式
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
//函数：main()                                                                                             主函数
//输入：无
//输出：无
//说明：
/*****************************************************************************************************************/
void main()
{
	int Servo_duty;
	sysinit();					//所有初始化
	pid_param_t Servo;			//伺服电机pid初始化
	Servo. target = 1650;		//舵机目标设置
	MotorCtrl (1800,1800);   	//电机控制（先开母版再开驱动板 因为未初始化会猛动）左1，右2  编码器同理
		
    //左轮
    float Left_motor_kp = 1;//kp
    float Left_motor_ki = 2;//ki
    float Left_motor_kd = 0;//kd
    float Pid_Motor_Left_imax = 1000;//误差积分项阈值
    float Left_pluse= 100;//左轮实际速度，该变量为编码器输出数值，这里我直接设置的100，大家使用时，一定要将该变量设置为编码器实际输出数值
    float left_speed_loop_out = 0;//左轮速度环速度输出
   
    //右轮
    float Right_motor_kp = 1;
    float Right_motor_ki = 2;
    float Right_motor_kd = 0;
    float Pid_Motor_Right_imax = 1000;//误差积分项阈值
    float Right_pluse= 100;//右轮实际速度，该变量为编码器输出数值，这里我直接设置的100，大家使用时，一定要将该变量设置为编码器实际输出数值
    float right_speed_loop_out = 0;//右轮速度环速度输出

    //(1)定义左右两个车轮pid结构体
    pid_param_t Pid_Left_Motor;//左电机PID结构体
    pid_param_t Pid_Right_Motor;//右电机PID结构体
    
    //(2)初始化pid结构体
    //左电机pid设置
	Pid_Init(&Pid_Left_Motor);//PID初始化
	PidSet(&Pid_Left_Motor, Left_motor_kp, Left_motor_ki, Left_motor_kd, Pid_Motor_Left_imax);//pid参数设置
	pidSetTarget(&Pid_Left_Motor,0);//pid参数设置
 
    //右电机pid设置
	Pid_Init(&Pid_Right_Motor);//PID初始化
	PidSet(&Pid_Right_Motor, Right_motor_kp, Right_motor_ki, Right_motor_kd, Pid_Motor_Right_imax);//pid参数设置
	pidSetTarget(&Pid_Right_Motor,0);//pid参数设置

	
	while(1)
	{
		Encoder();				//编码器读值
		ServoPidLocCtrl(Servo,Servo.last_error);
		Servo_duty = Servo_Center+(int)Servo.out;
		ServoCtrl (Servo_duty);			//舵机控制
		
		//(3.1) 增量式PID
        left_speed_loop_out  += PidIncCtrl(&Pid_Left_Motor,Left_pluse);
        right_speed_loop_out  += PidIncCtrl(&Pid_Right_Motor,Right_pluse);
		MotorCtrl (left_speed_loop_out,right_speed_loop_out);//电机控制
	}
}




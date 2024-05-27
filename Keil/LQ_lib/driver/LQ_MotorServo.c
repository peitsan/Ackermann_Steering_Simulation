#include "include.h"

#define USEDRV8701     //ʹ�ò�ͬ�����������岻ͬ�ĺ궨��
//#define USE7843OR7971//ʹ�ò�ͬ�����������岻ͬ�ĺ궨��

#define PWM1_0      0x00	//P:P1.0  N:P1.1
#define PWM1_1      0x01	//P:P2.0  N:P2.1 
#define PWM1_2      0x02	//P:P6.0  N:P6.1

#define PWM2_0      0x00	//P:P1.2/P5.4  N:P1.3
#define PWM2_1      0x04	//P:P2.2  N:P2.3
#define PWM2_2      0x08	//P:P6.2  N:P6.3

#define PWM3_0      0x00	//P:P1.4  N:P1.5
#define PWM3_1      0x10	//P:P2.4  N:P2.5
#define PWM3_2      0x20	//P:P6.4  N:P6.5

#define PWM4_0      0x00	//P:P1.6  N:P1.7
#define PWM4_1      0x40	//P:P2.6  N:P2.7
#define PWM4_2      0x80	//P:P6.6  N:P6.7
#define PWM4_3      0xC0	//P:P3.4  N:P3.3

#define PWM5_0      0x00	//P2.0
#define PWM5_1      0x01	//P1.7
#define PWM5_2      0x02	//P0.0
#define PWM5_3      0x03	//P7.4

#define PWM6_0      0x00	//P2.1
#define PWM6_1      0x04	//P5.4
#define PWM6_2      0x08	//P0.1
#define PWM6_3      0x0C	//P7.5

#define PWM7_0      0x00	//P2.2
#define PWM7_1      0x10	//P3.3
#define PWM7_2      0x20	//P0.2
#define PWM7_3      0x30	//P7.6

#define PWM8_0      0x00	//P2.3
#define PWM8_1      0x40	//P3.4
#define PWM8_2      0x80	//P0.3
#define PWM8_3      0xC0	//P7.7

#define ENO5P       0x01
#define ENO6P       0x04
#define ENO7P       0x10
#define ENO8P       0x40
#define ENO1P       0x01
#define ENO1N       0x02
#define ENO2P       0x04
#define ENO2N       0x08
#define ENO3P       0x10
#define ENO3N       0x20
#define ENO4P       0x40
#define ENO4N       0x80


u16 PWM1_Duty;
u16 PWM2_Duty;
u16 PWM3_Duty;
u16 PWM4_Duty;

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
����������void MotorInit(u16 freq)
����  �ܡ����PWM��ʼ��
������ֵ��int Encoder ���������ɼ��ĳ���
������ֵ��int Target  ����������
������ֵ�����PWM 
����  �ߡ�chiusir
�������¡�2021��1��22�� 
������汾��V1.0
�����þ������������  TIM4_PWM_Init(1000-1);  //1KHZ����
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#ifdef USE7843OR7971  		//����
void MotorInit(u16 freq)  	//����
{
	P_SW2 |= 0x80;
	PWMB_PS = 0xff;		//�л�PWM����Ϊ 7.4 7.5 7.6 7.7
	PWMB_PSCRL = (MAIN_Fosc/PWMB_CLOCK)-1;   	//��ƵΪ22M,PWM��Ƶ��10M��Χ 
	PWMB_CCER1 = 0x00;												//д CCMRx ǰ���������� CCxE �ر�ͨ��
  PWMB_CCER2 = 0x00;
  PWMB_CCMR1 = 0x60; 												//ͨ��ģʽ����
  PWMB_CCMR2 = 0x60;
  PWMB_CCMR3 = 0x60;
  PWMB_CCMR4 = 0x60;
  PWMB_CCER1 = 0x33; 												//����ͨ�����ʹ�ܺͼ���
  PWMB_CCER2 = 0x33;
  
  PWMB_ARRH  = (u8)((PWMB_CLOCK/freq)>>8);  //��������ʱ�䣬��0�������趨��ֵ
  PWMB_ARRL  = (u8)(PWMB_CLOCK/freq); //  
  
  PWMB_CCR1H = 0;
  PWMB_CCR1L = 20; 	//����ռ�ձ�ʱ��
  PWMB_CCR2H = 0;
  PWMB_CCR2L = 20; 	//����ռ�ձ�ʱ��	
  PWMB_CCR3H = 0;
  PWMB_CCR3L = 20; 	//����ռ�ձ�ʱ��
  PWMB_CCR4H = 0;	
  PWMB_CCR4L = 20; 	//����ռ�ձ�ʱ��
  
  PWMB_ENO = 0x00;
  PWMB_ENO |= ENO5P; //ʹ�����
  PWMB_ENO |= ENO6P; //ʹ����� 
  PWMB_ENO |= ENO7P; //ʹ�����
  PWMB_ENO |= ENO8P; //ʹ����� 
  PWMB_PS = 0x00;    //�߼� PWM ͨ�������ѡ��λ
  PWMB_PS |= PWM5_3; //ѡ�� PWM5_3 ͨ��
  PWMB_PS |= PWM6_3; //ѡ�� PWM6_3 ͨ�� //�߼� PWM ͨ�� 4P �����ѡ��λ, 0x00:P1.6, 0x40:P2.6, 0x80:P6.6, 0xC0:P3.4
  PWMB_PS |= PWM7_3;
  PWMB_PS |= PWM8_3;
  PWMB_BKR = 0x80;   //ʹ�������
  PWMB_CR1 |= 0x01;  //��ʼ��ʱ
  P_SW2 &= 0x7f;
}
#elif defined USEDRV8701
void MotorInit(u16 freq)
{

  P_SW2 |= 0x80;							//����P_SW2�Ĵ��������λ��ѡ��PWM������ŵ�
  PWMB_PSCRL = (MAIN_Fosc/PWMB_CLOCK)-1;   	//��ƵΪ35M,PWM��Ƶ��10M��Χ MAIN_Fosc����Ƶ��PWMB_CLOCK��PWM�Ĺ���Ƶ�� ����PWM��Ԥ��Ƶ����ͨ������Ԥ��Ƶϵ��������PWM��Ƶ�ʡ�

  PWMB_CCER1 = 0x00; 						//д CCMRx ǰ���������� CCxE �ر�ͨ��
  PWMB_CCER2 = 0x00;						//����PWM���ƼĴ���1��2���ر�����PWM���ͨ����
	
  PWMB_CCMR1 = 0x64; 	//ͨ��ģʽ���� PWM1ģʽ
  PWMB_CCMR2 = 0x64;	//
  PWMB_CCMR3 = 0x60;	//
  PWMB_CCMR4 = 0x60;	//
	
  PWMB_CCER1 = 0x55; 	//����ͨ�����ʹ�ܺͼ���
  PWMB_CCER2 = 0x55;	//
  
  PWMB_ARRH  = (u8)((PWMB_CLOCK/freq)>>8);  //����freq����ʱ�䣬��0�������趨��ֵ
  PWMB_ARRL  = (u8)(PWMB_CLOCK/freq); 		//  
  
  PWMB_CCR1H = 100;		//����ռ�ձ�ʱ��
  PWMB_CCR1L = 200; 	//
	
  PWMB_CCR3H = 100;		//����ռ�ձ�ʱ��
  PWMB_CCR3L = 200; 	//	
	
  PWMB_CR1 |= 0x80;		//�Զ�Ԥװ������
  PWMB_CR1 &= ~0x70;	//���ض���  ���ϼ���
  
  PWMB_ENO = 0x00;		//���PWMʹ������Ĵ���
  PWMB_ENO |= ENO5P; 	//ʹ�����
  //PWMB_ENO |= ENO6P;	//ʹ����� 
  PWMB_ENO |= ENO7P; 	//ʹ�����
  //PWMB_ENO |= ENO8P;	//ʹ����� 
  
  PWMB_PS = 0x00;   	//�߼� PWM ͨ�������ѡ��λ  4P �����ѡ��λ, 0x00:P1.6, 0x40:P2.6, 0x80:P6.6, 0xC0:P3.4
  PWMB_PS |= PWM5_3;	//ѡ�� PWM5_3 ͨ��
  //PWMB_PS |= PWM6_3;	//ѡ�� PWM6_3 ͨ��
  PWMB_PS |= PWM7_3;	//ѡ�� PWM7_3 ͨ��
  //PWMB_PS |= PWM8_3;	//ѡ�� PWM8_3 ͨ��
  
  PWMB_BKR = 0x80;			//ʹ�������
  PWMB_CR1 |= 0x01;  		//��ʼ��ʱ
  
  P7M1 = 0;   				//P75,P77����Ϊ���������ǿ������Ҫ����������
  P7M0 = 0xA0;				//
  P7PU |= 0xA0;				//ʹ��P75��P77����������
  
  P_SW2 &= 0x7f;			//�ر�PWM�������ѡ��
}
#endif
//�����ʼ��


/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
����������void MotorCtrl(float motor1, float motor2)
����  �ܡ�������ƺ���
������ֵ��motor1   �� ���1ռ�ձ�
������ֵ��motor2   �� ���2ռ�ձ�
������ֵ�����PWM 
����  �ߡ�chiusir
�������¡�2021��1��22�� 
������汾��V1.0
�����þ����� MotorCtrl (1000,-2000) //�������һ��һ��ת��
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#ifdef  USE7843OR7971							//����
void MotorCtrl (int16_t motor1,int16_t motor2)	//����
{
  if (motor1 < 0)
  {	 
		motor1=0-motor1;
    P_SW2 |= 0x80;
    PWMB_CCR1H = 0; //����ռ�ձ�ʱ��
    PWMB_CCR1L = 0;
    PWMB_CCR2H = (u8)(motor1 >> 8); //����ռ�ձ�ʱ��
    PWMB_CCR2L = (u8)(motor1);
    P_SW2 &= 0x7f;
  }
  else
  {
    P_SW2 |= 0x80;
    PWMB_CCR1H = (u8)(motor1 >> 8); //����ռ�ձ�ʱ��
    PWMB_CCR1L = (u8)(motor1);
    PWMB_CCR2H = 0; //����ռ�ձ�ʱ��
    PWMB_CCR2L = 0;
    P_SW2 &= 0x7f;
  }  
  if (motor2 < 0)
  {	 
		motor2=0-motor2;
    P_SW2 |= 0x80;
    PWMB_CCR3H = 0; //����ռ�ձ�ʱ��
    PWMB_CCR3L = 0;
    PWMB_CCR4H = (u8)(motor2 >> 8); //����ռ�ձ�ʱ��
    PWMB_CCR4L = (u8)(motor2);
    P_SW2 &= 0x7f;
  }
  else
  {
    P_SW2 |= 0x80;
    PWMB_CCR3H = (u8)(motor2 >> 8); //����ռ�ձ�ʱ��
    PWMB_CCR3L = (u8)(motor2);
    PWMB_CCR4H = 0; //����ռ�ձ�ʱ��
    PWMB_CCR4L = 0;
    P_SW2 &= 0x7f;
  }  
}
#elif defined USEDRV8701
void MotorCtrl (int16_t motor1,int16_t motor2)
{
  if (motor1 < 0)
  {	 
	motor1=0-motor1;
    P_SW2 |= 0x80;
    PWMB_CCR1H = (u8)(motor1 >> 8); //����ռ�ձ�ʱ��
    PWMB_CCR1L = (u8)(motor1);
    P75=1;
    
    P_SW2 &= 0x7f;
  }
  else
  {
    P_SW2 |= 0x80;
    PWMB_CCR1H = (u8)(motor1 >> 8); //����ռ�ձ�ʱ��
    PWMB_CCR1L = (u8)(motor1);
    P75=0;
    P_SW2 &= 0x7f;
  }
  
  if (motor2 < 0)
  {	 
	motor2=0-motor2;
    P_SW2 |= 0x80;
    PWMB_CCR3H = (u8)(motor2 >> 8); //����ռ�ձ�ʱ��
    PWMB_CCR3L = (u8)(motor2);
    P77=1;
    P_SW2 &= 0x7f;
  }
  else
  {
    P_SW2 |= 0x80;
    PWMB_CCR3H = (u8)(motor2 >> 8); //����ռ�ձ�ʱ��
    PWMB_CCR3L = (u8)(motor2);
    P77=0;
    P_SW2 &= 0x7f;
  }  
}
#endif
//�������






























/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
����������void ServoInit(u16 freq)
����  �ܡ����PWM��ʼ��
������ֵ��u16 freq,����Ƶ�ʣ���Χ0--10000
������ֵ����
������ֵ����
����  �ߡ�chiusir
�������¡�2021��1��22�� 
������汾��V1.0
�����þ���������1����� ServoInit(100);  //100HZ����
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void ServoInit(u16 freq)
{
  P_SW2 |= 0x80;	
  //PWM1_PSCRL = 16; //��Ƶ��Χ  fcn_cnt=fck_psc/(PSCR[15:0]+1)
  PWMA_PSCRL = MAIN_Fosc/PWMA_CLOCK-1;      //��ƵΪ30M,PWM��Ƶ��1M��Χ  
  PWMA_CCER2 = 0x00; //д CCMRx ǰ���������� CCxE �ر�ͨ�� ����PWM���ƼĴ���2���ر�����PWM���ͨ����
  PWMA_CCMR4 = 0x60; //���� PWM4 ģʽ1 ���
  PWMA_CCER2 = 0x55; //ʹ�� CC4E ͨ��
  
  PWMA_ARRH  = (u8)((PWMA_CLOCK/freq)>>8);  //����PWM������ʱ�䣬��0�������趨��ֵ
  PWMA_ARRL  = (u8)(PWMA_CLOCK/freq);       //
  
  PWMA_CCR4H = (u8)((Servo_Center)>>8);     //�����ֵ
  PWMA_CCR4L = (u8)(Servo_Center);          //����PWM4ͨ����ռ�ձȣ����������ֵλ��
  
  PWMA_ENO |= 0x80; //ʹ�� PWM4N ���
  PWMA_PS  |= 0x00; //�߼� PWM ͨ�� 4N �����ѡ��λ, 0x00:P1.7, 0x40:P2.7, 0x80:P6.7, 0xC0:P3.3
  
  PWMA_BKR = 0x80;  //ʹ�������
  PWMA_CR1 |= 0x01; //��ʼ��ʱ
  P_SW2 &= 0x7f;		
}
//�����ʼ��


/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
����������void ServoCtrl (uint16_t duty)
����  �ܡ����ת�Ǻ���������С�����˷�Χ���ƣ���С
������ֵ��short duty��ռ�ձȣ���Χ0--10000��
������ֵ��int Target  ����������
������ֵ�����PWM 
����  �ߡ�chiusir
�������¡�2021��1��22�� 
������汾��V1.0
�����þ�����ServoCtrl (1500);//�����ֵ
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void ServoCtrl(uint16_t duty)
{  
	
  if (duty >= Servo_Left_Max)               //���Ʒ�ֵ
  duty = Servo_Left_Max;
    else if (duty < Servo_Right_Min)        //���Ʒ�ֵ
  duty = Servo_Right_Min;
  
  P_SW2 |= 0x80;
  PWMA_CCR4H = (u8)(duty >> 8); 			//����ռ�ձ�ʱ��
  PWMA_CCR4L = (u8)(duty);
  P_SW2 &= 0x7f;
}
//�������






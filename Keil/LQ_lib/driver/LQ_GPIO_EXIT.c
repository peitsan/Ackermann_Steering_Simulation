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

#include "include.h"

#define EX2 0x10
#define EX3 0x20
#define EX4 0x40

u8  INT0_cnt, INT1_cnt; 					//�����õļ�������
u8  INT2_cnt, INT3_cnt, INT4_cnt; //�����õļ�������

/********************* INT0�жϺ��� *************************/
void INT0_int (void) interrupt 0  //���ж�ʱ�Ѿ������־
{
    INT0_cnt++;										//�ж�+1
}

/********************* INT1�жϺ��� *************************/
void INT1_int (void) interrupt 2      //���ж�ʱ�Ѿ������־
{
    INT1_cnt++; //�ж�+1
}

/********************* INT2�жϺ��� *************************/
void INT2_int (void) interrupt 10 //���ж�ʱ�Ѿ������־
{//IMU��
    INT2_cnt++; 									//�ж�+1
	
}

/********************* INT3�жϺ��� *************************/
void INT3_int (void) interrupt 11 //���ж�ʱ�Ѿ������־
{//�ɻɹ���
  INT3_cnt++; 									//�ж�+1
	
	//LED_Ctrl(LED0, RVS);   				//LED��˸;
}

/********************* INT4�жϺ��� *************************/
void INT4_int (void) interrupt 16 //���ж�ʱ�Ѿ������־
{
    INT4_cnt++; 									//�ж�+1
}
/*************************************************************************
*  �������ƣ�void LED_Init(void)
*  ����˵����GPIO��ʼ������ LED������PB10��PA3��ʼ��
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2020��3��10��
*  ��    ע��
*************************************************************************/
void GPIO_EXIT_Init(void)
{
	EA = 0;					//�ر����ж�
	
	IT0 = 0; 				//ʹ�� INT0 �����غ��½����ж�
	// IT0 = 1; 		//ʹ�� INT0 �½����ж�
	EX0 = 1; 				//ʹ�� INT0 �ж�
	
	IT1 = 0; 				//ʹ�� INT1 �����غ��½����ж�
	// IT1 = 1; 		//ʹ�� INT1 �½����ж�
	EX1 = 1; 				//ʹ�� INT1 �ж�
	
	INTCLKO = EX2; 	//ʹ�� INT2 �½����ж�
	
	INTCLKO |= EX3; //ʹ�� INT3 �½����ж�
	
	INTCLKO |= EX4; //ʹ�� INT4 �½����ж�
	
	EA = 1;
}


/*************************************************************************
*  �������ƣ�void Test_GPIO_EXIT_Init(void)
*  ����˵�������Գ���
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2020��3��10��
*  ��    ע�����İ��ϵ�LED����˸
*************************************************************************/
void Test_GPIO_EXIT_Init(void)
{
	char txt[32];
	GPIO_LED_Init();				
	GPIO_EXIT_Init();
 	while(1)
 	{
 		LED_Ctrl(LED0,RVS);		     //LED��˸
		sprintf(txt, "INT3:%d \n",INT3_cnt);
		UART4_PutStr(txt);
 		delayms(200);                //��ʱ�ȴ�
 	}
 }

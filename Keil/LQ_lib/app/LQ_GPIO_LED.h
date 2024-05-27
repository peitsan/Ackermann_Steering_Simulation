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
#ifndef __LQ_LED_H_
#define __LQ_LED_H_

#include "include.h"

//����ģ���
typedef enum
{
  LED0=0,  //���İ���LED0	P42����
  LED1=1,  //���İ���LED1	P62�̵�
  LED2=2,  //ĸ����LED0		��
  LED3=3,  //ĸ����LED1		��
  LEDALL=4
} LEDn_e;

typedef enum
{
  ON=0,  //��
  OFF=1, //��
  RVS=2, //��ת
}LEDs_e;

//����Ĺܽ�Ҫ��Ӧʵ�ʰ���
#define LED0p      P42   //���İ���LED0 ����
#define LED1p      P62   //���İ���LED1 �̵�
#define LED2p      P22   //ĸ����LED0		��
#define LED3p      P23   //ĸ����LED1		��

/*********************** UART���ܺ��� **************************/
//��ʼ��
void GPIO_LED_Init(void);
void LED_Ctrl(LEDn_e LEDno, LEDs_e sta);
void Test_GPIO_LED(void);
void Led_Flash(u16 time);
#endif

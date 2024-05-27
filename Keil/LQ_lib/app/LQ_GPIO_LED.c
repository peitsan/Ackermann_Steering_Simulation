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
#include "LQ_GPIO.h"


/*************************************************************************
*  �������ƣ�void LED_Init(void)
*  ����˵����GPIO��ʼ������ LED������P42 P62��ʼ��
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2022��3��8��
*  ��    ע��P62������ΪOLED�Ƶ����ţ����ʺ���ʹ��OLED��Ļ�������ʹ��led�ƣ��˴�δ��ʼ��
*************************************************************************/
void GPIO_LED_Init(void)
{
	P4M1 = 0x0;
	P4M0 = 0x0;		//P4������Ϊ׼˫���
	P6M1 = 0x0;
	P6M0 = 0x0;		//P6������Ϊ׼˫���
}

/*************************************************************************
*  �������ƣ�void LED_Ctrl(LEDn_e LEDno, LEDs_e sta)
*  ����˵����LED����
*  ����˵����LEDn_e LEDno���,LEDs_e sta״̬��/��/��ת
*  �������أ���
*  �޸�ʱ�䣺2022��3��8��
*  ��    ע��
*************************************************************************/
void LED_Ctrl(LEDn_e LEDno, LEDs_e sta)
{
    switch(LEDno)
    {
    case LED0:
      if(sta==ON)        LED0p=0;
      else if(sta==OFF)  LED0p=1;
      else if(sta==RVS) (LED0p)?(LED0p=0):(LED0p=1);
    break;

    case LED1:
      if(sta==ON)        LED1p=0;
      else if(sta==OFF)  LED1p=1;
      else if(sta==RVS) (LED1p)?(LED1p=0):(LED1p=1);
    break;

    case LED2:
      if(sta==ON)        LED2p=0;
      else if(sta==OFF)  LED2p=1;
      else if(sta==RVS) (LED2p)?(LED2p=0):(LED2p=1);
    break;

    case LED3:
      if(sta==ON)        LED3p=0;
      else if(sta==OFF)  LED3p=1;
      else if(sta==RVS) (LED3p)?(LED3p=0):(LED3p=1);
    break;
    case LEDALL:
      if(sta==ON)
      {
    	  LED0p=0;
    	  LED1p=0;
    	  LED2p=0;
				LED3p=0;
      }
      else if(sta==OFF)
      {
    	  LED0p=1;
    	  LED1p=1;
    	  LED2p=1;
				LED3p=1;
      }
      else if(sta==RVS)
      {
    	  (LED0p)?(LED0p=0):(LED0p=1);
    	  (LED1p)?(LED1p=0):(LED1p=1);
    	  (LED2p)?(LED2p=0):(LED2p=1);
    	  (LED3p)?(LED3p=0):(LED3p=1);
      }
    break;
    default:
    break;
    }
}
/**************************************************************************
�������ܣ�LED��˸
��ڲ�������˸Ƶ�� 
����  ֵ����
**************************************************************************/
void Led_Flash(u16 time)
{
	  static int temp;
	  if(0==time) LED0p=0;
	  else if(++temp>time)	LED0p=~LED0p,temp=0;
}

/*************************************************************************
*  �������ƣ�void Test_GPIO_LED(void)
*  ����˵�������Գ���
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2022��3��8��
*  ��    ע�����İ��ϵ���ɫLED����˸��P62������ΪOLED�Ƶ����źͺ��İ��̵ƣ�����
						 ����ʹ��OLED��Ļ�������ʹ��led�ƣ��˴�δʹ�ã�����Ҫ���������
*************************************************************************/

void Test_GPIO_LED(void)
{
	GPIO_LED_Init();
 	while(1)
 	{
		LED_Ctrl(LED0,RVS);        //LED��˸
 		LED_Ctrl(LED1,RVS);        //LED��˸
		delayms(200);              
 	}
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
��ƽ    ̨�������������ܿƼ�STC32G12K128
����    д��chiusir
��E-mail  ��chiusir@163.com
������汾��V1.0 ��Ȩ���У���λʹ��������ϵ��Ȩ
�������¡�2022��3��3��
�������Ϣ�ο����е�ַ��
����    վ��http://www.lqist.cn
���Ա����̡�http://longqiu.taobao.com
------------------------------------------------
��dev.env.��Keil5.2�����ϰ汾
��Target �� STC32G12K128
��SYS PLL�� 35MHz
------------------------------------------------
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#include "include.h"
#include "LQ_GPIO.h"

/*************************************************************************
*  �������ƣ�void PIN_InitConfig(GPIO_Name_t pin, IfxPort_Mode mode, unsigned char output)
*  ����˵����GPIO��ʼ������
*  ����˵����pin      �� �ܽű��    lq_gpio.h�ж���
*            mode     �� GPIO ģʽ   lq_gpio.h�еĺ궨����ѡ��
*            output   �� ���ģʽʱ�������ƽ 1���ߵ�ƽ  0���͵�ƽ
*  �������أ���
*  �޸�ʱ�䣺2020��3��10��
*  ��    ���� PIN_InitConfig(P00_0, PIN_MODE_OUTPUT, 1);  //P00_0��ʼ��������� �ߵ�ƽ
*************************************************************************/
void PIN_InitConfig(GPIO_Pin pin, Port_Mode mode)
{
	unsigned int pin_num, port_num;
	
	pin_num = (pin&0xF0)>>4;
	port_num = pin & 0x0F;
	
	switch(pin_num)
	{
		case 0:
			if(mode == 0){P0M1 &= ~(0x01<<port_num);	P0M0 &= ~(0x01<<port_num);}
			if(mode == 1){P0M1 &= ~(0x01<<port_num);	P0M0 |= ~(0x01<<port_num);}
			if(mode == 2){P0M1 |= ~(0x01<<port_num);	P0M0 &= ~(0x01<<port_num);}
			if(mode == 3){P0M1 |= ~(0x01<<port_num);	P0M0 |= ~(0x01<<port_num);}
			break;
			
			case 1:
			if(mode == 0){P1M1 &= ~(0x01<<port_num);	P1M0 &= ~(0x01<<port_num);}
			if(mode == 1){P1M1 &= ~(0x01<<port_num);	P1M0 |= ~(0x01<<port_num);}
			if(mode == 2){P1M1 |= ~(0x01<<port_num);	P1M0 &= ~(0x01<<port_num);}
			if(mode == 3){P1M1 |= ~(0x01<<port_num);	P1M0 |= ~(0x01<<port_num);}
			break;
			
			case 2:
			if(mode == 0){P4M1 &= ~(0x01<<port_num);	P4M0 &= ~(0x01<<port_num);}
			if(mode == 1){P4M1 &= ~(0x01<<port_num);	P4M0 |= ~(0x01<<port_num);}
			if(mode == 2){P4M1 |= ~(0x01<<port_num);	P4M0 &= ~(0x01<<port_num);}
			if(mode == 3){P4M1 |= ~(0x01<<port_num);	P4M0 |= ~(0x01<<port_num);}
			break;
			
			case 3:
			if(mode == 0){P3M1 &= ~(0x01<<port_num);	P3M0 &= ~(0x01<<port_num);}
			if(mode == 1){P3M1 &= ~(0x01<<port_num);	P3M0 |= ~(0x01<<port_num);}
			if(mode == 2){P3M1 |= ~(0x01<<port_num);	P3M0 &= ~(0x01<<port_num);}
			if(mode == 3){P3M1 |= ~(0x01<<port_num);	P3M0 |= ~(0x01<<port_num);}
			break;
			
		case 4:
			if(mode == 0){P4M1 &= ~(0x01<<port_num);	P4M0 &= ~(0x01<<port_num);}
			if(mode == 1){P4M1 &= ~(0x01<<port_num);	P4M0 |= ~(0x01<<port_num);}
			if(mode == 2){P4M1 |= ~(0x01<<port_num);	P4M0 &= ~(0x01<<port_num);}
			if(mode == 3){P4M1 |= ~(0x01<<port_num);	P4M0 |= ~(0x01<<port_num);}
			break;
		
		case 5:
			if(mode == 0){P5M1 &= ~(0x01<<port_num);	P5M0 &= ~(0x01<<port_num);}
			if(mode == 1){P5M1 &= ~(0x01<<port_num);	P5M0 |= ~(0x01<<port_num);}
			if(mode == 2){P5M1 |= ~(0x01<<port_num);	P5M0 &= ~(0x01<<port_num);}
			if(mode == 3){P5M1 |= ~(0x01<<port_num);	P5M0 |= ~(0x01<<port_num);}
			break;
			
		case 6:
			if(mode == 0){P6M1 &= ~(0x01<<port_num);	P6M0 &= ~(0x01<<port_num);}
			if(mode == 1){P6M1 &= ~(0x01<<port_num);	P6M0 |= ~(0x01<<port_num);}
			if(mode == 2){P6M1 |= ~(0x01<<port_num);	P6M0 &= ~(0x01<<port_num);}
			if(mode == 3){P6M1 |= ~(0x01<<port_num);	P6M0 |= ~(0x01<<port_num);}
			break;
			
		case 7:
			if(mode == 0){P7M1 &= ~(0x01<<port_num);	P7M0 &= ~(0x01<<port_num);}
			if(mode == 1){P7M1 &= ~(0x01<<port_num);	P7M0 |= ~(0x01<<port_num);}
			if(mode == 2){P7M1 |= ~(0x01<<port_num);	P7M0 &= ~(0x01<<port_num);}
			if(mode == 3){P7M1 |= ~(0x01<<port_num);	P7M0 |= ~(0x01<<port_num);}
			break;
			
	}

}








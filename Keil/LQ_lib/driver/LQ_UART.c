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

u8 Uart4_Receive;

#define Baudrate1   (65536 - MAIN_Fosc / 115200 / 4)
#define Baudrate4   (65536 - MAIN_Fosc / 115200 / 4)

bit busy;
int wptr;
char rptr;
char buffer[64];

//========================================================================
// ����: void UART1_int (void) interrupt UART1_VECTOR
// ����: UART1�жϺ�����
// ����: nine.
// ����: none.
// �汾: VER1.0
// ����: 2014-11-28
// ��ע: 
//========================================================================
void UART1_int (void) interrupt 4
{
	if(TI)
	{
		TI= 0;
		busy= 0;
	}
	if(RI)
	{
		RI= 0;
		buffer[wptr++]= SBUF;
		wptr &= 0xff;
	}
}

//========================================================================
// ����: void UART4_int (void) interrupt UART4_VECTOR
// ����: UART4�жϺ�����
// ����: nine.
// ����: none.
// �汾: VER1.0
// ����: 2022-03-04
// ��ע: 
//========================================================================
void UART4_int (void) interrupt 18
{
	if(S4CON & 0x02)
	{
		S4CON &= ~0x02;  //Clear Tx flag  ������ͱ�־λ
		busy=0;
	}
	if(S4CON & 0x01)                                  
	{
		S4CON &= ~0x01;  //Clear Rx flag	������ձ�־λ
		buffer[wptr++]= S4BUF;
		wptr &= 0xff;
	}
}
//========================================================================
// ����: SetTimer2Baudraye(u32 dat)
// ����: ����Timer2�������ʷ�������
// ����: dat: Timer2����װֵ.
// ����: none.
// �汾: VER1.0
// ����: 2014-11-28
// ��ע: 
//========================================================================
void SetTimer2Baudraye(u32 dat)
{
	S4CON = 0x10;                       //8λ����, ʹ��Timer2�������ʷ�����, �������
	T2H = dat>>8;
	T2L = dat;
	AUXR = 0x14;
}
//========================================================================
// ����: void   UART1_config(u16 tmr)
// ����: UART1��ʼ��������
// ����: tmr: ѡ������, 2: ʹ��Timer2��������, ����ֵ: ʹ��Timer1��������.
// ����: none.
// �汾: VER1.0
// ����: 2014-11-28
// ��ע: 
//========================================================================
void UART1_config()
{
//	SCON= 0x50;		//�ɱ䲨���� 8λ���ݷ�ʽ �����ڽ�������
//	T2L = Baudrate1;		//��ʱ��2
//	T2H = Baudrate1>>8;
//	AUXR= 0x15;		//ѡ��ʱ��2��Ϊ�����ʷ�����
//	wptr = 0x00;
//	rptr = 0x00;
//	busy= 0;

		TR1 = 0;
		AUXR &= ~0x01;                                        //S1 tmr Use Timer1;
		AUXR |=  (1<<6);                                      //Timer1 set as 1T mode
		TMOD &= ~(1<<6);                                      //Timer1 set As Timer
		TMOD &= ~0x30;                                        //Timer1_16bitAutoReload;
		TH1 = (u8)(Baudrate1 / 256);
		TL1 = (u8)(Baudrate1 % 256);
		ET1 = 0;    				                           	//��ֹ�ж�
		INTCLKO &= ~0x02;		                               	//�����ʱ��
		TR1  = 1;


    SCON = (SCON & 0x3f) | 0x40;                              	//UART1ģʽ, 0x00: ͬ����λ���, 0x40: 8λ����,�ɱ䲨����, 0x80: 9λ����,�̶�������, 0xc0: 9λ����,�ɱ䲨����
//  PS  = 1;                                                  	//�����ȼ��ж�
    ES  = 1;                                                  	//�����ж�
    REN = 1;                                                 	 //�������
    P_SW1 &= 0x3f;
//  P_SW1 |= 0x00;                                            	//UART1 switch to, 0x00: P3.0 P3.1, 0x40: P3.6 P3.7, 0x80: P1.6 P1.7, 0xC0: P4.3 P4.4

}

void UART4_config(u16 tmr)              // ѡ��ʱ��Դ, 2: ʹ��Timer2��������, ����ֵ: ʹ��Timer4��������.
{
	if(tmr == 2)													//ʹ��TIM2��Ϊ������ʱ��Դ
	{
		SetTimer2Baudraye(Baudrate4);
	}
	else
	{
		S4CON = 0x50;                       //8λ����, ʹ��Timer4�������ʷ�����, �������
		T4H = Baudrate4>>8;
		T4L = Baudrate4;
		T4T3M = 0xA0;												//��ʱ��4��ʼ����������Ƶ
	}
//		P_SW2 &= ~0x04;                   //UART4 switch bit2 to: 0: P0.2 P0.3ѡ������
	P_SW2 |= 0x04;                      //UART4 switch bit2 to: 1: P5.2 P5.3ѡ������
	wptr = 0x00;												//������־λ����
	rptr = 0x00;
	busy= 0;

	IE2 |= 0x10;                        //����UART4�ж�
	
}


/*************************************************************************
*  �������ƣ�void UART_PutChar(UART_t  uratn, char ch)
*  ����˵����UART�����ֽں���,ʹ��ǰ���ȳ�ʼ����Ӧ����
*  ����˵����uratn �� UART0 - UART3/ ch    �� Ҫ��ӡ���ַ�
*  �������أ���
*  �޸�ʱ�䣺2020��3��10��
*  ��    ע��UART_PutChar(UART0, 'a');  //��ӡ�ַ�a
*************************************************************************/
void UART1_PutChar(char dat)
{
	while(busy);
	busy= 1;
	SBUF= dat;
}

/*************************************************************************
*  �������ƣ�void UART_PutStr(UART_t  uratn, char *st)
*  ����˵����UART�����ַ�������(�� NULL ֹͣ����),ʹ��ǰ���ȳ�ʼ����Ӧ����
*  ����˵����uratn �� UART0 - UART3/ str   �� Ҫ��ӡ���ַ�����ַ
*  �������أ���
*  �޸�ʱ�䣺2020��3��10��
*  ��    ע��UART_PutStr(UART3, "123456789"); //����9���ֽ�
*************************************************************************/
void UART1_PutStr(char *p)
{
	char *p1;
	p1=p;
	while(*p1)
	{
		UART1_PutChar(*p1++);
	}
}

void UART4_PutChar(char ch)
{
	while(busy);
	busy=1;
	S4BUF = ch;
}
/*************************************************************************
*  �������ƣ�void UART_PutStr(UART_t  uratn, char *st)
*  ����˵����UART�����ַ�������(�� NULL ֹͣ����),ʹ��ǰ���ȳ�ʼ����Ӧ����
*  ����˵����uratn �� UART0 - UART3/ str   �� Ҫ��ӡ���ַ�����ַ
*  �������أ���
*  �޸�ʱ�䣺2020��3��10��
*  ��    ע��UART_PutStr(UART3, "123456789"); //����9���ֽ�
*************************************************************************/

void UART4_PutStr(char *st)
{
  while(*st)
  {
    UART4_PutChar(*st++);
  }
}
/*************************************************************************
*  �������ƣ�char UART_GetChar(UART_t  uratn)
*  ����˵����UART��ȡ�ֽ� ʹ��ǰ���ȳ�ʼ����Ӧ���� ����ǰ��ȷ���н��յ����� �����ȴ����ݵ���
*  ����˵����uratn �� UART0 - UART3
*  �������أ���ȡ�ֽ�
*  �޸�ʱ�䣺2020��3��10��
*  ��    ע��UART_GetChar(UART3); //����һ���ַ�
*************************************************************************/
char UART1_GetChar(void)
{  
  return (SBUF);
}
char UART4_GetChar(void)
{  
  return (S4BUF);
}

/*************************************************************************
*  �������ƣ�void UART4_PutBuff(char *st, u16 len)
*  ����˵����UART�����ַ���ʹ��ǰ���ȳ�ʼ����Ӧ���� ����ǰ��ȷ���н��յ����� �����ȴ����ݵ���
*  �޸�ʱ�䣺2023��2��7��
//��������վ��λ���ӿں���
*************************************************************************/
void UART4_PutBuff(char *st, u16 len)
{
  while(len--)
  {
    UART4_PutChar(*st++);
  }
}
void UART1_PutBuff(char *st, u16 len)
{
  while(len--)
  {
    UART1_PutChar(*st++);
  }
}
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
����������void delayus(unsigned int us)
����  �ܡ���ʱ����
������ֵ��us /30M
������ֵ���� 
����  �ߡ�chiusir
�������¡�2021��1��22�� 
������汾��V1.0
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void  delayus(unsigned int us)
{
  unsigned int i;
  do{    
    i=MAIN_Fosc/6000001;//��Լ1us
    while(--i);
  }while(--us);
}
//

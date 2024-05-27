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
#ifndef __LQ_KEY_H_
#define __LQ_KEY_H_

#include "include.h"

//����ģ���
typedef enum
{
    KEY0 = 0,  	//ĸ���ϰ���0
    KEY1 = 1,		//ĸ���ϰ���1
    KEY2 = 2,		//ĸ���ϰ���2
	  KEY3 = 3,		//��
    DSW0 = 4,   //ĸ���ϲ��뿪��1
    DSW1 = 5,		//ĸ���ϲ��뿪��2
} KEYn_e;

typedef enum
{
    LOW = 0,  		//����
    HIGH = 1, 		//�ɿ�
    FAIL = 0xff, 	//����
} KEYs_e;

typedef enum //
{
    NOKEYDOWN = 0, 			//û�а���
		KEY0DOWN = 0x01,  	//ĸ���ϰ���0
    KEY1DOWN = 0x02,		//ĸ���ϰ���1
    KEY2DOWN = 0x04,		//ĸ���ϰ���2
    KEY01DOWN = 0x03,  	//ĸ���ϰ���0&1
    KEY02DOWN = 0x05,		//ĸ���ϰ���0&2
    KEY12DOWN = 0x06,		//ĸ���ϰ���1&2
    KEY012DOWN = 0x07,	//ĸ���ϰ���0&1&2
    KEYError = 0xFF,		//����
} KEYdown_e;

//����Ĺܽ�Ҫ��Ӧʵ�ʰ���
#define KEY0p      P70 //ĸ���ϰ���0
#define KEY1p      P71 //ĸ���ϰ���1
#define KEY2p      P72 //ĸ���ϰ���2
#define KEY3p       //ĸ���ϰ���3���޴˰�����

#define DSW0p      P32 //ĸ���ϲ��뿪��
#define DSW1p      P33 //ĸ���ϲ��뿪��

#define REEDp      P37 //ĸ���ϸɻɹ�

void GPIO_KEY_Init (void);
unsigned char KEY_Read (KEYn_e KEYno);
unsigned char KEY_Read_All (void);
void Test_GPIO_KEY (void);
void Test_ComKEY_Tft (void);

#endif




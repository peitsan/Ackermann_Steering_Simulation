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
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ

�����������վ��λ��

QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#include <LQ_UART.h>
#include <ANO_DT.h>

//ʹ������4.3��λ��Э��
/////////////////////////////////////////////////////////////////////////////////////
//���ݲ�ֺ궨�壬�ڷ��ʹ���1�ֽڵ���������ʱ������int16��float�ȣ���Ҫ�����ݲ�ֳɵ����ֽڽ��з���
#define BYTE0(dwTemp)       ( *( (char *)(&dwTemp)	  ) )     /*!< uint32_t ���ݲ�� byte0  */
#define BYTE1(dwTemp)       ( *( (char *)(&dwTemp) + 1) )     /*!< uint32_t ���ݲ�� byte1  */
#define BYTE2(dwTemp)       ( *( (char *)(&dwTemp) + 2) )     /*!< uint32_t ���ݲ�� byte2  */
#define BYTE3(dwTemp)       ( *( (char *)(&dwTemp) + 3) )     /*!< uint32_t ���ݲ�� byte3  */

unsigned char data_to_send[64];

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
����������void UART4_SendInt16(unsigned char Function, int outData)
����  �ܡ�������λ���û�Э�飬ʹ��ʱռ��MCU��Դ�ϴ��ܳ�ʱ��ʹ��
������ֵ��unsigned char Function, �����ַ�
������ֵ��int outData ����
������ֵ���� 
����  �ߡ�chiusir
�������¡�2021��1��22�� 
������汾��V1.0
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void UART4_SendInt16(unsigned char Function, int outData)
{
	unsigned char sum;
	unsigned char outData_H = ((unsigned short)outData)>>8;
	unsigned char outData_L = (unsigned char)outData;
	S4BUF = (0xAA);   // ͷ
	delayus(90);
	S4BUF = (0xAA);   // ͷ
	delayus(90);
	S4BUF = Function; // ����
	delayus(90);
	S4BUF = (0x02);   // ����Ϊ�����ֽ�
	delayus(90);
	S4BUF = outData_H;// ���ݸ�8λ
	delayus(90);
	S4BUF = outData_L;// ���ݵ�8λ
	delayus(90);
	sum = 0xAA | 0xAA | 0xF1 | 0x02 | outData_H | outData_L;// У��ֵ
	S4BUF = sum;
	delayus(90);
}


void ANO_DT_send_int16byte16(short data1, short data2, short data3, short data4, short data5, short data6, short data7, short data8 )
{
  unsigned char  _cnt=0;
  unsigned char  sum = 0, i = 0;
  
  data_to_send[_cnt++] = 0xAA;      //����Э��֡ͷ  0xAAAA
  data_to_send[_cnt++] = 0xAA;
  data_to_send[_cnt++] = 0xF1;      //ʹ���û�Э��֡0xF1
  data_to_send[_cnt++] = 16;        //8��int16_t ���� 16���ֽ�
  
  data_to_send[_cnt++]=(unsigned short)(data1>>8);
  data_to_send[_cnt++]=(unsigned char)(data1);
  
  data_to_send[_cnt++]=(unsigned short)(data2>>8);
  data_to_send[_cnt++]=(unsigned char)(data2);
  
  data_to_send[_cnt++]=(unsigned short)(data3>>8);
  data_to_send[_cnt++]=(unsigned char)(data3);
  
  data_to_send[_cnt++]=(unsigned short)(data4>>8);
  data_to_send[_cnt++]=(unsigned char)(data4);
  
  data_to_send[_cnt++]=(unsigned short)(data5>>8);
  data_to_send[_cnt++]=(unsigned char)(data5);
  
  data_to_send[_cnt++]=(unsigned short)(data6>>8);
  data_to_send[_cnt++]=(unsigned char)(data6);
  
  data_to_send[_cnt++]=(unsigned short)(data7>>8);
  data_to_send[_cnt++]=(unsigned char)(data7);
  
  data_to_send[_cnt++]=(unsigned short)(data8>>8);
  data_to_send[_cnt++]=(unsigned char)(data8);
  
  sum = 0;
  for(i=0;i<_cnt;i++)
    sum += data_to_send[i];
  data_to_send[_cnt++]=sum;
  
  UART1_PutBuff(data_to_send, _cnt);     //�����޸Ĳ�ͬ�Ĵ��ڷ�������;
}

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


#ifndef __LQ_SOFTIIC_H
#define __LQ_SOFTIIC_H

#include "include.h"

/* ע�� IIC���߹涨��IIC����ʱ SCL��SDA��Ϊ�ߵ�ƽ ����ⲿ������һ������������ */
/* ģ�� IIC��Ҫע�⣬IIC��ַ����һλ ����MPU6050 ģ����ǵ�ַ 0xD0 */
/* �뻻�ñ��IO ֱ���޸ĺ궨�� SOFT_IIC_SCL_PIN �� SOFT_IIC_SDA_PIN ���� */
#define SOFT_IIC_SCL_PIN   P15   /*!< P15  ��Ϊ SCL */
#define SOFT_IIC_SDA_PIN   P14   /*!< P14  ��Ϊ SDA */

#define SDA_OUT       P1M0 &= ~0x10; P1M1 &= ~0x10; 
#define SDA_IN        P1M0 &= ~0x10; P1M1 |= 0x10; 

#define IIC_SCL_INIT       P1M0 |= 0x20; P1M1 &= ~0x20; 
#define IIC_SDA_INIT       P1M0 &= ~0x10; P1M1 &= ~0x10; 

#define IIC_SCL_H      SOFT_IIC_SCL_PIN = 1;
#define IIC_SCL_L      SOFT_IIC_SCL_PIN = 0;

#define IIC_SDA_H      SOFT_IIC_SDA_PIN = 1;
#define IIC_SDA_L      SOFT_IIC_SDA_PIN = 0;

#define IIC_SDA_READ   SOFT_IIC_SDA_PIN

void IIC_Start(void);			    //����IIC��ʼ�ź�
void IIC_Stop(void);	  	    //����IICֹͣ�ź�
void IIC_Ack(void);						//IIC����ACK�ź�
void IIC_NAck(void);					//IIC������ACK�ź�
unsigned char IIC_WaitAck(void); 		    //IIC�ȴ�ACK�ź�
void IIC_SendByte(unsigned char data_t);    //IIC����һ���ֽ�
unsigned char IIC_ReadByte(unsigned char ack);      //IIC��ȡһ���ֽ�
int LQ_I2C_Write(unsigned char addr, unsigned char reg, unsigned int len, unsigned char *dat);
int LQ_I2C_Read(unsigned char addr, unsigned char reg, unsigned int len, unsigned char *buf);
void IIC_Init(void);
unsigned char IIC_ReadByteFromSlave(unsigned char I2C_Addr,unsigned char reg,unsigned char *buf);
unsigned char IIC_ReadMultByteFromSlave(unsigned char dev, unsigned char reg, unsigned char length, unsigned char *data_t);
unsigned char IIC_WriteByteToSlave(unsigned char I2C_Addr,unsigned char reg,unsigned char buf);
unsigned char IIC_WriteMultByteToSlave(unsigned char dev, unsigned char reg, unsigned char length, unsigned char* data_t);

#endif

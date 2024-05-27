/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 【平    台】北京龙邱智能科技STC32G12K128 LQFP64核心板
 【编    写】chiusir
 【E-mail  】chiusir@163.com
 【软件版本】V1.1 版权所有，单位使用请先联系授权
 【最后更新】2022年3月8日
 【相关信息参考下列地址】
 【网    站】http://www.lqist.cn
 【淘宝店铺】http://longqiu.taobao.com
 ------------------------------------------------
 【IDE】STC32
 【Target 】 keil5.2及以上
 【SYS PLL】 35MHz使用内部晶振，通过
=================================================================
stc-isp下载时, 选择时钟 35MHZ
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/


#ifndef __LQ_SOFTIIC_H
#define __LQ_SOFTIIC_H

#include "include.h"

/* 注意 IIC总线规定，IIC空闲时 SCL和SDA都为高电平 最好外部上拉（一定不能下拉） */
/* 模拟 IIC需要注意，IIC地址左移一位 例如MPU6050 模拟就是地址 0xD0 */
/* 想换用别的IO 直接修改宏定义 SOFT_IIC_SCL_PIN 、 SOFT_IIC_SDA_PIN 即可 */
#define SOFT_IIC_SCL_PIN   P15   /*!< P15  作为 SCL */
#define SOFT_IIC_SDA_PIN   P14   /*!< P14  作为 SDA */

#define SDA_OUT       P1M0 &= ~0x10; P1M1 &= ~0x10; 
#define SDA_IN        P1M0 &= ~0x10; P1M1 |= 0x10; 

#define IIC_SCL_INIT       P1M0 |= 0x20; P1M1 &= ~0x20; 
#define IIC_SDA_INIT       P1M0 &= ~0x10; P1M1 &= ~0x10; 

#define IIC_SCL_H      SOFT_IIC_SCL_PIN = 1;
#define IIC_SCL_L      SOFT_IIC_SCL_PIN = 0;

#define IIC_SDA_H      SOFT_IIC_SDA_PIN = 1;
#define IIC_SDA_L      SOFT_IIC_SDA_PIN = 0;

#define IIC_SDA_READ   SOFT_IIC_SDA_PIN

void IIC_Start(void);			    //发送IIC开始信号
void IIC_Stop(void);	  	    //发送IIC停止信号
void IIC_Ack(void);						//IIC发送ACK信号
void IIC_NAck(void);					//IIC不发送ACK信号
unsigned char IIC_WaitAck(void); 		    //IIC等待ACK信号
void IIC_SendByte(unsigned char data_t);    //IIC发送一个字节
unsigned char IIC_ReadByte(unsigned char ack);      //IIC读取一个字节
int LQ_I2C_Write(unsigned char addr, unsigned char reg, unsigned int len, unsigned char *dat);
int LQ_I2C_Read(unsigned char addr, unsigned char reg, unsigned int len, unsigned char *buf);
void IIC_Init(void);
unsigned char IIC_ReadByteFromSlave(unsigned char I2C_Addr,unsigned char reg,unsigned char *buf);
unsigned char IIC_ReadMultByteFromSlave(unsigned char dev, unsigned char reg, unsigned char length, unsigned char *data_t);
unsigned char IIC_WriteByteToSlave(unsigned char I2C_Addr,unsigned char reg,unsigned char buf);
unsigned char IIC_WriteMultByteToSlave(unsigned char dev, unsigned char reg, unsigned char length, unsigned char* data_t);

#endif

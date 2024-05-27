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
#ifndef __LQ_SPI_GYRO_H_
#define __LQ_SPI_GYRO_H_

#include "include.h"


#define ICM_PWR_MGMT1_REG		0X6B	//电源管理寄存器1
#define ICM_PWR_MGMT2_REG		0X6C	//电源管理寄存器2
#define ICM_CFG_REG				0X1A	//配置寄存器
#define ICM_INT_EN_REG			0X38	//中断使能寄存器
#define ICM_USER_CTRL_REG		0X6A	//用户控制寄存器
#define ICM_GYRO_CFG_REG		0X1B	//陀螺仪配置寄存器
#define ICM_ACCEL_CFG_REG		0X1C	//加速度计配置寄存器
#define ICM_SAMPLE_RATE_REG		0X19	//采样频率分频器
#define ICM_ACCEL_XOUTH_REG		0X3B	//加速度值,X轴高8位寄存器

unsigned char Gyro_Get_Raw_data_SPI(short *ax,short *ay,short *az,short *gx,short *gy,short *gz);
void Test_SPI_Gyro(void);

#endif


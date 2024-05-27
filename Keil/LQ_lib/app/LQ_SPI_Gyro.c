/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 【平    台】北京龙邱智能科技STC32G12K128 LQFP64核心板
 【编    写】chiusir
 【E-mail  】chiusir@163.com
 【软件版本】V1.1 版权所有，单位使用请先联系授权
 【最后更新】2023年11月20日
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

#include "include.h"

#define MPU_ADDR  0x68  //SPI写入时的地址字节数据，+1为读取

unsigned char SPI_ICM20602 = 0;
unsigned char SPI_ICM20689 = 0;
unsigned char SPI_MPU9250 = 0;
unsigned char SPI_ICM42605 = 0;
unsigned char SPI_errorid = 0;

unsigned char Gyro_ID_SPI = 0;

unsigned char Gyro_Read_Byte_SPI(unsigned char reg);

signed short  aacx_spi,aacy_spi,aacz_spi;            //加速度传感器原始数据
signed short  gyrox_spi,gyroy_spi,gyroz_spi;         //陀螺仪原始数据
short magx_spi, magy_spi, magz_spi;

/**
  * @brief   读取陀螺仪的设备ID
  *
  * @param   void
  *
  * @retur   void
  *
  * @note
  *
  * @see      Gyro_Chose();
  *
  * @date     2021/7/12 星期一
  */
unsigned char Gyro_Chose_SPI(void)
{
    Gyro_ID_SPI = Gyro_Read_Byte_SPI(WHO_AM_I);
    switch(Gyro_ID_SPI)
    {
        case 0x12:SPI_ICM20602 = 1;break;
        case 0x71:SPI_MPU9250 = 1;break;
        case 0x98:SPI_ICM20689 = 1;break;
        case 0x42:SPI_ICM42605 = 1;break;
        default :SPI_errorid = 1;return Gyro_ID_SPI;
    }
    return Gyro_ID_SPI;
}

unsigned char Gyro_Read_Byte_SPI(unsigned char reg)
{
    uint8 buff[2];
    buff[0] = reg | 0x80;          //先发送寄存器

    Soft_SPI_ReadWriteNbyte(buff, 2);
    
    return buff[1];
}

void ICM_Read_Len(uint8_t reg,uint8_t len,uint8_t *buf)
{   
    buf[0] = reg | 0x80;
    /* 写入要读的寄存器地址 */
    Soft_SPI_ReadWriteNbyte(buf, len + 1);

}

void Gyro_Write_Byte_SPI(uint8_t reg,uint8_t value)
{
    uint8_t buff[2];

    buff[0] = reg;          //先发送寄存器
    buff[1] = value;        //再发送数据

    Soft_SPI_ReadWriteNbyte(buff, 2);//发送buff里数据，并采集到 buff里
}

unsigned char Gyro_Get_Raw_data_SPI(short *ax,short *ay,short *az,short *gx,short *gy,short *gz)
{
    unsigned char buf[15];
	ICM_Read_Len(ICM_ACCEL_XOUTH_REG,14,buf);
	
    *ax=((uint16)buf[1]<<8)|buf[2];
    *ay=((uint16)buf[3]<<8)|buf[4];
    *az=((uint16)buf[5]<<8)|buf[6];
    *gx=((uint16)buf[9]<<8)|buf[10];
    *gy=((uint16)buf[11]<<8)|buf[12];
	*gz=((uint16)buf[13]<<8)|buf[14];
    return 0;
}
//测试母板IMU陀螺仪接口，测试spi陀螺仪20602、20689  使用时需要将该接口的3v3引脚和板子上其他的3v3引脚相连
void Test_SPI_Gyro(void)
{
    signed short accelerat_X, accelerat_Y, accelerat_Z;
    signed short gyro_X, gyro_Y, gyro_Z;
    unsigned char res;
    char  txt[30];

    Soft_SPI_Init();

	delayms(500);
    OLED_Init();                //LCD初始化
    OLED_CLS();                 //LCD清屏
    
    res = Gyro_Chose_SPI();
    OLED_P14x16Str(8,0,(unsigned char*)"北京龙邱智能科技");		//字符串显示
    sprintf(txt,"ID:0X%2x",res);
    OLED_P6x8Str(70,2,txt);     // 字符串显示 
    if(res == 0xff)
    {
        sprintf(txt,"Gyro init error",res);
        OLED_P6x8Str(5,4,txt);     // 字符串显示
        while(1);        
    }
    
    Gyro_Write_Byte_SPI(ICM_PWR_MGMT1_REG,0X80);//复位
    delayms(100);  //延时100ms
    Gyro_Write_Byte_SPI(ICM_PWR_MGMT1_REG,0X00);//唤醒
    delayms(100);  //延时100ms

    Gyro_Write_Byte_SPI(ICM_GYRO_CFG_REG,3<<3);//设置陀螺仪满量程范围
    Gyro_Write_Byte_SPI(ICM_ACCEL_CFG_REG,1<<3);//设置加速度传感器满量程范围  
    Gyro_Write_Byte_SPI(ICM_SAMPLE_RATE_REG,1000/999);	//设置数字低通滤波器
    Gyro_Write_Byte_SPI(ICM_CFG_REG,1);//设置数字低通滤波器  
    Gyro_Write_Byte_SPI(ICM_CFG_REG,0x02);      //设置数字低通滤波器   98hz
    Gyro_Write_Byte_SPI(ICM_INT_EN_REG,0X00);   //关闭所有中断
    Gyro_Write_Byte_SPI(ICM_USER_CTRL_REG,0X00);//I2C主模式关闭
    Gyro_Write_Byte_SPI(ICM_PWR_MGMT1_REG,0X01);//设置CLKSEL,PLL X轴为参考
    Gyro_Write_Byte_SPI(ICM_PWR_MGMT2_REG,0X00);//加速度与陀螺仪都工作

    while(1)
    {
        Gyro_Get_Raw_data_SPI(&accelerat_X, &accelerat_Y, &accelerat_Z, &gyro_X, &gyro_Y, &gyro_Z);
        sprintf((char*)txt,"ax:%06d",accelerat_X);
        OLED_P6x8Str(0,2,txt);
        sprintf((char*)txt,"ay:%06d",accelerat_Y);
        OLED_P6x8Str(0,3,txt);
        sprintf((char*)txt,"az:%06d",accelerat_Z);
        OLED_P6x8Str(0,4,txt);
        sprintf((char*)txt,"gx:%06d",gyro_X);
        OLED_P6x8Str(0,5,txt);
        sprintf((char*)txt,"gy:%06d",gyro_Y);
        OLED_P6x8Str(0,6,txt);
        sprintf((char*)txt,"gz:%06d",gyro_Z);
        OLED_P6x8Str(0,7,txt);
         delayms(20);
    }
}

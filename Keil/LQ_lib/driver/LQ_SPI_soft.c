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


#include "include.h"


/*************************************************************************
*  函数名称：void SPI_SoftReadWriteNbyte(u8 *lqbuff, u16 len)
*  功能说明：SPI读写数据及长度
*  参数说明：u8 *buf数据指针,u16 len长度
*  函数返回：
*  修改时间：2020年8月10日
*  备    注：   
*************************************************************************/
void Soft_SPI_ReadWriteNbyte(u8 *lqbuff, u16 len)
{
	u8 i;
	
    SPI_CS = 0;
    SPI_SCK= 1;
    do
    {
        for(i = 0; i < 8; i++)
        {   
            SPI_MOSI = ((*lqbuff) >= 0x80);
            SPI_SCK= 0;
            (*lqbuff) = (*lqbuff)<<1;      

            SPI_SCK= 1;

            (*lqbuff) |= SPI_MISO;          
        }
    lqbuff++;
  }while(--len);
  SPI_CS = 1;
}

/*************************************************************************
*  函数名称：void SPI_SoftInit(void)
*  功能说明：模拟SPI口初始化
*  参数说明：无
*  函数返回：无
*  修改时间：2020年8月10日
*  备    注：   
*************************************************************************/
void Soft_SPI_Init(void)
{

	P2M1 = 0;   P2M0 = 0;   //设置为准双向口
//	P2PU =0x3C;          //上拉
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void LQ_SPI_Read(unsigned char reg, unsigned short len, unsigned char* buf)
@功能说明：SPI从设备读取数据
@参数说明：unsigned char reg,设备起始地址unsigned char* buf,数据存放地址unsigned short len,待写入的数据长度
@函数返回：无
@修改时间：2022/02/24
@调用方法：SPI_Read_Nbytes(SPI1,reg|0x80,buf,len);
@备    注：无
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void LQ_SPI_Read(unsigned char reg, unsigned short len, unsigned char* buf)
{  
    unsigned short i;
    unsigned char buff[32];
    buff[0] = reg | 0x80;  
        
    Soft_SPI_ReadWriteNbyte(buff, len+1); 
    for(i=0;i<len;i++)
        buf[i]=buff[i+1];
}
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void LQ_SPI_Write(unsigned char reg, unsigned short len, unsigned char* buf)
@功能说明：SPI向设备写入数据
@参数说明：unsigned char reg,设备起始地址unsigned char* buf,数据存放地址unsigned short len,待写入的数据长度
@函数返回：无
@修改时间：2022/02/24
@调用方法：无
@备    注：无
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void LQ_SPI_Write(unsigned char reg, unsigned char value)
{
    unsigned char buff[2];

    buff[0] = reg & 0x7f;          //先发送寄存器
    buff[1] = value;        //再发送数据
    Soft_SPI_ReadWriteNbyte(buff, 2);
}
unsigned char LQ_SPI_LSM60DSR_GetID(unsigned char reg)
{

    unsigned char ID[1];
    LQ_SPI_Read(reg, 1, ID);
    return ID[0];

}
void LQ_SPI_LSM60DSR_Init(void)
{
    //加速度计52HZ（倾斜角检测功能工作在26HZ，因此加速度计ODR必须设置为>=26hz）,2g量程。
    LQ_SPI_Write(LSM6DSR_CTRL1_XL,0X20);
    //使能加速度计x,y,z轴
    LQ_SPI_Write(LSM6DSR_CTRL9_XL,0X38);
    //陀螺仪电平触发，加速度计高性能使能
    LQ_SPI_Write(LSM6DSR_CTRL6_C,0X40|0x10);
    //陀螺仪高性能使能
    LQ_SPI_Write(LSM6DSR_CTRL7_G,0X80);
    //加速度计INT2引脚失能,陀螺仪数据INT2使能
    LQ_SPI_Write(LSM6DSR_INT2_CTRL,0X03);
    //陀螺仪12.5hz  2000dps
    LQ_SPI_Write(LSM6DSR_CTRL2_G,0X1C);
    //使能陀螺仪x,y,z轴
    LQ_SPI_Write(LSM6DSR_CTRL10_C,0X38);
}
void LQ_SPI_LSM60DSR_Read(signed short *ax,signed short *ay,signed short *az,signed short *gx,signed short *gy,signed short *gz)
{
    unsigned char buf[12];

    LQ_SPI_Read(LSM6DSR_OUTX_L_GYRO, 12, buf);

    *ax=((uint16)buf[1]<<8)|buf[0];
    *ay=((uint16)buf[3]<<8)|buf[2];
    *az=((uint16)buf[5]<<8)|buf[4];
    *gx=((uint16)buf[7]<<8)|buf[6];
    *gy=((uint16)buf[9]<<8)|buf[8];
    *gz=((uint16)buf[11]<<8)|buf[10];
}
/**
  * @brief    LQLSM6DSRSPI测试函数
  *
  * @param    
  *
  * @return   无
  *
  * @note     无
  *
  * @see      Test_SPI_LQLSM6DSR(void));
  * @see      
  *
  * @date     2023/4/10 星期一
  */
void Test_SPI_LQLSM6DSR(void)
{
    signed short accelerat_X, accelerat_Y, accelerat_Z;
    signed short gyro_X, gyro_Y, gyro_Z;
    unsigned char res;
    char  txt[30];

    Soft_SPI_Init();

		delayms(500);

    OLED_Init();                //LCD初始化
    
    OLED_CLS();                 //LCD清屏

    /*读取设备ID*/

    res = LQ_SPI_LSM60DSR_GetID(WHO_AM_LSM6D);

		OLED_P14x16Str(8,0,(unsigned char*)"北京龙邱智能科技");		//字符串显示
		sprintf(txt,"ID:0X%2x",res);
		OLED_P6x8Str(70,2,txt);     //字符串显示 
	
        LQ_SPI_LSM60DSR_Init();
		while(1)
		{
			LQ_SPI_LSM60DSR_Read(&accelerat_X, &accelerat_Y, &accelerat_Z, &gyro_X, &gyro_Y, &gyro_Z);
			
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



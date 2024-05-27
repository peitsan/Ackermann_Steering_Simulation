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
#include "LQ_IIC_VL53.h"

unsigned char VL53_STAR = 0x02;    //0x02 连续测量模式    0x01 单次测量模式

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void Test_Vl53(void)
@功能说明：VL53 写1个寄存器 reg：  寄存器 data：  写入数据
@参数说明：dev：    地址
@函数返回：无
@修改时间：2022/03/15
@调用方法：无
@备    注：无
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void VL53_Write_Byte(unsigned char dev, unsigned char reg, unsigned char val)
{
  IIC_WriteByteToSlave(dev<<1, reg, val);
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void VL53_Read_nByte(unsigned char dev, unsigned char reg, unsigned char length, unsigned char* data)
@功能说明：VL53 写1个寄存器 reg：  寄存器 data：  写入数据
@参数说明：dev：    地址
@函数返回：无
@修改时间：2022/03/15
@调用方法：无
@备    注：无
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void VL53_Read_nByte(unsigned char dev, unsigned char reg, unsigned char length, unsigned char* val)
{
  IIC_ReadMultByteFromSlave(dev<<1, reg, length, val);
}


void Test_IIC_VL53(void)
{
    char txt[36] = {0};
    unsigned char dis_buff[2];
    uint16 dis, last_dis = 0;
    IIC_Init();  
    OLED_P14x16Str(8,0,(unsigned char*)"北京龙邱智能科技");		//字符串显示
    delayms(50);
    VL53_Write_Byte(VL53ADDR, VL53L0X_REG_SYSRANGE_START, VL53_STAR);//   delayms(50);
    while(1)
    {
        
        /* 获取测量数据 */
        VL53_Read_nByte(VL53ADDR, VL53_REG_DIS, 2, dis_buff);
        /* 转换数据 */
        dis = (dis_buff[0]<<8) | (dis_buff[1]);
        
        /* 超过2M没有障碍 */
        if(dis > 8000)
        {
            dis = 0;
        }

        if(dis == 20)
        {
            dis = last_dis;
        }
        last_dis = dis;
        
        sprintf((char*)txt,"dis:%05d",dis);
        OLED_P6x8Str(0,3,txt);
        delayms(50);
    }
}
























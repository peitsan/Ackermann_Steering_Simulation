/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 ��ƽ    ̨�������������ܿƼ�STC32G12K128 LQFP64���İ�
 ����    д��chiusir
 ��E-mail  ��chiusir@163.com
 ������汾��V1.1 ��Ȩ���У���λʹ��������ϵ��Ȩ
 �������¡�2023��11��20��
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
#include "LQ_IIC_VL53.h"

unsigned char VL53_STAR = 0x02;    //0x02 ��������ģʽ    0x01 ���β���ģʽ

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@�������ƣ�void Test_Vl53(void)
@����˵����VL53 д1���Ĵ��� reg��  �Ĵ��� data��  д������
@����˵����dev��    ��ַ
@�������أ���
@�޸�ʱ�䣺2022/03/15
@���÷�������
@��    ע����
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void VL53_Write_Byte(unsigned char dev, unsigned char reg, unsigned char val)
{
  IIC_WriteByteToSlave(dev<<1, reg, val);
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@�������ƣ�void VL53_Read_nByte(unsigned char dev, unsigned char reg, unsigned char length, unsigned char* data)
@����˵����VL53 д1���Ĵ��� reg��  �Ĵ��� data��  д������
@����˵����dev��    ��ַ
@�������أ���
@�޸�ʱ�䣺2022/03/15
@���÷�������
@��    ע����
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
    OLED_P14x16Str(8,0,(unsigned char*)"�����������ܿƼ�");		//�ַ�����ʾ
    delayms(50);
    VL53_Write_Byte(VL53ADDR, VL53L0X_REG_SYSRANGE_START, VL53_STAR);//   delayms(50);
    while(1)
    {
        
        /* ��ȡ�������� */
        VL53_Read_nByte(VL53ADDR, VL53_REG_DIS, 2, dis_buff);
        /* ת������ */
        dis = (dis_buff[0]<<8) | (dis_buff[1]);
        
        /* ����2Mû���ϰ� */
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
























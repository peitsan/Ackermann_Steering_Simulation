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

#define MPU_ADDR  0x68  //SPIд��ʱ�ĵ�ַ�ֽ����ݣ�+1Ϊ��ȡ

unsigned char SPI_ICM20602 = 0;
unsigned char SPI_ICM20689 = 0;
unsigned char SPI_MPU9250 = 0;
unsigned char SPI_ICM42605 = 0;
unsigned char SPI_errorid = 0;

unsigned char Gyro_ID_SPI = 0;

unsigned char Gyro_Read_Byte_SPI(unsigned char reg);

signed short  aacx_spi,aacy_spi,aacz_spi;            //���ٶȴ�����ԭʼ����
signed short  gyrox_spi,gyroy_spi,gyroz_spi;         //������ԭʼ����
short magx_spi, magy_spi, magz_spi;

/**
  * @brief   ��ȡ�����ǵ��豸ID
  *
  * @param   void
  *
  * @retur   void
  *
  * @note
  *
  * @see      Gyro_Chose();
  *
  * @date     2021/7/12 ����һ
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
    buff[0] = reg | 0x80;          //�ȷ��ͼĴ���

    Soft_SPI_ReadWriteNbyte(buff, 2);
    
    return buff[1];
}

void ICM_Read_Len(uint8_t reg,uint8_t len,uint8_t *buf)
{   
    buf[0] = reg | 0x80;
    /* д��Ҫ���ļĴ�����ַ */
    Soft_SPI_ReadWriteNbyte(buf, len + 1);

}

void Gyro_Write_Byte_SPI(uint8_t reg,uint8_t value)
{
    uint8_t buff[2];

    buff[0] = reg;          //�ȷ��ͼĴ���
    buff[1] = value;        //�ٷ�������

    Soft_SPI_ReadWriteNbyte(buff, 2);//����buff�����ݣ����ɼ��� buff��
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
//����ĸ��IMU�����ǽӿڣ�����spi������20602��20689  ʹ��ʱ��Ҫ���ýӿڵ�3v3���źͰ�����������3v3��������
void Test_SPI_Gyro(void)
{
    signed short accelerat_X, accelerat_Y, accelerat_Z;
    signed short gyro_X, gyro_Y, gyro_Z;
    unsigned char res;
    char  txt[30];

    Soft_SPI_Init();

	delayms(500);
    OLED_Init();                //LCD��ʼ��
    OLED_CLS();                 //LCD����
    
    res = Gyro_Chose_SPI();
    OLED_P14x16Str(8,0,(unsigned char*)"�����������ܿƼ�");		//�ַ�����ʾ
    sprintf(txt,"ID:0X%2x",res);
    OLED_P6x8Str(70,2,txt);     // �ַ�����ʾ 
    if(res == 0xff)
    {
        sprintf(txt,"Gyro init error",res);
        OLED_P6x8Str(5,4,txt);     // �ַ�����ʾ
        while(1);        
    }
    
    Gyro_Write_Byte_SPI(ICM_PWR_MGMT1_REG,0X80);//��λ
    delayms(100);  //��ʱ100ms
    Gyro_Write_Byte_SPI(ICM_PWR_MGMT1_REG,0X00);//����
    delayms(100);  //��ʱ100ms

    Gyro_Write_Byte_SPI(ICM_GYRO_CFG_REG,3<<3);//���������������̷�Χ
    Gyro_Write_Byte_SPI(ICM_ACCEL_CFG_REG,1<<3);//���ü��ٶȴ����������̷�Χ  
    Gyro_Write_Byte_SPI(ICM_SAMPLE_RATE_REG,1000/999);	//�������ֵ�ͨ�˲���
    Gyro_Write_Byte_SPI(ICM_CFG_REG,1);//�������ֵ�ͨ�˲���  
    Gyro_Write_Byte_SPI(ICM_CFG_REG,0x02);      //�������ֵ�ͨ�˲���   98hz
    Gyro_Write_Byte_SPI(ICM_INT_EN_REG,0X00);   //�ر������ж�
    Gyro_Write_Byte_SPI(ICM_USER_CTRL_REG,0X00);//I2C��ģʽ�ر�
    Gyro_Write_Byte_SPI(ICM_PWR_MGMT1_REG,0X01);//����CLKSEL,PLL X��Ϊ�ο�
    Gyro_Write_Byte_SPI(ICM_PWR_MGMT2_REG,0X00);//���ٶ��������Ƕ�����

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

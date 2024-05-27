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


#include "include.h"


/*************************************************************************
*  �������ƣ�void SPI_SoftReadWriteNbyte(u8 *lqbuff, u16 len)
*  ����˵����SPI��д���ݼ�����
*  ����˵����u8 *buf����ָ��,u16 len����
*  �������أ�
*  �޸�ʱ�䣺2020��8��10��
*  ��    ע��   
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
*  �������ƣ�void SPI_SoftInit(void)
*  ����˵����ģ��SPI�ڳ�ʼ��
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2020��8��10��
*  ��    ע��   
*************************************************************************/
void Soft_SPI_Init(void)
{

	P2M1 = 0;   P2M0 = 0;   //����Ϊ׼˫���
//	P2PU =0x3C;          //����
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@�������ƣ�void LQ_SPI_Read(unsigned char reg, unsigned short len, unsigned char* buf)
@����˵����SPI���豸��ȡ����
@����˵����unsigned char reg,�豸��ʼ��ַunsigned char* buf,���ݴ�ŵ�ַunsigned short len,��д������ݳ���
@�������أ���
@�޸�ʱ�䣺2022/02/24
@���÷�����SPI_Read_Nbytes(SPI1,reg|0x80,buf,len);
@��    ע����
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
@�������ƣ�void LQ_SPI_Write(unsigned char reg, unsigned short len, unsigned char* buf)
@����˵����SPI���豸д������
@����˵����unsigned char reg,�豸��ʼ��ַunsigned char* buf,���ݴ�ŵ�ַunsigned short len,��д������ݳ���
@�������أ���
@�޸�ʱ�䣺2022/02/24
@���÷�������
@��    ע����
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void LQ_SPI_Write(unsigned char reg, unsigned char value)
{
    unsigned char buff[2];

    buff[0] = reg & 0x7f;          //�ȷ��ͼĴ���
    buff[1] = value;        //�ٷ�������
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
    //���ٶȼ�52HZ����б�Ǽ�⹦�ܹ�����26HZ����˼��ٶȼ�ODR��������Ϊ>=26hz��,2g���̡�
    LQ_SPI_Write(LSM6DSR_CTRL1_XL,0X20);
    //ʹ�ܼ��ٶȼ�x,y,z��
    LQ_SPI_Write(LSM6DSR_CTRL9_XL,0X38);
    //�����ǵ�ƽ���������ٶȼƸ�����ʹ��
    LQ_SPI_Write(LSM6DSR_CTRL6_C,0X40|0x10);
    //�����Ǹ�����ʹ��
    LQ_SPI_Write(LSM6DSR_CTRL7_G,0X80);
    //���ٶȼ�INT2����ʧ��,����������INT2ʹ��
    LQ_SPI_Write(LSM6DSR_INT2_CTRL,0X03);
    //������12.5hz  2000dps
    LQ_SPI_Write(LSM6DSR_CTRL2_G,0X1C);
    //ʹ��������x,y,z��
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
  * @brief    LQLSM6DSRSPI���Ժ���
  *
  * @param    
  *
  * @return   ��
  *
  * @note     ��
  *
  * @see      Test_SPI_LQLSM6DSR(void));
  * @see      
  *
  * @date     2023/4/10 ����һ
  */
void Test_SPI_LQLSM6DSR(void)
{
    signed short accelerat_X, accelerat_Y, accelerat_Z;
    signed short gyro_X, gyro_Y, gyro_Z;
    unsigned char res;
    char  txt[30];

    Soft_SPI_Init();

		delayms(500);

    OLED_Init();                //LCD��ʼ��
    
    OLED_CLS();                 //LCD����

    /*��ȡ�豸ID*/

    res = LQ_SPI_LSM60DSR_GetID(WHO_AM_LSM6D);

		OLED_P14x16Str(8,0,(unsigned char*)"�����������ܿƼ�");		//�ַ�����ʾ
		sprintf(txt,"ID:0X%2x",res);
		OLED_P6x8Str(70,2,txt);     //�ַ�����ʾ 
	
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



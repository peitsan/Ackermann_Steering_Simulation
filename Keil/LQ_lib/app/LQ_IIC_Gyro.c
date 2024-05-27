#include "include.h"

#define MPU_ADDR  0x68  //IICд��ʱ�ĵ�ַ�ֽ����ݣ�+1Ϊ��ȡ

unsigned char IIC_ICM20602 = 0;
unsigned char IIC_ICM20689 = 0;
unsigned char IIC_MPU9250 = 0;
unsigned char IIC_ICM42605 = 0;
unsigned char IIC_MPU6050 = 0;
unsigned char IIC_errorid = 0;

unsigned char Gyro_ID = 0;

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
unsigned char Gyro_Chose(void)
{
    Gyro_ID = MPU_Read_Byte(MPU_ADDR,WHO_AM_I);   //Gyro_ID = 0x68
    switch(Gyro_ID)
    {
        case 0x12:IIC_ICM20602 = 1;break;
        case 0x71:IIC_MPU9250 = 1;break;
        case 0x98:IIC_ICM20689 = 1;break;
        case 0x42:IIC_ICM42605 = 1;break;
        case 0x68:IIC_MPU6050 = 1;break;
        default :IIC_errorid = 1;return Gyro_ID;
    }
    return Gyro_ID;
}


signed short  aacx,aacy,aacz;            //���ٶȴ�����ԭʼ����
signed short  gyrox,gyroy,gyroz;         //������ԭʼ����
short magx, magy, magz;
/**
  * @brief    ��ȡMPU6050����ICM20602ԭʼ���� ����
  *
  * @param
  *
  * @return
  *
  * @note
  *
  * @see      Test_MPU6050();
  *
  * @date     2019/6/12 ������
  */
void Test_IIC_Gyro(void)
{
  unsigned char res;
	char  txt[64];
	unsigned char DATH[6], DATL[6];

	IIC_Init();                  //IIC��ʼ��//IIC��ʼ��
	delayms(100);
	res = Gyro_Chose();                //�ж��������ͺ�
	sprintf(txt,"ID:0X%2x",res);
	UART4_PutStr(txt);

	OLED_Init();                //LCD��ʼ��
	delayms(100);
	OLED_CLS();                 //LCD����
	OLED_P6x8Str(70,2,txt);     // �ַ�����ʾ
	if(IIC_ICM20602)
			OLED_P8x16Str(0,0,"LQ 20602 Test");// �ַ�����ʾ
	if(IIC_MPU6050)
			OLED_P8x16Str(0,0,"LQ 6050 Test");// �ַ�����ʾ
	if(IIC_ICM42605)
			OLED_P8x16Str(0,0,"LQ 42605 Test");// �ַ�����ʾ
	if(IIC_MPU9250)
			OLED_P8x16Str(0,0,"LQ 9250 Test");// �ַ�����ʾ
	if(IIC_ICM20689)
			OLED_P8x16Str(0,0,"LQ 20689 Test");// �ַ�����ʾ
	if(IIC_errorid)
	{
			OLED_P8x16Str(0,0,"Gyro Test fail");// �ַ�����ʾ
			while(1);
	}
		
	MPU6050_Init();
	while(1)
    {
			//�õ����ٶȴ���������
			DATH[0] = MPU_Read_Byte(MPU_ADDR,0x3B);DATL[0] = MPU_Read_Byte(MPU_ADDR,0x3C);
			DATH[1] = MPU_Read_Byte(MPU_ADDR,0x3D);DATL[1] = MPU_Read_Byte(MPU_ADDR,0x3E);
			DATH[2] = MPU_Read_Byte(MPU_ADDR,0x3F);DATL[2] = MPU_Read_Byte(MPU_ADDR,0x40);
			DATH[3] = MPU_Read_Byte(MPU_ADDR,0x43);DATL[3] = MPU_Read_Byte(MPU_ADDR,0x44);
			DATH[4] = MPU_Read_Byte(MPU_ADDR,0x45);DATL[4] = MPU_Read_Byte(MPU_ADDR,0x46);
			DATH[5] = MPU_Read_Byte(MPU_ADDR,0x47);DATL[5] = MPU_Read_Byte(MPU_ADDR,0x48);
			//����ת��
			aacx = DATH[0]; aacx = (aacx<<8)|DATL[0];
			aacy = DATH[1]; aacy = (aacy<<8)|DATL[1];
			aacz = DATH[2]; aacz = (aacz<<8)|DATL[2];
			gyrox = DATH[3]; gyrox = (gyrox<<8)|DATL[3];
			gyroy = DATH[4]; gyroy = (gyroy<<8)|DATL[4];
			gyroz = DATH[5]; gyroz = (gyroz<<8)|DATL[5];
			
	    if (IIC_MPU9250)
			{
				DATH[0] = MPU_Read_Byte(MPU_ADDR,0X03);DATL[0] = MPU_Read_Byte(MPU_ADDR,0X04);
				DATH[1] = MPU_Read_Byte(MPU_ADDR,0X05);DATL[1] = MPU_Read_Byte(MPU_ADDR,0X06);
				DATH[2] = MPU_Read_Byte(MPU_ADDR,0X07);DATL[2] = MPU_Read_Byte(MPU_ADDR,0X08);
				magx = DATH[0]; magx = (magx<<8)|DATL[0];
				magy = DATH[1]; magy = (magy<<8)|DATL[1];
				magz = DATH[2]; magz = (magz<<8)|DATL[2];
				sprintf((char*)txt,"gx:%06d",gyrox);
				OLED_P6x8Str(70,5,txt);
				sprintf((char*)txt,"gy:%06d",gyroy);
				OLED_P6x8Str(70,6,txt);
				sprintf((char*)txt,"gz:%06d",gyroz);
				OLED_P6x8Str(70,7,txt);
			}
	       
	    sprintf((char*)txt,"ax:%06d",aacx);
			OLED_P6x8Str(0,2,txt);
			sprintf((char*)txt,"ay:%06d",aacy);
			OLED_P6x8Str(0,3,txt);
			sprintf((char*)txt,"az:%06d",aacz);
			OLED_P6x8Str(0,4,txt);
			sprintf((char*)txt,"gx:%06d",gyrox);
			OLED_P6x8Str(0,5,txt);
			sprintf((char*)txt,"gy:%06d",gyroy);
			OLED_P6x8Str(0,6,txt);
			sprintf((char*)txt,"gz:%06d",gyroz);
			OLED_P6x8Str(0,7,txt);
//			ANO_DT_send_int16byte16(aacx,aacy,aacz,gyrox,gyroy,gyroz,gyroz,gyroz);
	    delayms(100);
    }

}




//****************************************************************************************
//
//
//
//
/**
  * @brief    ��ȡMPU6050�������� ����
  *
  * @param
  *
  * @return
  *
  * @note
  *
  * @see      Test_MPU6050();
  *
  * @date     2019/6/12 ������
  */
void IIC_Gyro(void)
{
	unsigned char res;
	char  txt[64];
	unsigned char DATH[6], DATL[6];

	IIC_Init();                  //IIC��ʼ��//IIC��ʼ��
	delayms(100);
	res = Gyro_Chose();                //�ж��������ͺ�
	sprintf(txt,"ID:0X%2x",res);

	OLED_Init();                //LCD��ʼ��
	delayms(100);
	OLED_CLS();                 //LCD����
	OLED_P6x8Str(70,2,txt);     // �ַ�����ʾ
	if(IIC_MPU6050)
			OLED_P8x16Str(0,0,"LQ 6050");// �ַ�����ʾ
	if(IIC_errorid)
	{
			OLED_P8x16Str(0,0,"Gyro Test fail");// �ַ�����ʾ
			while(1);
	}
		
	MPU6050_Init();
	while(1)
    {
		//�õ����ٶȴ���������
		DATH[0] = MPU_Read_Byte(MPU_ADDR,0x3B);DATL[0] = MPU_Read_Byte(MPU_ADDR,0x3C);
		DATH[1] = MPU_Read_Byte(MPU_ADDR,0x3D);DATL[1] = MPU_Read_Byte(MPU_ADDR,0x3E);
		DATH[2] = MPU_Read_Byte(MPU_ADDR,0x3F);DATL[2] = MPU_Read_Byte(MPU_ADDR,0x40);
		DATH[3] = MPU_Read_Byte(MPU_ADDR,0x43);DATL[3] = MPU_Read_Byte(MPU_ADDR,0x44);
		DATH[4] = MPU_Read_Byte(MPU_ADDR,0x45);DATL[4] = MPU_Read_Byte(MPU_ADDR,0x46);
		DATH[5] = MPU_Read_Byte(MPU_ADDR,0x47);DATL[5] = MPU_Read_Byte(MPU_ADDR,0x48);
		//����ת��
		aacx = DATH[0]; aacx = (aacx<<8)|DATL[0];
		aacy = DATH[1]; aacy = (aacy<<8)|DATL[1];
		aacz = DATH[2]; aacz = (aacz<<8)|DATL[2];
		gyrox = DATH[3]; gyrox = (gyrox<<8)|DATL[3];
		gyroy = DATH[4]; gyroy = (gyroy<<8)|DATL[4];
		gyroz = DATH[5]; gyroz = (gyroz<<8)|DATL[5];
		
		sprintf((char*)txt,"ax:%06d",aacx);
		OLED_P6x8Str(0,2,txt);
		sprintf((char*)txt,"ay:%06d",aacy);
		OLED_P6x8Str(0,3,txt);
		sprintf((char*)txt,"az:%06d",aacz);
		OLED_P6x8Str(0,4,txt);
		sprintf((char*)txt,"gx:%06d",gyrox);
		OLED_P6x8Str(0,5,txt);
		sprintf((char*)txt,"gy:%06d",gyroy);
		OLED_P6x8Str(0,6,txt);
		sprintf((char*)txt,"gz:%06d",gyroz);
		OLED_P6x8Str(0,7,txt);
//		ANO_DT_send_int16byte16(aacx,aacy,aacz,gyrox,gyroy,gyroz,gyroz,gyroz);
	    delayms(100);
    }

}

/**
  * @brief    ����ȷ��ʱ
  *
  * @param    ��
  *
  * @return   ��
  *
  * @note     ��
  *
  * @see      delayms_mpu(100);
  *
  * @date     2019/4/22 ����һ
*/
void delayms_mpu(uint16 ms)
{
	while(ms--)
	{
		uint16  i = 300*50;
		while(i--)
		{
			_nop_();
		}
	}
}

/**
  * @brief    ��ʼ��MPU6050 ���� ICM20602
  *
  * @param    ��
  *
  * @return   0����ʼ���ɹ�   1��ʧ��
  *
  * @note     ʹ��ǰ�ȳ�ʼ��IIC�ӿ�
  *
  * @see      MPU6050_Init();
  *
  * @date     2019/6/12 ������
  */
unsigned char MPU6050_Init(void)
{
    int  res;
    if (IIC_ICM42605)
    {
//        MPU_Write_Byte(MPU_ADDR, device_config_reg,bit_soft_reset_chip_config);//chip soft reset
//        delayms(10);
//
//            MPU_Write_Byte(MPU_ADDR,reg_bank_sel,0x01);//Change to bank 1
//            MPU_Write_Byte(MPU_ADDR,intf_config4,0x03);//4 wire spi mode  ����SPI  Ĭ��ֵ0x03
//            MPU_Write_Byte(MPU_ADDR,reg_bank_sel,0x00);
//            MPU_Write_Byte(MPU_ADDR,fifo_config_reg,0x40);//Stream-to-FIFO Mode  ������Y���ݲ��˲�����Ƶ��ѡ��
//
//            res = MPU_Read_Byte(MPU_ADDR,int_source0_reg);
//            MPU_Write_Byte(MPU_ADDR,int_source0_reg,0x00);
//            MPU_Write_Byte(MPU_ADDR,fifo_config2_reg,0x00);// watermark
//            MPU_Write_Byte(MPU_ADDR,fifo_config3_reg,0x02);// watermark
//            MPU_Write_Byte(MPU_ADDR,int_source0_reg, (unsigned char)res);
//            MPU_Write_Byte(MPU_ADDR,fifo_config1_reg,0x63);// Enable the accel and gyro to the FIFO
//
//            MPU_Write_Byte(MPU_ADDR,reg_bank_sel,0x00);
//            MPU_Write_Byte(MPU_ADDR,int_config_reg,0x36);
//
//            MPU_Write_Byte(MPU_ADDR,reg_bank_sel, 0x00);
//            res = (MPU_Read_Byte(MPU_ADDR,int_source0_reg)|bit_int_fifo_ths_int1_en);
//            MPU_Write_Byte(MPU_ADDR,int_source0_reg, (unsigned char)res);
//
//            MPU_Write_Byte(MPU_ADDR,reg_bank_sel, 0x00);
//            res = ((MPU_Read_Byte(MPU_ADDR,accel_config0_reg)&0x1F)|(bit_accel_ui_fs_sel_8g));//8g
//            MPU_Write_Byte(MPU_ADDR,accel_config0_reg, (unsigned char)res);
//
//            MPU_Write_Byte(MPU_ADDR,reg_bank_sel, 0x00);
//            res = ((MPU_Read_Byte(MPU_ADDR,accel_config0_reg)&0xF0)|bit_accel_odr_50hz);
//            MPU_Write_Byte(MPU_ADDR,accel_config0_reg, (unsigned char)res);
//
//            MPU_Write_Byte(MPU_ADDR,reg_bank_sel, 0x00);
//            res = ((MPU_Read_Byte(MPU_ADDR,gyro_config0_reg)&0x1F)|(bit_gyro_ui_fs_sel_1000dps));
//            MPU_Write_Byte(MPU_ADDR,gyro_config0_reg,(unsigned char)res);
//
//            MPU_Write_Byte(MPU_ADDR,reg_bank_sel, 0x00);
//            res = ((MPU_Read_Byte(MPU_ADDR,gyro_config0_reg)&0xF0)|bit_gyro_odr_50hz);
//            MPU_Write_Byte(MPU_ADDR,gyro_config0_reg, (unsigned char)res);
//
//            MPU_Write_Byte(MPU_ADDR,reg_bank_sel, 0x00);
//            res = MPU_Read_Byte(MPU_ADDR,pwr_mgmt0_reg)|(bit_accel_mode_ln); // Accel on in LNM
//            MPU_Write_Byte(MPU_ADDR,pwr_mgmt0_reg, (unsigned char)res);
//            delayms(1);
//
//            MPU_Write_Byte(MPU_ADDR,reg_bank_sel, 0x00);
//            res = MPU_Read_Byte(MPU_ADDR,pwr_mgmt0_reg)|(bit_gyro_mode_ln); // Gyro on in LNM
//            MPU_Write_Byte(MPU_ADDR,pwr_mgmt0_reg,(unsigned char)res);
            delayms(1);
            return 0;
    }
    else
    {
        res = MPU_Read_Byte(MPU_ADDR,WHO_AM_I);           //��ȡMPU6050��ID
        if(res != Gyro_ID)                                 //����ID��ȷ
            return 1;

        res = 0;
        res += MPU_Write_Byte(MPU_ADDR,MPU_PWR_MGMT1_REG,0X80);//��λMPU6050
        delayms_mpu(100);  //��ʱ100ms
        res += MPU_Write_Byte(MPU_ADDR,MPU_PWR_MGMT1_REG,0X00);//����MPU6050
        res += MPU_Set_Gyro_Fsr(3);					        	   //�����Ǵ�����,��2000dps
        res += MPU_Set_Accel_Fsr(1);					       	   //���ٶȴ�����,��4g
        res += MPU_Set_Rate(1000);						       	   //���ò�����1000Hz
        res += MPU_Write_Byte(MPU_ADDR,MPU_CFG_REG,0x02);      //�������ֵ�ͨ�˲���   98hz
        res += MPU_Write_Byte(MPU_ADDR,MPU_INT_EN_REG,0X00);   //�ر������ж�
        res += MPU_Write_Byte(MPU_ADDR,MPU_USER_CTRL_REG,0X00);//I2C��ģʽ�ر�
        res += MPU_Write_Byte(MPU_ADDR,MPU_PWR_MGMT1_REG,0X01);//����CLKSEL,PLL X��Ϊ�ο�
        res += MPU_Write_Byte(MPU_ADDR,MPU_PWR_MGMT2_REG,0X00);//���ٶ��������Ƕ�����

        return 0;
    }
}

/**
  * @brief    ���������ǲ�����Χ
  *
  * @param    fsr:0,��250dps;1,��500dps;2,��1000dps;3,��2000dps
  *
  * @return   0 �����óɹ�
  *
  * @note     ��
  *
  * @see      MPU_Set_Gyro_Fsr(3);		  //�����Ǵ�����,��2000dps
  *
  * @date     2019/6/12 ������
  */
unsigned char MPU_Set_Gyro_Fsr(unsigned char fsr)
{
	return MPU_Write_Byte(MPU_ADDR,MPU_GYRO_CFG_REG,fsr<<3);
}

/**
  * @brief    ���ü��ٶȼƲ�����Χ
  *
  * @param    fsr:0,��2g;1,��4g;2,��8g;3,��16g
  *
  * @return   0�����óɹ�
  *
  * @note     ��
  *
  * @see      MPU_Set_Accel_Fsr(1);		  //���ٶȴ�����,��4g
  *
  * @date     2019/6/12 ������
  */
unsigned char MPU_Set_Accel_Fsr(unsigned char fsr)
{
	return MPU_Write_Byte(MPU_ADDR,MPU_ACCEL_CFG_REG,fsr<<3);
}

/**
  * @brief    �������ֵ�ͨ�˲�
  *
  * @param    lpf:���ֵ�ͨ�˲�Ƶ��(Hz)
  *
  * @return   0�����óɹ�
  *
  * @note     ��
  *
  * @see      MPU_Set_LPF(100);
  *
  * @date     2019/6/12 ������
  */
unsigned char MPU_Set_LPF(uint16 lpf)
{
	unsigned char  dat=0;
	if(lpf>=188)dat=1;
	else if(lpf>=98)dat=2;
	else if(lpf>=42)dat=3;
	else if(lpf>=20)dat=4;
	else if(lpf>=10)dat=5;
	else dat=6;
	return MPU_Write_Byte(MPU_ADDR,MPU_CFG_REG,dat);//�������ֵ�ͨ�˲���
}

/**
  * @brief    ���ò�����
  *
  * @param    rate:4~1000(Hz)
  *
  * @return   0�����óɹ�
  *
  * @note     ��
  *
  * @see      MPU_Set_Rate(1000);       	   //���ò�����1000Hz
  *
  * @date     2019/6/12 ������
  */
unsigned char MPU_Set_Rate(uint16 rate)
{
	unsigned char  dat;
	if(rate>1000)rate=1000;
	if(rate<4)rate=4;
	dat=1000/rate-1;
	MPU_Write_Byte(MPU_ADDR,MPU_SAMPLE_RATE_REG,dat);	    //�������ֵ�ͨ�˲���
 	return MPU_Set_LPF(rate/2);	                                //�Զ�����LPFΪ�����ʵ�һ��
}

/**
  * @brief    ��ȡ�¶�ֵ
  *
  * @param    ��
  *
  * @return   �¶�ֵ(������100��)
  *
  * @note     ��
  *
  * @see      signed short temp = MPU_Get_Temperature();
  *
  * @date     2019/6/12 ������
  */
short MPU_Get_Temperature(void)
{
	unsigned char  buf[2];
	short raw;
	float temp;
	MPU_Read_Len(MPU_ADDR,MPU_TEMP_OUTH_REG,2,buf);
	raw=((uint16)buf[0]<<8)|buf[1];
	temp=21+((double)raw)/333.87;
	return (short)temp*100;
}

/**
  * @brief    ��ȡ������ֵ
  *
  * @param    gx,gy,gz:������x,y,z���ԭʼ����(������)
  *
  * @return   0����ȡ�ɹ�
  *
  * @note     ��
  *
  * @see      signed short data[3];
  * @see      MPU_Get_Gyroscope(&data[0], &data[1], &data[2]);
  *
  * @date     2019/6/12 ������
  */
unsigned char MPU_Get_Gyroscope(signed short *gx,signed short *gy,signed short *gz)
{
	unsigned char  buf[6],res;
	res=MPU_Read_Len(MPU_ADDR,MPU_GYRO_XOUTH_REG,6,buf);
	if(res==0)
	{
		*gx=((uint16)buf[0]<<8)|buf[1];
		*gy=((uint16)buf[2]<<8)|buf[3];
		*gz=((uint16)buf[4]<<8)|buf[5];
	}
	return res;
}

/**
  * @brief    ��ȡ���ٶ�ֵ
  *
  * @param    ax,ay,az:������x,y,z���ԭʼ����(������)
  *
  * @return   0����ȡ�ɹ�
  *
  * @note     ��
  *
  * @see      signed short data[3];
  * @see      MPU_Get_Accelerometer(&data[0], &data[1], &data[2]);
  *
  * @date     2019/6/12 ������
  */
unsigned char MPU_Get_Accelerometer(signed short *ax,signed short *ay,signed short *az)
{
	unsigned char  buf[6],res;
	res=MPU_Read_Len(MPU_ADDR,MPU_ACCEL_XOUTH_REG,6,buf);
	if(res==0)
	{
		*ax=((uint16)buf[0]<<8)|buf[1];
		*ay=((uint16)buf[2]<<8)|buf[3];
		*az=((uint16)buf[4]<<8)|buf[5];
	}
	return res;
}

/**
  * @brief    ��ȡ ���ٶ�ֵ ���ٶ�ֵ
  *
  * @param    ax,ay,az:������x,y,z���ԭʼ����(������)
  * @param    gx,gy,gz:������x,y,z���ԭʼ����(������)
  *
  * @return   0����ȡ�ɹ�
  *
  * @note     ��
  *
  * @see      signed short data[6];
  * @see      ICM_Get_Raw_data(&data[0], &data[1], &data[2],&data[3], &data[4], &data[5]);
  *
  */
unsigned char MPU_Get_Raw_data(signed short *ax,signed short *ay,signed short *az,signed short *gx,signed short *gy,signed short *gz)
{
	unsigned char  buf[14],res;
	if(IIC_ICM42605)
	{
		res=MPU_Read_Len(MPU_ADDR,0X1F,12,buf);
		if(res==0)
		{
			*ax=((uint16)buf[0]<<8)|buf[1];
			*ay=((uint16)buf[2]<<8)|buf[3];
			*az=((uint16)buf[4]<<8)|buf[5];
			*gx=((uint16)buf[6]<<8)|buf[7];
			*gy=((uint16)buf[8]<<8)|buf[9];
			*gz=((uint16)buf[10]<<8)|buf[11];
		}
		return res;
	}
	else
	{
		res=MPU_Read_Len(MPU_ADDR,MPU_ACCEL_XOUTH_REG,14,buf);
		if(res==0)
		{
			*ax=((uint16)buf[0]<<8)|buf[1];
			*ay=((uint16)buf[2]<<8)|buf[3];
			*az=((uint16)buf[4]<<8)|buf[5];
			*gx=((uint16)buf[8]<<8)|buf[9];
			*gy=((uint16)buf[10]<<8)|buf[11];
			*gz=((uint16)buf[11]<<8)|buf[13];
		}
		return res;
	}
}

/**
* @brief    ��ȡ������ֵ
*
* @param    mx,my,mz:��������x,y,z���ԭʼ����(������)
*
* @return   0����ȡ�ɹ�
*
* @note     ��
*
* @see      signed short data[3];
* @see      MPU9250_Get_Magnetometer(&data[0], &data[1], &data[2]);
*
* @date     2020/12/15 ���ڶ�
*/
unsigned char MPU9250_Get_Magnetometer(short *mx,short *my,short *mz)
{

  unsigned char  buf[6],res;
  res=MPU_Read_Len(AK8963_ADDR,MPU_MAG_XOUTL_REG,6,buf);
  if(res==0)
  {
    *mx=((uint16)buf[1]<<8)|buf[0];
    *my=((uint16)buf[3]<<8)|buf[2];
    *mz=((uint16)buf[5]<<8)|buf[4];
  }
  MPU_Read_Byte(AK8963_ADDR,MPU_MAG_ST2_REG);
  return res;
}

/**
  * @brief    IIC ������
  *
  * @param    addr:������ַ
  * @param    reg :Ҫ��ȡ�ļĴ�����ַ
  * @param    len :Ҫ��ȡ�ĳ���
  * @param    buf :��ȡ�������ݴ洢��
  *
  * @return   0 ����ȡ�ɹ�
  *
  * @note     �ײ����� ��ֲʱ��Ҫ�޸�
  *
  * @see      unsigned char buf[14];
  * @see      MPU_Read_Len(MPU_ADDR,MPU_ACCEL_XOUTH_REG,14,buf);
  *
  * @date     2019/6/12 ������
  */
unsigned char MPU_Read_Len(unsigned char addr,unsigned char reg,unsigned char len,unsigned char *buf)
{
	return IIC_ReadMultByteFromSlave(addr<<1, reg, len, buf);
}

/**
  * @brief    IIC ����д
  *
  * @param    addr:������ַ
  * @param    reg :Ҫд��ļĴ�����ַ
  * @param    len :Ҫд��ĳ���
  * @param    buf :д�뵽�����ݴ洢��
  *
  * @return   0 ��д��ɹ�
  *
  * @note     �ײ����� ��ֲʱ��Ҫ�޸�
  *
  * @see      unsigned char buf[14];
  * @see      MPU9250_Write_Len(MPU9250_ADDR,MPU_ACCEL_XOUTH_REG,14,buf);
  *
  * @date     2021/7/1 ������
  */
unsigned char MPU_Write_Len(unsigned char addr,unsigned char reg,unsigned char len,unsigned char *buf)
{
	return IIC_WriteMultByteToSlave(addr<<1, reg, len, buf);
}

/**
  * @brief    IIC дһ���Ĵ���
  *
  * @param    addr  :������ַ
  * @param    reg   :�Ĵ�����ַ
  * @param    value :Ҫд���ֵ
  *
  * @return   0 ����ȡ�ɹ�
  *
  * @note     �ײ����� ��ֲʱ��Ҫ�޸�
  *
  * @see      MPU_Write_Byte(MPU_ADDR,MPU_SAMPLE_RATE_REG,1);
  *
  * @date     2019/6/12 ������
  */
unsigned char MPU_Write_Byte(unsigned char addr,unsigned char reg,unsigned char value)
{
	return IIC_WriteByteToSlave(addr<<1, reg, value);
}

/**
  * @brief    IIC ��һ���Ĵ���
  *
  * @param    addr  :������ַ
  * @param    reg   :�Ĵ�����ַ
  *
  * @return   ��ȡ��ֵ
  *
  * @note     �ײ����� ��ֲʱ��Ҫ�޸�
  *
  * @see      MPU_Read_Byte(MPU_ADDR,WHO_AM_I);
  *
  * @date     2019/6/12 ������
  */
unsigned char MPU_Read_Byte(unsigned char addr,unsigned char reg)
{
	unsigned char value[1];
	MPU_Read_Len(addr, reg, 1, value);
	return value[0];
}
/**
  * @brief    LQLSM6DSRIIC��ȡID����
  *
  * @param    
  *
  *
  * @note     ��
  *
  * @see      res = LQ_IIC_LSM60DSR_GetID(LSM6DSR_ADDR, WHO_AM_LSM6D); 
  * @see      
  *
  * @date     2023/4/10 ����һ
  */
unsigned char LQ_IIC_LSM60DSR_GetID(unsigned char dev, unsigned char reg)
{
    unsigned char ID[1];
    IIC_ReadMultByteFromSlave(dev, reg, 1, ID);
    return ID[0];
}
/**
  * @brief    LQLSM6DSRIIC��ʼ������
  *
  * @param    
  *
  *
  * @note     ��
  *
  * @see      LQ_IIC_LSM60DSR_Init();
  * @see      
  *
  * @date     2023/4/10 ����һ
  */
void LQ_IIC_LSM60DSR_Init(void)
{
    //���ٶȼ�52HZ����б�Ǽ�⹦�ܹ�����26HZ����˼��ٶȼ�ODR��������Ϊ>=26hz��,2g���̡�
    IIC_WriteByteToSlave(LSM6DSR_ADDR,LSM6DSR_CTRL1_XL,0X20);
    //ʹ�ܼ��ٶȼ�x,y,z��
    IIC_WriteByteToSlave(LSM6DSR_ADDR,LSM6DSR_CTRL9_XL,0X38);
    //�����ǵ�ƽ���������ٶȼƸ�����ʹ��
    IIC_WriteByteToSlave(LSM6DSR_ADDR,LSM6DSR_CTRL6_C,0X40|0x10);
    //�����Ǹ�����ʹ��
    IIC_WriteByteToSlave(LSM6DSR_ADDR,LSM6DSR_CTRL7_G,0X80);
    //���ٶȼ�INT2����ʧ��,����������INT2ʹ��
    IIC_WriteByteToSlave(LSM6DSR_ADDR,LSM6DSR_INT2_CTRL,0X03);
    //������12.5hz  2000dps
    IIC_WriteByteToSlave(LSM6DSR_ADDR,LSM6DSR_CTRL2_G,0X1C);
    //ʹ��������x,y,z��
    IIC_WriteByteToSlave(LSM6DSR_ADDR,LSM6DSR_CTRL10_C,0X38);

//    //WAKE_UP INTERRUPT Configuration
//    MPU_Write_Byte(LSM6DSR_ADDR, LSM6DSR_CTRL1_XL,0x60);
//    MPU_Write_Byte(TAP_CFG,0x90);
//    MPU_Write_Byte(WAKE_UP_DUR,0x00);
//    MPU_Write_Byte(WAKE_UP_THS,0x02);
//    MPU_Write_Byte(MD1_CFG,0x20);
//    //6D Orientation Configuration
//    MPU_Write_Byte(TAP_THS_6D,0x40);
//    MPU_Write_Byte(CTRL8_XL,0x01);
}
/**
  * @brief    LQLSM6DSRIIC��ȡ����
  *
  * @param    
  *
  * @param    ax,ay,az:������x,y,z���ԭʼ����(������)
  * @param    gx,gy,gz:������x,y,z���ԭʼ����(������)
  *
  * @note     ��
  *
  * @see      LQ_IIC_LSM60DSR_Read(&accelerat_X, &accelerat_Y, &accelerat_Z, &gyro_X, &gyro_Y, &gyro_Z);
  * @see      
  *
  * @date     2023/4/10 ����һ
  */
void LQ_IIC_LSM60DSR_Read(signed short *ax,signed short *ay,signed short *az,signed short *gx,signed short *gy,signed short *gz)
{
   unsigned char buf_L[1], buf_H[1];
    IIC_ReadMultByteFromSlave(LSM6DSR_ADDR, LSM6DSR_OUTX_L_GYRO, 1, buf_L);
  IIC_ReadMultByteFromSlave(LSM6DSR_ADDR, LSM6DSR_OUTX_H_GYRO, 1, buf_H);
    *gx=((unsigned short )buf_H[0]<<8)|buf_L[0];
    IIC_ReadMultByteFromSlave(LSM6DSR_ADDR, LSM6DSR_OUTY_L_GYRO, 1, buf_L);
  IIC_ReadMultByteFromSlave(LSM6DSR_ADDR, LSM6DSR_OUTY_H_GYRO, 1, buf_H);
    *gy=((unsigned short )buf_H[0]<<8)|buf_L[0];
    IIC_ReadMultByteFromSlave(LSM6DSR_ADDR, LSM6DSR_OUTZ_L_GYRO, 1, buf_L);
  IIC_ReadMultByteFromSlave(LSM6DSR_ADDR, LSM6DSR_OUTZ_H_GYRO, 1, buf_H);
    *gz=((unsigned short )buf_H[0]<<8)|buf_L[0];
    IIC_ReadMultByteFromSlave(LSM6DSR_ADDR, LSM6DSR_OUTX_L_ACC, 1, buf_L);
  IIC_ReadMultByteFromSlave(LSM6DSR_ADDR, LSM6DSR_OUTX_H_ACC, 1, buf_H);
    *ax=((unsigned short )buf_H[0]<<8)|buf_L[0];
    IIC_ReadMultByteFromSlave(LSM6DSR_ADDR, LSM6DSR_OUTY_L_ACC, 1, buf_L);
  IIC_ReadMultByteFromSlave(LSM6DSR_ADDR, LSM6DSR_OUTY_H_ACC, 1, buf_H);
    *ay=((unsigned short )buf_H[0]<<8)|buf_L[0];
    IIC_ReadMultByteFromSlave(LSM6DSR_ADDR, LSM6DSR_OUTZ_L_ACC, 1, buf_L);
  IIC_ReadMultByteFromSlave(LSM6DSR_ADDR, LSM6DSR_OUTZ_H_ACC, 1, buf_H);
    *az=((unsigned short )buf_H[0]<<8)|buf_L[0];
}
/**
  * @brief    LQLSM6DSRIIC���Ժ���
  *
  * @param    
  *
  * @return   ��
  *
  * @note     ��
  *
  * @see      Test_IIC_LQLSM6DSR();
  * @see      
  *
  * @date     2023/4/10 ����һ
  */
void Test_IIC_LQLSM6DSR(void)
{
    signed short accelerat_X, accelerat_Y, accelerat_Z;
    signed short gyro_X, gyro_Y, gyro_Z;
    unsigned char res;
    char  txt[30];

    IIC_Init();                  //IIC��ʼ��//IIC��ʼ��
    delayms(100);


    

    OLED_Init();                //LCD��ʼ��
    delayms(100);
    OLED_CLS();                 //LCD����

    /*��ȡ�豸ID*/
    res = LQ_IIC_LSM60DSR_GetID(LSM6DSR_ADDR, WHO_AM_LSM6D); 
	  
   OLED_P14x16Str(8,0,(unsigned char*)"�����������ܿƼ�");		//�ַ�����ʾ
	 sprintf(txt,"ID:0X%2x",res);
   OLED_P6x8Str(70,2,txt);     // �ַ�����ʾ      
    
   LQ_IIC_LSM60DSR_Init();
    while(1)
    {
        LQ_IIC_LSM60DSR_Read(&accelerat_X, &accelerat_Y, &accelerat_Z, &gyro_X, &gyro_Y, &gyro_Z);
/*��Ļ��ʾѡ��*/

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
        delayms(10);
    }
}


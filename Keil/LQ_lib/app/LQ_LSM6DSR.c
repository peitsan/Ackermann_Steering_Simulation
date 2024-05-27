//#include "include.h"
//#define OLED  0                 //选择屏幕0：TFT屏      1:OLED屏

//unsigned char LQ_IIC_LSM60DSR_GetID(unsigned char dev, unsigned char reg)
//{
//    unsigned char ID[1];
//    IIC_ReadMultByteFromSlave(dev, reg, 1, ID);
//    return ID[0];
//}

//void LQ_IIC_LSM60DSR_Init(void)
//{
//    //加速度计52HZ（倾斜角检测功能工作在26HZ，因此加速度计ODR必须设置为>=26hz）,2g量程。
//    IIC_WriteByteToSlave(LSM6DSR_ADDR,LSM6DSR_CTRL1_XL,0X20);
//    //使能加速度计x,y,z轴
//    IIC_WriteByteToSlave(LSM6DSR_ADDR,LSM6DSR_CTRL9_XL,0X38);
//    //陀螺仪电平触发，加速度计高性能使能
//    IIC_WriteByteToSlave(LSM6DSR_ADDR,LSM6DSR_CTRL6_C,0X40|0x10);
//    //陀螺仪高性能使能
//    IIC_WriteByteToSlave(LSM6DSR_ADDR,LSM6DSR_CTRL7_G,0X80);
//    //加速度计INT2引脚失能,陀螺仪数据INT2使能
//    IIC_WriteByteToSlave(LSM6DSR_ADDR,LSM6DSR_INT2_CTRL,0X03);
//    //陀螺仪12.5hz  2000dps
//    IIC_WriteByteToSlave(LSM6DSR_ADDR,LSM6DSR_CTRL2_G,0X1C);
//    //使能陀螺仪x,y,z轴
//    IIC_WriteByteToSlave(LSM6DSR_ADDR,LSM6DSR_CTRL10_C,0X38);

////    //WAKE_UP INTERRUPT Configuration
////    MPU_Write_Byte(LSM6DSR_ADDR, LSM6DSR_CTRL1_XL,0x60);
////    MPU_Write_Byte(TAP_CFG,0x90);
////    MPU_Write_Byte(WAKE_UP_DUR,0x00);
////    MPU_Write_Byte(WAKE_UP_THS,0x02);
////    MPU_Write_Byte(MD1_CFG,0x20);
////    //6D Orientation Configuration
////    MPU_Write_Byte(TAP_THS_6D,0x40);
////    MPU_Write_Byte(CTRL8_XL,0x01);
//}

//void LQ_IIC_LSM60DSR_Read(signed short *ax,signed short *ay,signed short *az,signed short *gx,signed short *gy,signed short *gz)
//{
//    unsigned char buf[12];
//    IIC_ReadMultByteFromSlave(LSM6DSR_ADDR, LSM6DSR_OUTX_L_GYRO, 12, buf);
//    *ax=((uint16)buf[1]<<8)|buf[0];
//    *ay=((uint16)buf[3]<<8)|buf[2];
//    *az=((uint16)buf[5]<<8)|buf[4];
//    *gx=((uint16)buf[7]<<8)|buf[6];
//    *gy=((uint16)buf[9]<<8)|buf[8];
//    *gz=((uint16)buf[11]<<8)|buf[10];
//}

//void Test_IIC_LQLSM6DSR(void)
//{
//    signed short accelerat_X, accelerat_Y, accelerat_Z;
//    signed short gyro_X, gyro_Y, gyro_Z;
//    unsigned char res;
//    char  txt[30];

//    IIC_Init();                  //IIC初始化//IIC初始化
////    delayms(100);


//    /*屏幕显示选择*/

//    OLED_Init();                //LCD初始化
//    delayms(100);
//    OLED_CLS();                 //LCD清屏

//    /*读取设备ID*/
//    res = LQ_IIC_LSM60DSR_GetID(LSM6DSR_ADDR, WHO_AM_LSM6D);

//    /*屏幕显示选择*/
//    OLED_P8x16Str(0,0,"LQ LSM6DSR Test");// 字符串显示
//    sprintf(txt,"0X%2X",res);
//    OLED_P6x8Str(96, 4, "ID:");       //显示字符串
//    OLED_P6x8Str(97, 6, txt);       //显示字符串


//    LQ_IIC_LSM60DSR_Init();
//    while(1)
//    {
//        LQ_IIC_LSM60DSR_Read(&accelerat_X, &accelerat_Y, &accelerat_Z, &gyro_X, &gyro_Y, &gyro_Z);
///*屏幕显示选择*/

//        sprintf((char*)txt,"ax:%06d",accelerat_X);
//        OLED_P6x8Str(0,2,txt);
//        sprintf((char*)txt,"ay:%06d",accelerat_Y);
//        OLED_P6x8Str(0,3,txt);
//        sprintf((char*)txt,"az:%06d",accelerat_Z);
//        OLED_P6x8Str(0,4,txt);
//        sprintf((char*)txt,"gx:%06d",gyro_X);
//        OLED_P6x8Str(0,5,txt);
//        sprintf((char*)txt,"gy:%06d",gyro_Y);
//        OLED_P6x8Str(0,6,txt);
//        sprintf((char*)txt,"gz:%06d",gyro_Z);
//        OLED_P6x8Str(0,7,txt);



//        delayms(10);
//    }
//}

///*************************************************************************************************************************************
// *以下为SPI通信模式
// * ************************************************************************************************************************************/

//#define USE_SOFT_SPI    //用软件SPI取消注释 用硬件SPI请注释
//unsigned char LQ_SPI_LSM60DSR_GetID(unsigned char reg)
//{
//#ifdef USE_SOFT_SPI
//    unsigned char ID[1];
//    LQ_SPI_Read(reg, 1, ID);
//    return ID[0];
//#else
//    unsigned char ID[2];
//    ID[0] = reg|0x80;
//    QSPI_ReadWriteNByte(QSPI2, ID, ID, 2);
//    return ID[1];
//#endif
//}

//void LQ_SPI_LSM60DSR_Init(void)
//{
//#ifdef USE_SOFT_SPI
//    //加速度计52HZ（倾斜角检测功能工作在26HZ，因此加速度计ODR必须设置为>=26hz）,2g量程。
//    LQ_SPI_Write(LSM6DSR_CTRL1_XL,0X20);
//    //使能加速度计x,y,z轴
//    LQ_SPI_Write(LSM6DSR_CTRL9_XL,0X38);
//    //陀螺仪电平触发，加速度计高性能使能
//    LQ_SPI_Write(LSM6DSR_CTRL6_C,0X40|0x10);
//    //陀螺仪高性能使能
//    LQ_SPI_Write(LSM6DSR_CTRL7_G,0X80);
//    //加速度计INT2引脚失能,陀螺仪数据INT2使能
//    LQ_SPI_Write(LSM6DSR_INT2_CTRL,0X03);
//    //陀螺仪12.5hz  2000dps
//    LQ_SPI_Write(LSM6DSR_CTRL2_G,0X1C);
//    //使能陀螺仪x,y,z轴
//    LQ_SPI_Write(LSM6DSR_CTRL10_C,0X38);

//#else

//#endif

//}
//void LQ_SPI_LSM60DSR_Read(signed short *ax,signed short *ay,signed short *az,signed short *gx,signed short *gy,signed short *gz)
//{
//    unsigned char buf[12];
//#ifdef USE_SOFT_SPI
//    LQ_SPI_Read(LSM6DSR_OUTX_L_GYRO, 12, buf);
//#else

//#endif
//    *ax=((uint16)buf[1]<<8)|buf[0];
//    *ay=((uint16)buf[3]<<8)|buf[2];
//    *az=((uint16)buf[5]<<8)|buf[4];
//    *gx=((uint16)buf[7]<<8)|buf[6];
//    *gy=((uint16)buf[9]<<8)|buf[8];
//    *gz=((uint16)buf[11]<<8)|buf[10];
//}

//void Test_SPI_LQLSM6DSR(void)
//{
//    signed short accelerat_X, accelerat_Y, accelerat_Z;
//    signed short gyro_X, gyro_Y, gyro_Z;
//    unsigned char res;
//    char  txt[30];
//#ifdef USE_SOFT_SPI
//    Soft_SPI_Init();

//#else
//    QSPI_InitConfig(QSPI2_CLK_P15_8, QSPI2_MISO_P15_7, QSPI2_MOSI_P15_5, QSPI2_CS_P15_2, 5000000, 3);
//#endif

//    TFTSPI_Init(1);         //屏幕初始化为横屏
//    TFTSPI_CLS(u16BLACK);   // 清屏
//    TFTSPI_P8X16Str(2,0,"LQ LSM6DSR Test",u16RED,u16BLACK);// 字符串显示

//    /*读取设备ID*/
//    res = LQ_SPI_LSM60DSR_GetID(WHO_AM_LSM6D);

//    sprintf(txt,"0X%2X",res);
//    TFTSPI_P8X16Str(10,8,"ID:",u16RED,u16BLACK);
//    TFTSPI_P8X16Str(12,9,txt,u16RED,u16BLACK);// 字符串显示

//    LQ_SPI_LSM60DSR_Init();
//    while(1)
//    {
//        LQ_SPI_LSM60DSR_Read(&accelerat_X, &accelerat_Y, &accelerat_Z, &gyro_X, &gyro_Y, &gyro_Z);

//        sprintf((char*)txt,"ax:%06d",accelerat_X);
//        TFTSPI_P8X16Str(1,1,txt,u16RED,u16BLACK);// 字符串显示
//        sprintf((char*)txt,"ay:%06d",accelerat_Y);
//        TFTSPI_P8X16Str(1,2,txt,u16RED,u16BLACK);// 字符串显示
//        sprintf((char*)txt,"az:%06d",accelerat_Z);
//        TFTSPI_P8X16Str(1,3,txt,u16RED,u16BLACK);// 字符串显示
//        sprintf((char*)txt,"gx:%06d",gyro_X);
//        TFTSPI_P8X16Str(1,4,txt,u16RED,u16BLACK);// 字符串显示
//        sprintf((char*)txt,"gy:%06d",gyro_Y);
//        TFTSPI_P8X16Str(1,5,txt,u16RED,u16BLACK);// 字符串显示
//        sprintf((char*)txt,"gz:%06d",gyro_Z);
//        TFTSPI_P8X16Str(1,6,txt,u16RED,u16BLACK);// 字符串显示
//       
//        LED_Ctrl(LED0,RVS);                            //电平翻转,LED闪烁
//        delayms(50);
//    }
//}





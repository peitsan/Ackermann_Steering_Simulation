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
#include "LQ_CCD.h"


/**
  * @brief    CCD�ܽų�ʼ��
  *
  * @param    ccd_n:CCD��ţ�һ������CCD�ӿ�,����1��2
  *
  * @return   ��
  *
  * @note     
  *
  * @example  
  *
  * @date     2023/11/24
*/
void CCD_Init(char ccd_n) 
{
    if(ccd_n == 1)
    {
        CCD1_AO_INIT;
        CCD1_CLK_INIT;
        CCD1_SI_INIT;
    }
    else if(ccd_n == 2)
    {
        CCD2_AO_INIT;
        CCD2_CLK_INIT;
        CCD2_SI_INIT;
    }
}

void CCD_Delayus(uint16_t us)
{
    delayus(us);
}

/**
  * @brief    CCD�ɼ�����
  *
  * @param    p  ��  ��Ųɼ����ݵ��׵�ַ
  *
  * @return   ��
  *
  * @note     
  *
  * @example  uint16_t ccd_buf[128];
  *           CCD_Collect(ccd_buf); 
  *
*/
void CCD_Collect(uint16_t *p,char ccd_n)
{
	unsigned char i;
	unsigned int  temp = 0;
    if(ccd_n == 1)
    {
        CCD1_SI_H;              //SI  = 1 
        CCD_Delayus(1);
        CCD1_CLK_H;             // CLK = 1 
        CCD_Delayus(1);
        CCD1_SI_L;              // SI  = 0 
        CCD_Delayus(1);

        for(i=0; i<128; i++) 
        {
            CCD_Delayus(1);
            CCD1_CLK_H;         // CLK = 1 
            CCD_Delayus(1);
            temp = CCD1_AO_GET();       
            *p++ = temp;     
            CCD1_CLK_L;        // CLK = 0 
        }
        CCD_Delayus(1);
        CCD1_CLK_H;            // CLK = 1 
        CCD_Delayus(1);
        CCD1_CLK_L;            // CLK = 0 
    }
    else if(ccd_n == 2)
    {
        CCD2_SI_H;              //SI  = 1 
        CCD_Delayus(1);
        CCD2_CLK_H;             // CLK = 1 
        CCD_Delayus(1);
        CCD2_SI_L;              // SI  = 0 
        CCD_Delayus(1);

        for(i=0; i<128; i++) 
        {
            CCD_Delayus(1);
            CCD2_CLK_H;         // CLK = 1 
            CCD_Delayus(1);
            temp = CCD2_AO_GET();       
            *p++ = temp;     
            CCD2_CLK_L;        // CLK = 0 
        }
        CCD_Delayus(1);
        CCD2_CLK_H;            // CLK = 1 
        CCD_Delayus(1);
        CCD2_CLK_L;            // CLK = 0 
    }
}

/**
  * @brief    ����CCD  OLED��ͼ
  *
  * @param    ��
  *
  * @return   ��
  *
  * @note     
  *
  * @example  
  *
*/
void Test_CCD(char ccd_n)
{
	uint32_t xdata now_time = 0;
	uint16_t xdata ccd_buf[128];
	static uint8_t xdata count = 0;
	uint8_t xdata i = 0;
	
	GPIO_LED_Init();					//LED��ʼ��
	OLED_Init();
    OLED_CLS();
	CCD_Init(ccd_n);
	
	OLED_P8x16Str(15,0,"LQ CCD Test");  	
	
	while(1)
	{
        /* �ɼ�ccdͼ�� */
        CCD_Collect(ccd_buf,ccd_n);
        
        count++;
        /* OLED ��ʾ */
        if(count % 3 == 0)
        {
            for(i=0; i<128; i++)
            {
                OLED_Set_Pos(i , 7); 
                if(ccd_buf[i] > THRESHOLD)	      			
                    OLED_WrDat(0xFF);
                else
                    OLED_WrDat(0x00);	
            }
        
        }
        if(count == 240)
            count = 0;
        delayms(80);
    }
}













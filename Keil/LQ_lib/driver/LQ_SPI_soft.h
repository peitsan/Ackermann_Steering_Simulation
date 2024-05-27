#ifndef __LQ_SPIsoft_H_
#define __LQ_SPIsoft_H_
#include "include.h"

//                  ��Ƭ��    ����20602ģ��
#define SPI_SCK      P25      //SCK
#define SPI_MOSI     P24      //��Ƭ��MOSI��ģ��--SDI
#define SPI_MISO     P23      //��Ƭ��MISO��ģ��--SDO
#define SPI_CS       P22      //CS

void Soft_SPI_ReadWriteNbyte(u8 *lqbuff, u16 len);
void Soft_SPI_Init(void);
void LQ_SPI_Read(unsigned char reg, unsigned short len, unsigned char* buf);

void LQ_SPI_Write(unsigned char reg, unsigned char value);

#endif
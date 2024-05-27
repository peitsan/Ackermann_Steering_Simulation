#ifndef __LQ_SPI_H_
#define __LQ_SPI_H_
#include "include.h"
void SPI_init(void);
void SPI_WriteByte(u8 out);
u8 SPI_ReadByte(void);
void SPI_Read_Nbytes( u8 addr, u8 *buffer, u16 siz);
void SPI_Read_Nbytes(u8 addr, u8 *buffer, u16 mysize);
#endif

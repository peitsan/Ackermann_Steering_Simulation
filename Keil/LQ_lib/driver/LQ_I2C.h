#ifndef __LQ_IIC_H_
#define __LQ_IIC_H_
#include "include.h"

void WriteNbyte(u8 addr, u8 *p, u8 number);
void ReadNbyte(u8 addr, u8 *p, u8 number);
void I2C_init(void);

#endif
#ifndef _OV7670_H_
#define _OV7670_H_

#include "SCCB.h"

//#define OV7670_REG_NUM  114

#define OV7670_REG_NUM  167

/////////////////////////////////////////
//void XCLK_init_ON(void);//STM32 mco功能开启 输出时钟给CMOS传感器

unsigned char wr_Sensor_Reg(unsigned char regID, unsigned char regDat);
unsigned char rd_Sensor_Reg(unsigned char regID, unsigned char *regDat);

unsigned char Sensor_init(void);
/*
void Sensor_GPIO_Init(void);
void Sensor_EXTI_Config(void);
void Sensor_Interrupts_Config(void);
*/

#endif




#ifndef _FIFO_H_
#define _FIFO_H_

#include "stc15.h"

//===========FIFO PIN============
sbit FIFO_WEN =P1^0;
sbit FIFO_RRST=P1^1;
sbit FIFO_OE  =P1^2;
sbit FIFO_RCLK=P1^3; 

#define DATA_PORT (P2)
//===================================

int FIFO_Reset_Read_Addr(void);
int FIFO_Read_Words(unsigned int len,unsigned char *pbuf);

#endif
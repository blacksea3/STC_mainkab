#ifndef _SCCB_H_
#define _SCCB_H_

#include "stc15.h"

sbit SCCB_SIC=P3^6;
sbit SCCB_SID=P3^7;

#define DELAYTIME 1

void startSCCB(void);
void stopSCCB(void);
void noAck(void);
unsigned char getAck();
unsigned char SCCBwriteByte(unsigned char dat);
unsigned char SCCBreadByte(void);

#endif



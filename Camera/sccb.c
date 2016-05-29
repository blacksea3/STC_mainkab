#include "sccb.h"
#include "myfun.h"

//sfr P0M0 = 0x94; // 0000,0000
//sfr P0M1 = 0x93; // 0000,0000

/*
-----------------------------------------------
   功能: start命令,SCCB的起始信号
   参数: 无
 返回值: 无
-----------------------------------------------
*/
void startSCCB()
{
 SCCB_SID=1;
 delay(DELAYTIME);
 SCCB_SIC=1;
 delay(DELAYTIME);
 SCCB_SID=0;
 delay(DELAYTIME);
 SCCB_SIC=0;
 delay(DELAYTIME);
}
/*
-----------------------------------------------
   功能: stop命令,SCCB的停止信号
   参数: 无
 返回值: 无
-----------------------------------------------
*/
void stopSCCB()
{
 SCCB_SID=0;
 delay(DELAYTIME);
 SCCB_SIC=1;
 delay(DELAYTIME);
 SCCB_SID=1;
 delay(DELAYTIME);  
}

/*
-----------------------------------------------
   功能: noAck,用于连续读取中的最后一个结束周期
   参数: 无
 返回值: 无
-----------------------------------------------
*/
void noAck(void)
{
 SCCB_SID=1;
 delay(DELAYTIME);
 SCCB_SIC=1;
 delay(DELAYTIME);
 SCCB_SIC=0;
 delay(DELAYTIME);
 SCCB_SID=0;
 delay(DELAYTIME);
}

unsigned char getAck() 
{
 unsigned char idata Error;

 P0M1=0x02;
 SCCB_SID=1;//设置SCCB_SID为输入

 delay(DELAYTIME); 
 SCCB_SIC=1; 
 delay(DELAYTIME);
 Error=SCCB_SID;
 //if(SCCB_SID) P2=0x00; 
 //else P2=0xff;
 delay(DELAYTIME);
 SCCB_SIC=0; 
 delay(DELAYTIME);

 P0M1=0x00;
 SCCB_SID=0;
 return !Error;
}

/*
-----------------------------------------------
   功能: 写入一个字节的数据到SCCB
   参数: 写入数据
 返回值: 发送成功返回1，发送失败返回0
-----------------------------------------------
*/
unsigned char SCCBwriteByte(unsigned char dat)
{
 unsigned char idata i;
 for(i=0;i<8;i++)
 {
  SCCB_SID=((dat<<i)&0x80);
  delay(DELAYTIME);
  SCCB_SIC=1;
  delay(DELAYTIME);
  SCCB_SIC=0;
  delay(DELAYTIME);
 }
 SCCB_SID=0;
 return getAck();
}

/*
-----------------------------------------------
   功能: 一个字节数据读取并且返回
   参数: 无
 返回值: 读取到的数据
-----------------------------------------------
*/
unsigned char SCCBreadByte(void)
{
 unsigned char idata i,rbyte=0;

 P0M1=0x02;
 SCCB_SID=1;//设置SCCB_SID为输入

 for(i=0;i<8;i++)
 {
  SCCB_SIC=1;
  delay(DELAYTIME);
  if(SCCB_SID) rbyte|=(0x80>>i);
  delay(DELAYTIME);
  SCCB_SIC=0;
  delay(DELAYTIME);
 } 

 P0M1=0x00;
 SCCB_SID=0;
 return rbyte;
}

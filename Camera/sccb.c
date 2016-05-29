#include "sccb.h"
#include "myfun.h"

//sfr P0M0 = 0x94; // 0000,0000
//sfr P0M1 = 0x93; // 0000,0000

/*
-----------------------------------------------
   ����: start����,SCCB����ʼ�ź�
   ����: ��
 ����ֵ: ��
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
   ����: stop����,SCCB��ֹͣ�ź�
   ����: ��
 ����ֵ: ��
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
   ����: noAck,����������ȡ�е����һ����������
   ����: ��
 ����ֵ: ��
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
 SCCB_SID=1;//����SCCB_SIDΪ����

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
   ����: д��һ���ֽڵ����ݵ�SCCB
   ����: д������
 ����ֵ: ���ͳɹ�����1������ʧ�ܷ���0
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
   ����: һ���ֽ����ݶ�ȡ���ҷ���
   ����: ��
 ����ֵ: ��ȡ��������
-----------------------------------------------
*/
unsigned char SCCBreadByte(void)
{
 unsigned char idata i,rbyte=0;

 P0M1=0x02;
 SCCB_SID=1;//����SCCB_SIDΪ����

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

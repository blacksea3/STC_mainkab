/****************��51��Ƭ����������-����STC15W4Kϵ�С��������� *************
 �������������������������
 ��51��Ƭ����������-����STC15W4Kϵ�С� һ���Ѿ��ɱ�����������ʽ���淢�С�
  �������ִ�������̲����׵�51˫��ʵ���(2��MCU)�Գ������ء����ԡ����淽�㣬����Ҫ�ⲿ
  ��������������������Ʒ�ʽ���׽����ϵͳ�ж��������ȼ�˭Ҳ������˭���жϾ������⡣
  �Ա����ַ��https://shop117387413.taobao.com
  QQȺ��STC51-STM32(3) ��515624099 �� STC51-STM32(2)��99794374��
        ��֤��Ϣ��STC15��Ƭ��
  ���䣺xgliyouquan@126.com
  �������������������������*/

/**************************************************
;����ͼ:
;DB0---DPROT.0  DB4---DPROT.4    RS-------------P1.3
;DB1---DPROT.1  DB5---DPROT.5    RW-------------P1.2
;DB2---DPROT.2  DB6---DPROT.6    E--------------P4.7
;DB3---DPROT.3  DB7---DPROT.7    
;STC15f2k6060S2�ľ���Ƶ��Ϊ22.1184MHz 
;**************************************************/ 
#include "STC15W4K.H"
#include "intrins.h"		 // _nop_()������Ҫ	 
sbit	RS	=	P1^3;		 // ����ʵ��Ӳ�������޸�
sbit	RW	=	P1^2;		 // ����ʵ��Ӳ�������޸�
sbit	E	=	P1^1;		 // ����ʵ��Ӳ�������޸�
#define DPORT	P0			 // ����ʵ��Ӳ�������޸�

#define NoDisp 0 		     // ����ʾ
#define NoCur 1 	 	     // ����ʾ�޹��
#define CurNoFlash 2	     // �й�굫����˸
#define CurFlash 3     	     // �й������˸

void LCD1602_Init();		 // ��ʼ��
void ClrLcd();           	 // ��������
void SetCur(unsigned char Para);	//���ù��
void WriteChar(unsigned char xPos,unsigned char yPos,unsigned char Dat);  // д1���ַ�
void WriteString(unsigned char xPos,unsigned char yPos,unsigned char *s); // д�ַ���
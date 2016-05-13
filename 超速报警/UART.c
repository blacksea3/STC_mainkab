/*UART.c
ʹ��STC�ٷ��ĵ�����*/

#include "reg51.h"
#include "intrins.h"

typedef unsigned char BYTE;
typedef unsigned int WORD;

#define FOSC 11059200L                       //ϵͳƵ��
#define BAUD 115200                          //���ڲ�����

#define NONE_PARITY 0                        //��У��
#define ODD_PARITY 1                         //��У��
#define EVEN_PARITY 2                        //żУ��
#define MARK_PARITY 3                        //���У��
#define SPACE_PARITY 4                       //�հ�У��

#define PARITYBIT EVEN_PARITY                //����У��λ

sfr AUXR = 0x8e;                             //�����Ĵ���
sfr T2H = 0xd6;                            //��ʱ��2��8λ
sfr T2L = 0xd7;                            //��ʱ��2��8λ

sbit P22 = P2^2;

bit busy;

void SendData(BYTE dat);
void SendString(char *s);
void UARTInit();

void UARTInit()
{
#if(PARITYBIT == NONE_PARITY)
	SCON = 0x50;                   			//8λ�ɱ䲨����
#elif(PARITYBIT == ODD_PARITY)||(PARITYBIT == EVEN_PARITY)||(PARITYBIT == MARK_PARITY)
	SCON = 0xda;                   			//9λ�ɱ䲨����,У��λ��ʼΪ1
#elif(PARITYBIT == SPACE_PARITY)   	
	SCON = 0xd2;                   			//9λ�ɱ䲨����,У��λ��ʼΪ0
#endif
	
    T2L = (65536-(FOSC/4/BAUD));    		//���ò�������װֵ
	T2H = (65536-(FOSC/4/BAUD))>>8;
	AUXR |= 0x14;                   		//T2 1Tģʽ,������ʱ��2
	AUXR |= 0x01;                  			//��ʱ��2Ϊ����1�Ĳ����ʷ�����
	ES = 1;                        			//ʹ�ܴ���1�ж�
	//EA = 1;                        		//ʹ�����ж�
}

/*-------------------------
UART �жϷ������
-------------------------*/
void Uart() interrupt 4 using 1
{
	if(RI)
	{
		RI = 0;                     //���RIλ
		P0 = SBUF;                  //P0��ʾ��������
		P22 = RB8;                  //P2.2��ʾ����λ
	}
	if(TI)
	{
		TI = 0;                     //���TIλ
		busy = 0;                   //��æ��־
	}
}

/*--------------------------
UART�жϷ������
--------------------------*/
void SendData(BYTE dat)
{
	while(busy);                     //�ȴ�ǰ������ݷ������
	ACC = dat;                       //��ȡУ��λP(PSW.0)
	if(P)
	{
	#if(PARITYBIT == ODD_PARITY)
		TB8 = 0;                     //����У��λΪ0
	#elif(PARITYBIT == EVEN_PARITY)
		TB8 = 1;                     //����У��λΪ1
	#endif
	}
	else
	{
	#if(PARITYBIT == ODD_PARITY)
		TB8 = 1;                     //����У��λΪ0
	#elif(PARITYBIT == EVEN_PARITY)
		TB8 = 0;                     //����У��λΪ1
	#endif
	}
	busy = 1;
	SBUF = ACC;                      //д���ݵ�UART���ݼĴ���
}

/*--------------------------
�����ַ���
--------------------------*/
void SendString(char *s)
{
	while(*s)                        //����ַ���������־
	{
		SendData(*s++);              //���͵�ǰ�ַ�
	}
}
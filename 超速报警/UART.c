/*UART.c
ʹ��STC�ٷ��ĵ�����*/

#include "reg51.h"
#include "intrins.h"
#include "main.h"
#include "Timer.h"
#include "Delay.h"

typedef unsigned char BYTE;
typedef unsigned int WORD;

#define FOSC 11059200L                      //ϵͳƵ��
#define BAUD 115200                         //���ڲ�����

#define NONE_PARITY 0                       //��У��
#define ODD_PARITY 1                        //��У��
#define EVEN_PARITY 2                       //żУ��
#define MARK_PARITY 3                       //���У��
#define SPACE_PARITY 4                      //�հ�У��

#define PARITYBIT NONE_PARITY               //����У��λ

#define PARITYBIT2 NONE_PARITY              //����У��λ

sfr AUXR = 0x8e;                            //�����Ĵ���
sfr T2H = 0xd6;                            	//��ʱ��2��8λ
sfr T2L = 0xd7;                            	//��ʱ��2��8λ
sbit P22 = P2^2;
bit busy,busy2;

sfr S2CON = 0x9a;							//UART2���ƼĴ���
sfr S2BUF = 0x9b;							//UART2���ݼĴ���
sfr IE2 = 0xaf;								//�жϿ��ƼĴ���2

sfr IP2 = 0xB5;   							//xxxx,xx00 �ж����ȼ��Ĵ���2

sbit P15        =   P1^5;

#define S2RI 0x01							//S2CON.0
#define S2TI 0x02							//S2CON.1
#define S2RB8 0x04							//S2CON.2
#define S2TB8 0x08							//S2CON.3


void SendData(BYTE dat);
void SendString(char *s);
void UARTInit();

//unsigned char UART2Temp[256];
//unsigned int UART2Loc = 0;
//unsigned char UART1Temp[256];
//unsigned int UART1Loc = 0;
unsigned char UART1Temp;
unsigned char UART2Temp;

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
	//TL1 = (65536-(FOSC/4/BAUD));
	//TH1 = (65536-(FOSC/4/BAUD))>>8;
	TMOD = 0x00;
	AUXR |= 0x14;                   		//T2 1Tģʽ,������ʱ��2
	AUXR |= 0x01;                  			//��ʱ��2Ϊ����1�Ĳ����ʷ�����

	ES = 1;                        			//ʹ�ܴ���1�ж�
	PS = 1;                               	//UART�жϵ���Ϊ�����ȼ�
	ET1 = 0; 
}

void UART2Init()
{
#if(PARITYBIT2 == NONE_PARITY)
	S2CON = 0x50;                   		//8λ�ɱ䲨����
#elif(PARITYBIT2 == ODD_PARITY)||(PARITYBIT2 == EVEN_PARITY)||(PARITYBIT2 == MARK_PARITY)
	S2CON = 0xda;                   		//9λ�ɱ䲨����,У��λ��ʼΪ1
#elif(PARITYBIT2 == SPACE_PARITY)   	
	S2CON = 0xd2;                   		//9λ�ɱ䲨����,У��λ��ʼΪ0
#endif
    T2L = (65536-(FOSC/4/BAUD));    		//���ò�������װֵ
	T2H = (65536-(FOSC/4/BAUD))>>8;
	AUXR |= 0x14;                   		//T2 1Tģʽ,������ʱ��2
	AUXR |= 0x01;                  			//��ʱ��2Ϊ����1�Ĳ����ʷ�����
	IE2 |= 0x01;                        	//ʹ�ܴ���2�ж�
	//UART2Loc = 0;
	IP2 |= 0x01;						   	//UART2�жϵ���Ϊ�����ȼ�
	IP2 &= ~0xfe;
	//PS2 = 1;                              		
}

/*-------------------------
UART1 �жϷ������
-------------------------*/
void Uart() interrupt 4
{
	P15 = 1;
	if(RI)
	{
		RI = 0;                     //���RIλ
		UART1Temp = SBUF;
		//UART1Temp[UART1Loc++] = SBUF;   
		UART1RIREADY = 1;
		/*if(WIFINEEDDELAY == 0)
		{
			EnableTimer4();
			WIFINEEDDELAY = 1;
		}
		else
		{
			T4times = 0;
		}*/
	}
	if(TI)
	{
		TI = 0;                     //���TIλ
		busy = 0;                   //��æ��־
	}
}

/*--------------------------
UART1��������
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
    unsigned char FUCK = 0;
	while(*s)                        //����ַ���������־
	{
		SendData(*s++);              //���͵�ǰ�ַ�
		FUCK ++;
		if(FUCK==1)
		{
			P15 = 0;
		}
	}
	
}

/*
 * ����1������2���յ���Ϣ
 */
void Uart1SendUart2String()
{
    //unsigned char i;
	//for(i=0;i<UART2Loc;i++)
	//{
	//	SendData(UART2Temp[i]);
	//}
	//UART2Loc = 0;
	SendData(UART2Temp);
}

/*-------------------------
UART2 �жϷ������
-------------------------*/
void Uart2() interrupt 8
{
	if(S2CON&S2RI)
	{
		S2CON&=~S2RI;                 	//���RIλ
		//UART2Temp[UART2Loc++] = S2BUF;   
		//UART2RIREADY = 1;
		UART2Temp = S2BUF;
		/*if(WIFINEEDDELAY == 0)
		{
			EnableTimer4();
			WIFINEEDDELAY = 1;
		}
		else
		{
			T4times = 0;
		}*/
		//SendData(SBUF);
		//P22 = RB8;                  	//P2.2��ʾ����λ
	}
	if(S2CON&S2TI)
	{
		S2CON&=~S2TI;                 	//���TIλ
		busy2 = 0;                 	  	//��æ��־
	}
}

/*--------------------------
UART2��������
--------------------------*/
void SendData2(BYTE dat)
{
	while(busy2);                     	//�ȴ�ǰ������ݷ������
	ACC = dat;                       	//��ȡУ��λP(PSW.0)
	if(P)
	{
	#if(PARITYBIT2 == ODD_PARITY)
		S2CON&=~S2TB8;               	//����У��λΪ0
	#elif(PARITYBIT2 == EVEN_PARITY)
		S2CON|=~S2TB8;               	//����У��λΪ1
	#endif
	}
	else
	{
	#if(PARITYBIT2 == ODD_PARITY)
		S2CON|=~S2TB8;               	//����У��λΪ1
	#elif(PARITYBIT2 == EVEN_PARITY)
		S2CON&=~S2TB8;               	//����У��λΪ0
	#endif
	}
	busy2 = 1;
	S2BUF = ACC;                      	//д���ݵ�UART���ݼĴ���
	
}

/*--------------------------
UART2�����ַ���
--------------------------*/
void SendString2(char *s)
{
	while(*s)                        	//����ַ���������־
	{
		SendData2(*s++);              	//���͵�ǰ�ַ�
	}
}

void SendString2Length(char s[], unsigned char length)
{
	unsigned char i;
	for(i=0;i<length;i++)
	{
		SendData2(s[i]);              	//���͵�ǰ�ַ�
	}
	//WIFINEEDDELAY = 0;
}

/*
 * ����2������1���յ���Ϣ
 */
void Uart2SendUart1String()
{
    //unsigned char i;
	//for(i=0;i<UART1Loc;i++)
	//{
	SendData2(UART1Temp);
	if(UART1Temp=='G')
	{
		SendString2("AT+CIPMUX=1\r\n");
		Delay5Ms();
		SendString2("AT+CIPSERVER=1,333\r\n");
		Delay5Ms();
		SendString2("AT+CIPSTO=7200\r\n");
		Delay5Ms();
		SendString2("AT+CIPSEND=0,26\r\n");
		Delay5Ms();
		SendString2("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	}
	//}
	//UART1Loc = 0;
}
/*UART.c
使用STC官方文档代码*/

#include "reg51.h"
#include "intrins.h"
#include "main.h"
#include "Timer.h"
#include "Delay.h"

typedef unsigned char BYTE;
typedef unsigned int WORD;

#define FOSC 11059200L                      //系统频率
#define BAUD 115200                         //串口波特率

#define NONE_PARITY 0                       //无校验
#define ODD_PARITY 1                        //奇校验
#define EVEN_PARITY 2                       //偶校验
#define MARK_PARITY 3                       //标记校验
#define SPACE_PARITY 4                      //空白校验

#define PARITYBIT NONE_PARITY               //定义校验位

#define PARITYBIT2 NONE_PARITY              //定义校验位

sfr AUXR = 0x8e;                            //辅助寄存器
sfr T2H = 0xd6;                            	//定时器2高8位
sfr T2L = 0xd7;                            	//定时器2低8位
sbit P22 = P2^2;
bit busy,busy2;

sfr S2CON = 0x9a;							//UART2控制寄存器
sfr S2BUF = 0x9b;							//UART2数据寄存器
sfr IE2 = 0xaf;								//中断控制寄存器2

sfr IP2 = 0xB5;   							//xxxx,xx00 中断优先级寄存器2

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
	SCON = 0x50;                   			//8位可变波特率
#elif(PARITYBIT == ODD_PARITY)||(PARITYBIT == EVEN_PARITY)||(PARITYBIT == MARK_PARITY)
	SCON = 0xda;                   			//9位可变波特率,校验位初始为1
#elif(PARITYBIT == SPACE_PARITY)   	
	SCON = 0xd2;                   			//9位可变波特率,校验位初始为0
#endif
	
    T2L = (65536-(FOSC/4/BAUD));    		//设置波特率重装值
	T2H = (65536-(FOSC/4/BAUD))>>8;
	//TL1 = (65536-(FOSC/4/BAUD));
	//TH1 = (65536-(FOSC/4/BAUD))>>8;
	TMOD = 0x00;
	AUXR |= 0x14;                   		//T2 1T模式,启动定时器2
	AUXR |= 0x01;                  			//定时器2为串口1的波特率发生器

	ES = 1;                        			//使能串口1中断
	PS = 1;                               	//UART中断调整为高优先级
	ET1 = 0; 
}

void UART2Init()
{
#if(PARITYBIT2 == NONE_PARITY)
	S2CON = 0x50;                   		//8位可变波特率
#elif(PARITYBIT2 == ODD_PARITY)||(PARITYBIT2 == EVEN_PARITY)||(PARITYBIT2 == MARK_PARITY)
	S2CON = 0xda;                   		//9位可变波特率,校验位初始为1
#elif(PARITYBIT2 == SPACE_PARITY)   	
	S2CON = 0xd2;                   		//9位可变波特率,校验位初始为0
#endif
    T2L = (65536-(FOSC/4/BAUD));    		//设置波特率重装值
	T2H = (65536-(FOSC/4/BAUD))>>8;
	AUXR |= 0x14;                   		//T2 1T模式,启动定时器2
	AUXR |= 0x01;                  			//定时器2为串口1的波特率发生器
	IE2 |= 0x01;                        	//使能串口2中断
	//UART2Loc = 0;
	IP2 |= 0x01;						   	//UART2中断调整为高优先级
	IP2 &= ~0xfe;
	//PS2 = 1;                              		
}

/*-------------------------
UART1 中断服务程序
-------------------------*/
void Uart() interrupt 4
{
	P15 = 1;
	if(RI)
	{
		RI = 0;                     //清除RI位
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
		TI = 0;                     //清除TI位
		busy = 0;                   //清忙标志
	}
}

/*--------------------------
UART1发送数据
--------------------------*/
void SendData(BYTE dat)
{
	while(busy);                     //等待前面的数据发送完成
	ACC = dat;                       //获取校验位P(PSW.0)
	if(P)
	{
	#if(PARITYBIT == ODD_PARITY)
		TB8 = 0;                     //设置校验位为0
	#elif(PARITYBIT == EVEN_PARITY)
		TB8 = 1;                     //设置校验位为1
	#endif
	}
	else
	{
	#if(PARITYBIT == ODD_PARITY)
		TB8 = 1;                     //设置校验位为0
	#elif(PARITYBIT == EVEN_PARITY)
		TB8 = 0;                     //设置校验位为1
	#endif
	}
	busy = 1;
	SBUF = ACC;                      //写数据到UART数据寄存器
	
}

/*--------------------------
发送字符串
--------------------------*/
void SendString(char *s)
{
    unsigned char FUCK = 0;
	while(*s)                        //检测字符串结束标志
	{
		SendData(*s++);              //发送当前字符
		FUCK ++;
		if(FUCK==1)
		{
			P15 = 0;
		}
	}
	
}

/*
 * 串口1发串口2接收的信息
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
UART2 中断服务程序
-------------------------*/
void Uart2() interrupt 8
{
	if(S2CON&S2RI)
	{
		S2CON&=~S2RI;                 	//清除RI位
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
		//P22 = RB8;                  	//P2.2显示检验位
	}
	if(S2CON&S2TI)
	{
		S2CON&=~S2TI;                 	//清除TI位
		busy2 = 0;                 	  	//清忙标志
	}
}

/*--------------------------
UART2发送数据
--------------------------*/
void SendData2(BYTE dat)
{
	while(busy2);                     	//等待前面的数据发送完成
	ACC = dat;                       	//获取校验位P(PSW.0)
	if(P)
	{
	#if(PARITYBIT2 == ODD_PARITY)
		S2CON&=~S2TB8;               	//设置校验位为0
	#elif(PARITYBIT2 == EVEN_PARITY)
		S2CON|=~S2TB8;               	//设置校验位为1
	#endif
	}
	else
	{
	#if(PARITYBIT2 == ODD_PARITY)
		S2CON|=~S2TB8;               	//设置校验位为1
	#elif(PARITYBIT2 == EVEN_PARITY)
		S2CON&=~S2TB8;               	//设置校验位为0
	#endif
	}
	busy2 = 1;
	S2BUF = ACC;                      	//写数据到UART数据寄存器
	
}

/*--------------------------
UART2发送字符串
--------------------------*/
void SendString2(char *s)
{
	while(*s)                        	//检测字符串结束标志
	{
		SendData2(*s++);              	//发送当前字符
	}
}

void SendString2Length(char s[], unsigned char length)
{
	unsigned char i;
	for(i=0;i<length;i++)
	{
		SendData2(s[i]);              	//发送当前字符
	}
	//WIFINEEDDELAY = 0;
}

/*
 * 串口2发串口1接收的信息
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
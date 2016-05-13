/*UART.c
使用STC官方文档代码*/

#include "reg51.h"
#include "intrins.h"

typedef unsigned char BYTE;
typedef unsigned int WORD;

#define FOSC 11059200L                       //系统频率
#define BAUD 115200                          //串口波特率

#define NONE_PARITY 0                        //无校验
#define ODD_PARITY 1                         //奇校验
#define EVEN_PARITY 2                        //偶校验
#define MARK_PARITY 3                        //标记校验
#define SPACE_PARITY 4                       //空白校验

#define PARITYBIT EVEN_PARITY                //定义校验位

sfr AUXR = 0x8e;                             //辅助寄存器
sfr T2H = 0xd6;                            //定时器2高8位
sfr T2L = 0xd7;                            //定时器2低8位

sbit P22 = P2^2;

bit busy;

void SendData(BYTE dat);
void SendString(char *s);
void UARTInit();

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
	AUXR |= 0x14;                   		//T2 1T模式,启动定时器2
	AUXR |= 0x01;                  			//定时器2为串口1的波特率发生器
	ES = 1;                        			//使能串口1中断
	//EA = 1;                        		//使能总中断
}

/*-------------------------
UART 中断服务程序
-------------------------*/
void Uart() interrupt 4 using 1
{
	if(RI)
	{
		RI = 0;                     //清除RI位
		P0 = SBUF;                  //P0显示串口数据
		P22 = RB8;                  //P2.2显示检验位
	}
	if(TI)
	{
		TI = 0;                     //清除TI位
		busy = 0;                   //清忙标志
	}
}

/*--------------------------
UART中断服务程序
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
	while(*s)                        //检测字符串结束标志
	{
		SendData(*s++);              //发送当前字符
	}
}
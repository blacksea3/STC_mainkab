#include "stc15.h"
//#include "Temperature.h"
#include "UART.h"

#define TFOSC 11059200L
#define TIMS (65536-TFOSC/1000)  //1T模式

unsigned int T0times;     	//定时器0中断次数
unsigned int T1times;     	//定时器1中断次数
unsigned int T3times;     	//定时器3中断次数
unsigned int T4times;     	//定时器4中断次数
//sbit P20 = P2^0;

//固定定时器Timer0
void Timer0Init(void)		//1毫秒@11.0592MHz
{
	AUXR |= 0x80;		    //定时器0时钟1T模式
	TMOD &= 0xfc;			//设置定时器0 16位自动重装载
	TL0 = TIMS;				//设置定时初值
	TH0 = TIMS>>8;			//设置定时初值
	TR0 = 1;		        //定时器0开始计时
	ET0 = 1;            	//使能定时器0中断
	T0times = 0;
}

//Timer0中断函数
void Timer0_ISR() interrupt 1 using 2			//由于发送数据使用串口中断,尽量避免在别的中断里直接发数据
{
    //EA = 0;
	if(T0times<500)
	{	
		T0times++;
	}
	else
	{
	    P20 = !P20;
		T0times=0;
		//SendString("ABCDE");
		//ReadTemperature();                    //读取温度湿度传感器值
		//ReadAir();                            //读取空气传感器值
		//WifiSend();                           //Wifi发送数据
	}
	//EA = 1;
}

//可变定时器Timer1
void Timer1Init(void)		//1毫秒@11.0592MHz
{
	AUXR |= 0x40;		    //定时器1时钟1T模式
	TMOD &= 0xf3;			//设置定时器1 16位自动重装载
	TL1 = TIMS;				//设置定时初值
	TH1 = TIMS>>8;			//设置定时初值
	TR1 = 0;		        //定时器1停止计时
	ET1 = 0;            	//关闭定时器1中断
}

void EnableTimer1()
{
	TR1 = 1;		        //定时器1开始计时
	ET1 = 1;            	//使能定时器1中断
	T1times = 0;
}

void DisableTimer1()
{
	TR1 = 0;		        //定时器1停止计时
	ET1 = 0;            	//关闭定时器1中断	
}

//Timer1中断函数
void Timer1_ISR() interrupt 3 using 2			//由于发送数据使用串口中断,尽量避免在别的中断里直接发数据
{
    //EA = 0;
	if(T1times<1000)
	{	
		T1times++;
	}
	else
	{
		P24 = !P24;
		T1times=0;
		//SendString("ABCDE");
		//ReadTemperature();                    //读取温度湿度传感器值
		//ReadAir();                            //读取空气传感器值
		//WifiSend();                           //Wifi发送数据
	}
	//EA = 1;
}

//可变定时器Timer3
void Timer3Init(void)		//1毫秒@11.0592MHz
{
	T4T3M &= 0xfb;		    //定时器3内部系统时钟
	T4T3M |= 0x02;          //定时器3 1T模式
	T3L = TIMS;				//设置定时初值
	T3H = TIMS>>8;			//设置定时初值
	T4T3M &= 0xf7;		    //定时器3停止计时
	IE2 &= 0xdf;            //关闭定时器3中断
}

void EnableTimer3()
{
	T4T3M |= 0x08;		    //定时器3开始计时
	IE2 |= 0x20;            //开启定时器3中断
	T3times = 0;
}

void DisableTimer3()
{
	T4T3M &= 0xf7;		    //定时器3停止计时
	IE2 &= 0xdf;            //关闭定时器3中断	
}

//Timer3中断函数
void Timer3_ISR() interrupt 19 using 2			//由于发送数据使用串口中断,尽量避免在别的中断里直接发数据
{
    //EA = 0;
	if(T3times<2000)
	{	
		T3times++;
	}
	else
	{
		P25 = !P25;
		T3times=0;
		//SendString("ABCDE");
		//ReadTemperature();                    //读取温度湿度传感器值
		//ReadAir();                            //读取空气传感器值
		//WifiSend();                           //Wifi发送数据
	}
	//EA = 1;
}

//可变定时器Timer4
void Timer4Init(void)		//1毫秒@11.0592MHz
{
	T4T3M &= 0xbf;		    //定时器4内部系统时钟
	T4T3M |= 0x20;          //定时器4 1T模式
	T4L = TIMS;				//设置定时初值
	T4H = TIMS>>8;			//设置定时初值
	T4T3M &= 0x7f;		    //定时器4停止计时
	IE2 &= 0xbf;            //关闭定时器4中断
}

void EnableTimer4()
{
	T4T3M |= 0x80;		    //定时器4开始计时
	IE2 |= 0x40;            //开启定时器4中断
	T4times = 0;
}

void DisableTimer4()
{
	T4T3M &= 0x7f;		    //定时器4停止计时
	IE2 &= 0xbf;            //关闭定时器4中断	
}

//Timer4中断函数
void Timer4_ISR() interrupt 20 using 2			//由于发送数据使用串口中断,尽量避免在别的中断里直接发数据
{
    //EA = 0;
	if(T4times<4000)
	{	
		T4times++;
	}
	else
	{
		P26 = !P26;
		T4times=0;
		//SendString("ABCDE");
		//ReadTemperature();                    //读取温度湿度传感器值
		//ReadAir();                            //读取空气传感器值
		//WifiSend();                           //Wifi发送数据
	}
	//EA = 1;
}

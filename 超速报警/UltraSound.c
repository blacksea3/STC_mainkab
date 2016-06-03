#include "stc15.h"
//#include "UART.h"
#include "Timer.h"
#include "Delay.h"
#include "LCD12864.h"
#include "main.h"
#include "FLASH.h"

sbit trig=P4^1;	//触发输入 至少为10us以上
sbit echo=P4^2 ;	//输出回响信号

unsigned long RawTime;
signed int RealDistance;

#define FOSC_FENZHIYI 9.0422e-5                	//先乘1000,下面直接当成毫秒

char bat[7]; //保存超声波测量的数据，单位为m   	//1个操作9.0422e-8   1000个e-5    RawTime*FOSC_FENZHIYI s
void delay(unsigned int i)
{
	while(i--);
}

void super_start()	//触发超声波开始工作
{
	trig=1;
	delay(10);	 //至少10us
	trig=0;	
}
signed int super_count()	  //计算超声波回响的高电平持续时间,直接取出TH4和TL4		 0-25m/s
{
    RawTime=65536*T1times+TH1*256+TL1;
	RealDistance=(RawTime*FOSC_FENZHIYI*170);   //精确到mm								 3m         1/11059200 s  假设1m 则t=2m/340m/s  
					  															 		//0-4000	   pp * 	 9.0422e-8 s * 170m/s = 4000mm
	//bat[0]=RealDistance/1000 + 48;
	//bat[1]=(unsigned char)('.');
	//bat[2]=RealDistance%1000/100 + 48;
	//bat[3]=RealDistance%100/10 + 48;
	//bat[4]=RealDistance%10 + 48;
	//bat[5]=(unsigned char)('m');
	//bat[6]=(unsigned char)('m');
	//bat[7]='\0';
	return RealDistance;
}

signed int DisUltraSound()
{
    signed int t;
	super_start();
	while(echo==0);	  //等待高电平
	EnableTimer1();	  //打开定时器1开始计时
	while(echo==1);	  //等待高电平下去变成低电平（高电平持续）
	DisableTimer1();  //关闭定时器1
	t=super_count();
	return t;
}

void UltraSoundDisplay()
{     
    //unsigned char bat1[8]={0,0,0,0,0,0};
	//unsigned char bat2[8]={0,0,0,0,0,0};
	//unsigned char bat3[10]={'+',0,'.',0,0,0,'m','/','s','\0'};
	  
	unsigned char VelocityData[] = {"速度为000000000 "};			//字符串初始化
	unsigned long Dis1,Dis2,DDis;								//临时变量
	unsigned char i;											//临时变量

	unsigned int TempVelocityThreshold;						//速度阈值

    TempVelocityThreshold = GetVelocityThreshold();				//获取当前速度阈值

	//for(i=0;i<5;i++)
    //{
	//	Delay10ms();
	//}
    Dis1=DisUltraSound();          	  							//超声波模块测距
    for(i=0;i<5;i++)
	{
		Delay50ms();
	}
    Dis2=DisUltraSound();          	  							//超声波模块测距

    if(Dis1>4000 || Dis2>4000)								  	//单位mm
	{
		VelocityData[7] = 'N';
		VelocityData[8] = '/';
		VelocityData[9] = 'A';
		VelocityData[10] = 'N';
		VelocityData[11] = '/';
		VelocityData[12] = 'A';
		VelocityData[13] = 'N';
		VelocityData[14] = 'A';
		VelocityData[15]='\0';

		Display_String(1,VelocityData);
	}
	else
	{
    	if(Dis1>Dis2)
		{
			DDis=Dis1-Dis2;
			VelocityData[6] = '+';
		}
		else
		{
			DDis=Dis2-Dis1;
			VelocityData[6] = '-';
		}
		DDis*=4;											//单位mm  时间差是250ms,要乘1000ms/250ms=4
        DDis*=3.6;											//转为km/h,取整即可
		VelocityData[7]=DDis%100000/10000 + 48;
		VelocityData[8]=DDis%10000/1000 + 48;
		VelocityData[9]=(unsigned char)('.');
		VelocityData[10]=DDis%1000/100 + 48;						
		VelocityData[11]=DDis%100/10 + 48;
		//VelocityData[10]=DDis%10 + 48;
		VelocityData[12]=(unsigned char)('k');							 
		VelocityData[13]=(unsigned char)('m');
		VelocityData[14]=(unsigned char)('/');
		VelocityData[15]=(unsigned char)('h');
		VelocityData[16]='\0';

		Display_String(1,VelocityData);

		if((unsigned int)DDis > TempVelocityThreshold)					 //超过速度阈值,单位m/h,1000mm/s=1m/s=3.6km/h
		{	 
			FASTSPEED = 1;
			//P20 = !P20;
			//Display_String(1,VelocityData);
			//LCD12864DisplayChar(2,6,DDis%100000/10000 + 48);
			//LCD12864DisplayChar(2,7,DDis%10000/1000 + 48);
			//LCD12864DisplayChar(2,8,DDis%1000/100 + 48);
			//LCD12864DisplayChar(3,7,TempVelocityThreshold%1000/100 + 48);                                      
		}
	}	
}
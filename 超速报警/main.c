#include "stc15.h"
#include "Timer.h"

#include "LCD12864.H"    //

#include "DHT11.h"				  //DHT11没有初始化程序

//#include "Temperature.h"
//#include "Wifi.h"
#include "Delay.h"

//#include "Force.h"
//#include "Camera.h"
#include "UART.h"
#include "UltraSound.h"

#include "main.h"

unsigned char code StrHello[]="HELLOWORLD!";  
unsigned char DHT11ISREADY;
unsigned char ULTRAISREADY;
unsigned char FASTSPEED;
unsigned char TIMER3STOP;

void port_mode()            // 端口模式
{
	P0M1=0x00; P0M0=0x00;P1M1=0x00; P1M0=0x00;P2M1=0x00; P2M0=0x00;P3M1=0x00; P3M0=0x00;
	P4M1=0x00; P4M0=0x00;P5M1=0x00; P5M0=0x00;P6M1=0x00; P6M0=0x00;P7M1=0x00; P7M0=0x00; 
}

void main()
{	
    //UARTInit();
    unsigned char code FUCKSTOP[]={"FUCKSTOOP!"};
	unsigned char code FUCKSTART[]={"FUCKSTAART!"};


    DHT11ISREADY = 0;	  //DHT11未激活
	ULTRAISREADY = 0;	  //超声波未激活,他们两个由Timer0中断激活
	FASTSPEED = 0;		  //超速蜂鸣器未激活
	TIMER3STOP = 0;
    UltraSoundInit();	  //超声波初始化

    port_mode();		  //端口设置全部弱上拉
    Delay1000ms();		  //延时3s
    Delay1000ms();
    Delay1000ms();   
    Lcm_Init();			  //LCD12864以及初始化

    Timer3Init();         //蜂鸣器定时器初始化但不开启,开启由变量FASTSPEED指示

	Timer0Init();		  //DHT11和超声波
	EA = 1;     		  //使能总中断
    P20 = 0;

	Display_String(1,StrHello);			   		    //LCD显示HelloWorld!
	while(1)
	{	
        if(TIMER3STOP)
		{
			DisableTimer3();
			Display_String(4,FUCKSTOP);
		}

		if(DHT11ISREADY)
		{
			DisplayDHT11();                         //读取温度湿度传感器值
			DHT11ISREADY = 0;
		}
		else if(ULTRAISREADY)
		{
			UltraSoundDisplay();                    //超声波
			ULTRAISREADY = 0;
			if(FASTSPEED)
			{
			    TIMER3STOP = 0;
				EnableTimer3();
				Display_String(4,FUCKSTART);
			}
		}
		else ;
	}
}
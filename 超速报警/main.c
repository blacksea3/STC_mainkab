#include "stc15.h"
#include "Timer.h"
#include "Delay.h"

#include "main.h"

#include "LCD12864.H"
#include "DHT11.h"
#include "UltraSound.h"
#include "DS1302.h"
#include "Button.h"
#include "STC_AD.h"
#include "FLASH.h"				 
//#include "Wifi.h"

#include "UART.h"

unsigned char code StrHello[]="HELLOWORLD!";
unsigned char DS3231ISREADY;  
unsigned char DHT11ISREADY;
unsigned char ULTRAISREADY;

unsigned char ADCSTARTREADY;
unsigned char ADCSTOPREADY;
unsigned char ADCDISPLAYREADY;

unsigned char FASTSPEED;
unsigned char BUZZERTIMER3STOP;
unsigned char ISSETTING;							//设置,1为设置状态 0为运行状态
unsigned char EXITSETTING;							//退出设置标志1为激活0为未激活

unsigned char UART2RIREADY;
unsigned char UART1RIREADY;

unsigned char WIFINEEDDELAY;
						  
unsigned char TEMPSTOP;
						  
unsigned char StartTime[7]=	{16,5,25,23,52,45,3};  	// 年月日时分秒周 14-11-30 23:50:59   DS1302起始时间
unsigned int FirstSetVelocity = 3600;			  	// 初始速度阈值约为3600m/h

void port_mode()            // 端口模式
{
	P0M1=0x00; P0M0=0x00;P1M1=0x00; P1M0=0x00;P2M1=0x00; P2M0=0x00;P3M1=0x00; P3M0=0x00;
	P4M1=0x00; P4M0=0x00;
	//P5M1=0x00; P5M0=0x00;P6M1=0x00; P6M0=0x00;P7M1=0x00; P7M0=0x00; 
}

void main()
{	
    
    //unsigned char code FUCKSTOP[]={"FUCKSTOOP!"};
	//unsigned char code FUCKSTART[]={"FUCKSTAART!"};

    port_mode();		  						//端口设置全部弱上拉
	UARTInit();		  							//UART初始化
	UART2Init();								//UART2初始化


    DS3231ISREADY = 0;							//DS1302未激活
	DHT11ISREADY = 0;	  						//DHT11未激活
	ULTRAISREADY = 0;	  						//超声波未激活,他们两个由Timer0中断激活
	FASTSPEED = 0;		  						//超速蜂鸣器未激活
	BUZZERTIMER3STOP = 0;
    ISSETTING = 0;
	EXITSETTING = 0;
	ADCSTARTREADY = 0;
	ADCSTOPREADY = 0;
	ADCDISPLAYREADY = 0;
	UART2RIREADY = 0;
	UART1RIREADY = 0;

	WIFINEEDDELAY = 0;
	//P20 = 1;
	/*//DS1302Init(StartTime);					//DS1302初始化,初始化一次就行了*/

	UltraSoundInit();	  						//超声波初始化
	ADC_P12_init();                             //ADC P12口初始化,MQ-135空气质量传感器
	SetVelocityThreshold(FirstSetVelocity,1);	//速度EEPROM初始化,可以自动识别是否初始化过
	//P20 = 0;
	
	//DHT11Init();        	//DHT11没有初始化程序
    
    
    Delay1000ms();		  	//延时3s
    Delay1000ms();   
    Lcm_Init();			  	//LCD12864以及初始化

    Timer3Init();         	//蜂鸣器定时器初始化但不开启,开启由变量FASTSPEED指示
	INT0_Init();            //按键外部中断0开启
	Timer0Init();		  	//DHT11和超声波     		  	
    Timer4Init();			//Wifi超时用定时器

	Display_String(1,StrHello);			   		    //LCD显示HelloWorld!
	Display_String(2,StrHello);			   		    //LCD显示HelloWorld!
	Display_String(3,StrHello);			   		    //LCD显示HelloWorld!
	Display_String(4,StrHello);			   		    //LCD显示HelloWorld!
	
	//P20 = 0;
	INT0_Init();
	EA = 1;				    //使能总中断
	
	//SendString2("AT+CIPSTART=\"TCP\",\"192.168.4.2\",8070\r\n");

	while(1)
	{	
		if(WIFINEEDDELAY)
		{
			if(UART2RIREADY)
			{
				Uart1SendUart2String();
				UART2RIREADY = 0;
				P20 = !P20;
			}
			if(UART1RIREADY)
			{
		    	//SendString2Length("AT+RST\r\n",8);
				Uart2SendUart1String();
				UART1RIREADY = 0;
				P25 = !P25;
			}
		}
		else
		{
		/**********************设置状态*****************************/
	    if(ISSETTING == 0)							
		{

        	if(BUZZERTIMER3STOP)
			{
				DisableTimer3();
				//Display_String(4,FUCKSTOP);
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
				    BUZZERTIMER3STOP = 0;
					EnableTimer3();
					SendString2("AT+CIPSEND=4\r\n");
					SendString2("BOOM");
					//Display_String(4,FUCKSTART);
				}
			}
			else if(DS3231ISREADY)
			{
				DS3231read_time();
				DS3231ISREADY = 0;
			}
			else if(ADCSTARTREADY)
			{
				ADCStart();
			}
			else if(ADCSTOPREADY)
			{
				ADCStop();
			}
			else if(ADCDISPLAYREADY)
			{
				ADCDisplay();
			}
			else ;
		}
		
		else
		{
			//WIFI在发送各数据时延时一会儿应该us级就行的
			//WIFINEEDDELAY = 1;
			//;
			
			EnterSetting();								 //这里设置成死循环!
		}

		}
	}
}
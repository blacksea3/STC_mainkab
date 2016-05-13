#include "stc15.h"
#include "Timer.h"
//#include "Temperature.h"
//#include "Wifi.h"
#include "Delay.h"
//#include "Force.h"
//#include "Camera.h"
#include "UART.h"

//static unsigned char StrHello[]="HELLOWORLD!";

void main()
{
    unsigned char i;
    //ForceInit();	                  //开一个中断
    Timer0Init();			          //定时器0初始化
	Timer1Init();					  //定时器1初始化
	EnableTimer1();
	Timer3Init();					  //定时器3初始化
	EnableTimer3();
 	Timer4Init();					  //定时器4初始化
	EnableTimer4();
	P20 = 0;
	P24 = 0;
	P25 = 0;
	P26 = 0;
	UARTInit();				          //Uart0初始化
	//CameraInit();                   //摄像头初始化
	EA = 1;
				   
	while(1)
	{
	    //Delay100ms();
	    
	    for(i=0;i<200;i++)
		{
			Delay100ms();
		}
		DisableTimer1();
		DisableTimer3();
		DisableTimer4();
		//P20 = !P20;
	    //SendString("A");
		
		//SendString(StrHello);
		
		//巴拉巴拉
	}
}
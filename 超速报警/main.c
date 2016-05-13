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
    //ForceInit();	                  //��һ���ж�
    Timer0Init();			          //��ʱ��0��ʼ��
	Timer1Init();					  //��ʱ��1��ʼ��
	EnableTimer1();
	Timer3Init();					  //��ʱ��3��ʼ��
	EnableTimer3();
 	Timer4Init();					  //��ʱ��4��ʼ��
	EnableTimer4();
	P20 = 0;
	P24 = 0;
	P25 = 0;
	P26 = 0;
	UARTInit();				          //Uart0��ʼ��
	//CameraInit();                   //����ͷ��ʼ��
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
		
		//��������
	}
}
#include "stc15.h"
#include "Timer.h"
#include "Delay.h"

#include "main.h"

#include "LCD12864.H"
#include "DHT11.h"
#include "UltraSound.h"
#include "DS1302.h"				 
//#include "Wifi.h"
//#include "Force.h"
//#include "Camera.h"
//#include "UART.h"

unsigned char code StrHello[]="HELLOWORLD!";
unsigned char DS1302ISREADY;  
unsigned char DHT11ISREADY;
unsigned char ULTRAISREADY;
unsigned char FASTSPEED;
unsigned char TIMER3STOP;

unsigned char StartTime[7]=	{14,11,30,23,50,59,7};  // ������ʱ������ 14-11-30 23:50:59   DS1302��ʼʱ��

void port_mode()            // �˿�ģʽ
{
	P0M1=0x00; P0M0=0x00;P1M1=0x00; P1M0=0x00;P2M1=0x00; P2M0=0x00;P3M1=0x00; P3M0=0x00;
	P4M1=0x00; P4M0=0x00;P5M1=0x00; P5M0=0x00;P6M1=0x00; P6M0=0x00;P7M1=0x00; P7M0=0x00; 
}

void main()
{	
    
    //unsigned char code FUCKSTOP[]={"FUCKSTOOP!"};
	//unsigned char code FUCKSTART[]={"FUCKSTAART!"};

    port_mode();		  	//�˿�����ȫ��������

    DS1302ISREADY = 0;    	//DS1302δ����
    DHT11ISREADY = 0;	  	//DHT11δ����
	ULTRAISREADY = 0;	  	//������δ����,����������Timer0�жϼ���
	FASTSPEED = 0;		  	//���ٷ�����δ����
	TIMER3STOP = 0;
    
	UltraSoundInit();	  	//��������ʼ��
	DS1302Init(StartTime);	//DS1302��ʼ��
	//DHT11Init();        	//DHT11û�г�ʼ������
    //UARTInit();		  	//UART��ʼ��
    
    Delay1000ms();		  	//��ʱ3s
    Delay1000ms();
    Delay1000ms();   
    Lcm_Init();			  	//LCD12864�Լ���ʼ��

    Timer3Init();         	//��������ʱ����ʼ����������,�����ɱ���FASTSPEEDָʾ

	Timer0Init();		  	//DHT11�ͳ�����
	EA = 1;     		  	//ʹ�����ж�
    //P20 = 0;

	Display_String(1,StrHello);			   		    //LCD��ʾHelloWorld!
	while(1)
	{	
        if(TIMER3STOP)
		{
			DisableTimer3();
			//Display_String(4,FUCKSTOP);
		}

		if(DHT11ISREADY)
		{
			DisplayDHT11();                         //��ȡ�¶�ʪ�ȴ�����ֵ
			DHT11ISREADY = 0;
		}
		else if(ULTRAISREADY)
		{
			UltraSoundDisplay();                    //������
			ULTRAISREADY = 0;
			if(FASTSPEED)
			{
			    TIMER3STOP = 0;
				EnableTimer3();
				//Display_String(4,FUCKSTART);
			}
		}
		else ;
	}
}
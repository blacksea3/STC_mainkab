#include "stc15.h"
#include "Timer.h"

#include "LCD12864.H"    //

#include "DHT11.h"				  //DHT11û�г�ʼ������

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

void port_mode()            // �˿�ģʽ
{
	P0M1=0x00; P0M0=0x00;P1M1=0x00; P1M0=0x00;P2M1=0x00; P2M0=0x00;P3M1=0x00; P3M0=0x00;
	P4M1=0x00; P4M0=0x00;P5M1=0x00; P5M0=0x00;P6M1=0x00; P6M0=0x00;P7M1=0x00; P7M0=0x00; 
}

void main()
{	
    //UARTInit();
    unsigned char code FUCKSTOP[]={"FUCKSTOOP!"};
	unsigned char code FUCKSTART[]={"FUCKSTAART!"};


    DHT11ISREADY = 0;	  //DHT11δ����
	ULTRAISREADY = 0;	  //������δ����,����������Timer0�жϼ���
	FASTSPEED = 0;		  //���ٷ�����δ����
	TIMER3STOP = 0;
    UltraSoundInit();	  //��������ʼ��

    port_mode();		  //�˿�����ȫ��������
    Delay1000ms();		  //��ʱ3s
    Delay1000ms();
    Delay1000ms();   
    Lcm_Init();			  //LCD12864�Լ���ʼ��

    Timer3Init();         //��������ʱ����ʼ����������,�����ɱ���FASTSPEEDָʾ

	Timer0Init();		  //DHT11�ͳ�����
	EA = 1;     		  //ʹ�����ж�
    P20 = 0;

	Display_String(1,StrHello);			   		    //LCD��ʾHelloWorld!
	while(1)
	{	
        if(TIMER3STOP)
		{
			DisableTimer3();
			Display_String(4,FUCKSTOP);
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
				Display_String(4,FUCKSTART);
			}
		}
		else ;
	}
}
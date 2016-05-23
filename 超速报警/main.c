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

void port_mode()            // �˿�ģʽ
{
	P0M1=0x00; P0M0=0x00;P1M1=0x00; P1M0=0x00;P2M1=0x00; P2M0=0x00;P3M1=0x00; P3M0=0x00;
	P4M1=0x00; P4M0=0x00;P5M1=0x00; P5M0=0x00;P6M1=0x00; P6M0=0x00;P7M1=0x00; P7M0=0x00; 
}

void main()
{	
    //UARTInit();

    DHT11ISREADY = 0;
	ULTRAISREADY = 0;

	UltraSoundInit();

    port_mode();
    Delay1000ms();
    Delay1000ms();
    Delay1000ms();   
    Lcm_Init();

	//Timer4Init();         //������  1s   �����ڲ�ʹ��Timer1
	//EnableTimer4();
	Timer0Init();		  //DHT11	2s
	EA = 1;     
    
	/*Delay1000ms();   	  // LCD�ϵ���ʱ
	
	port_mode();	      // ����IO����Ϊ׼˫����������ʽ��
	Delay1000ms();   	  // LCD�ϵ���ʱ
	Lcm_Init();
	*/
	Display_String(1,StrHello);			   
	while(1)
	{	
		if(DHT11ISREADY)
		{
			DisplayDHT11();                         //��ȡ�¶�ʪ�ȴ�����ֵ
			DHT11ISREADY = 0;
		}
		else if(ULTRAISREADY)
		{
			UltraSoundDisplay();                     //������
			ULTRAISREADY = 0;
		}
		else ;
	}
}
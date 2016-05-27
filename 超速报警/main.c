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
//#include "Force.h"
//#include "Camera.h"
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
unsigned char ISSETTING;							//����,1Ϊ����״̬ 0Ϊ����״̬
unsigned char EXITSETTING;							//�˳����ñ�־1Ϊ����0Ϊδ����

unsigned char StartTime[7]=	{16,5,25,23,52,0,3};  	// ������ʱ������ 14-11-30 23:50:59   DS1302��ʼʱ��
unsigned int FirstSetVelocity = 3600;			  	// ��ʼ�ٶ���ֵԼΪ3600m/h

void port_mode()            // �˿�ģʽ
{
	P0M1=0x00; P0M0=0x00;P1M1=0x00; P1M0=0x00;P2M1=0x00; P2M0=0x00;P3M1=0x00; P3M0=0x00;
	P4M1=0x00; P4M0=0x00;
	//P5M1=0x00; P5M0=0x00;P6M1=0x00; P6M0=0x00;P7M1=0x00; P7M0=0x00; 
}

void main()
{	
    
    //unsigned char code FUCKSTOP[]={"FUCKSTOOP!"};
	//unsigned char code FUCKSTART[]={"FUCKSTAART!"};

    port_mode();		  						//�˿�����ȫ��������

    DS3231ISREADY = 0;							//DS1302δ����
	DHT11ISREADY = 0;	  						//DHT11δ����
	ULTRAISREADY = 0;	  						//������δ����,����������Timer0�жϼ���
	FASTSPEED = 0;		  						//���ٷ�����δ����
	BUZZERTIMER3STOP = 0;
    ISSETTING = 0;
	EXITSETTING = 0;
	ADCSTARTREADY = 0;
	ADCSTOPREADY = 0;
	ADCDISPLAYREADY = 0;
	P20 = 1;
	UltraSoundInit();	  						//��������ʼ��
	ADC_P12_init();                             //ADC P12�ڳ�ʼ��,MQ-135��������������
	SetVelocityThreshold(FirstSetVelocity,1);	//�ٶ�EEPROM��ʼ��,�����Զ�ʶ���Ƿ��ʼ����
	
	//DS1302Init(StartTime);					//DS1302��ʼ��,��ʼ��һ�ξ�����
	
	//P20 = 0;
	
	//DHT11Init();        	//DHT11û�г�ʼ������
    //UARTInit();		  	//UART��ʼ��
    
    Delay1000ms();		  	//��ʱ3s
    Delay1000ms();   
    Lcm_Init();			  	//LCD12864�Լ���ʼ��

    Timer3Init();         	//��������ʱ����ʼ����������,�����ɱ���FASTSPEEDָʾ
	INT0_Init();            //�����ⲿ�ж�0����
	Timer0Init();		  	//DHT11�ͳ�����
	EA = 1;     		  	//ʹ�����ж�
    

	Display_String(1,StrHello);			   		    //LCD��ʾHelloWorld!
	Display_String(2,StrHello);			   		    //LCD��ʾHelloWorld!
	Display_String(3,StrHello);			   		    //LCD��ʾHelloWorld!
	Display_String(4,StrHello);			   		    //LCD��ʾHelloWorld!
	//P20 = 0;
	while(1)
	{	
		/**********************����״̬*****************************/
	    if(ISSETTING == 0)							
		{
        	if(BUZZERTIMER3STOP)
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
				    BUZZERTIMER3STOP = 0;
					EnableTimer3();
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
		/*********************************����״̬*****************************/
		else
		{
			EnterSetting();								 //�������ó���ѭ��!
		}
	}
}
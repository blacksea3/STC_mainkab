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
unsigned char ISSETTING;							//����,1Ϊ����״̬ 0Ϊ����״̬
unsigned char EXITSETTING;							//�˳����ñ�־1Ϊ����0Ϊδ����

unsigned char UART2RIREADY;
unsigned char UART1RIREADY;

//unsigned char SENDREADY;

unsigned char WIFINEEDDELAY;
unsigned char WifiDataSendReady;
unsigned char WifiDataSendType = 0;

						  
unsigned char TEMPSTOP;
						  
unsigned char StartTime[7]=	{16,5,25,23,52,45,3};  	// ������ʱ������ 14-11-30 23:50:59   DS1302��ʼʱ��
unsigned int FirstSetVelocity = 3600;			  	// ��ʼ�ٶ���ֵԼΪ3600m/h

void port_mode()            // �˿�ģʽ
{
	P0M1=0x00; P0M0=0x00;P1M1=0x00; P1M0=0x00;P2M1=0x00; P2M0=0x00;P3M1=0x00; P3M0=0x00;
	P4M1=0x00; P4M0=0x00;
	//P5M1=0x00; P5M0=0x00;P6M1=0x00; P6M0=0x00;P7M1=0x00; P7M0=0x00; 
}

void main()
{	
    //unsigned char TimeStr[]={0,0,0,0,0,0,'\0'};
    //unsigned char code FUCKSTOP[]={"FUCKSTOOP!"};
	//unsigned char code FUCKSTART[]={"FUCKSTAART!"};

    port_mode();		  						//�˿�����ȫ��������
	UARTInit();		  							//UART��ʼ��
	UART2Init();								//UART2��ʼ��
    
	P44 = 1;									//�������ر�
	P54 = 1;									//�������ر�
	P55 = 1;									//�������ر�

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
	UART2RIREADY = 0;
	UART1RIREADY = 0;
	//WifiDataSendReady = 0;
	WIFINEEDDELAY = 0;
    //SENDREADY = 0;
	WifiDataSendType = 0;
	//P20 = 1;
	//DS1302Init(StartTime);						//DS1302��ʼ��,��ʼ��һ�ξ�����

	
	//UltraSoundInit();	  						//��������ʼ��
	Timer1Init();								//��������ʱ��ʱ��				  /
	ADC_P12_init();                             //ADC P12�ڳ�ʼ��,MQ-135��������������
	SetVelocityThreshold(FirstSetVelocity,1);	//�ٶ�EEPROM��ʼ��,�����Զ�ʶ���Ƿ��ʼ����
	
    Delay1000ms();		  	//��ʱ3s
    Delay1000ms();   
    Lcm_Init();			  	//LCD12864�Լ���ʼ��

    Timer3Init();         	//��������ʱ����ʼ����������,�����ɱ���FASTSPEEDָʾ
	INT0_Init();            //�����ⲿ�ж�0����
	Timer0Init();		  	//DHT11�ͳ�����
	     		  	
    //Timer4Init();			//Wifi��ʱ�ö�ʱ��

	Display_String(1,StrHello);			   		    //LCD��ʾHelloWorld!
	Display_String(2,StrHello);			   		    //LCD��ʾHelloWorld!
	Display_String(3,StrHello);			   		    //LCD��ʾHelloWorld!
	Display_String(4,StrHello);			   		    //LCD��ʾHelloWorld!
	
	P15 = 1;
	P14 = 1;
	//P20 = 0;
	INT0_Init();
	EA = 1;				    //ʹ�����ж�
	
	//SendString2("AT+CIPSTART=\"TCP\",\"192.168.4.2\",8070\r\n");
	
	while(1)
	{
		if(WIFINEEDDELAY)
		{
			if(WifiDataSendType != 0)
			{
				if(WifiDataSendType == 1)
				{
					SendString2("AT+CIPSEND=0,17\r\n");
					WifiDataSendType = 2;
					//WIFINEEDDELAY = 1;
					P14 = 0;
				}
				else
				{
					Delay5Ms();
					DS1302SendTimeByWifi();
					WifiDataSendType = 0;
					WIFINEEDDELAY = 0;
					P15 = 0;
				}	
			}
			else
			{
				if(UART2Temp)
				{
					Uart1SendUart2String();
					UART2Temp = 0;	
				}
				else if(UART1Temp)
				{
					Uart2SendUart1String();
					UART1Temp = 0;
				}	
				else
				{
					;
				}				
			}
		}
		else
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

						//DisableTimer0();
					    //DisableTimer3();
						//SENDREADY = 1;
						SendString("����");
						SendString("ʱ��Ϊ");
						DS1302SendTimeByUART();

						WIFINEEDDELAY = 1;
						WifiDataSendType = 1;								 	//Wifi����ʱ��,������
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
				//WIFI�ڷ��͸�����ʱ��ʱһ���Ӧ��us�����е�
				//WIFINEEDDELAY = 1;
				//;
				
				EnterSetting();								 //�������ó���ѭ��!
			}
        }
		//}
	}
}
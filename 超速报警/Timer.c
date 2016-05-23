#include "stc15.h"
//#include "Temperature.h"
//#include "UART.h"
//#include "DHT11.h"
#include "Buzzer.h"
//#include "UltraSound.h"
#include "main.h"

#define TFOSC 11059200L
#define TIMS (65536-TFOSC/1000)  //1Tģʽ

unsigned int T0times0;     	//��ʱ��0�жϴ���0
unsigned int T0times1;     	//��ʱ��0�жϴ���1
unsigned int T1times;     	//��ʱ��1�жϴ���
unsigned int T3times;     	//��ʱ��3�жϴ���
unsigned int T4times;     	//��ʱ��4�жϴ���
unsigned int T3TimesValue;	//��ʱ��3�ж���ֵ
//sbit P20 = P2^0;

//�̶���ʱ��Timer0
void Timer0Init(void)		//1����@11.0592MHz
{
	AUXR |= 0x80;		    //��ʱ��0ʱ��1Tģʽ
	TMOD &= 0xfc;			//���ö�ʱ��0 16λ�Զ���װ��
	TL0 = TIMS;				//���ö�ʱ��ֵ
	TH0 = TIMS>>8;			//���ö�ʱ��ֵ
	PT0 = 0;                //Timer0�����ȼ�
	TR0 = 1;		        //��ʱ��0��ʼ��ʱ
	ET0 = 1;            	//ʹ�ܶ�ʱ��0�ж�
	T0times0 = 0;
	T0times1 = 0;
	P25 = 1;
	P24 = 1;
}

//Timer0�жϺ���
void Timer0_ISR() interrupt 1      			    
{
    //EA = 0;
	if(T0times0<2000)
	{
	    T0times0++;
	}
	else
	{
	    T0times0=0;
		DHT11ISREADY = 1;
		
	}
	if(T0times1<500)
	{
	    T0times1++;
	}
	else
	{
	    T0times1=0;
		ULTRAISREADY = 1;
		
	}
	
	//EA = 1;
}

//��ʱ��ʱ��Timer1
void Timer1Init(void)		//@11.0592MHz		340m/s  4m  	 340t=2x    x<=4m    t=x/170<=0.02352941s   65535*1/11059200
{									            //������Ŀ�����,��Ҫ����жϴ�������T1Times
	AUXR |= 0x40;		                        //��ʱ��1ʱ��1Tģʽ
	TMOD &= 0xf3;								//���ö�ʱ��1 16λ�Զ���װ��
	TR1 = 0;		        					//��ʱ��1ֹͣ��ʱ
	ET1 = 0;            						//�رն�ʱ��1�ж�
}

void EnableTimer1()
{
	TL1 = 0;									//���ö�ʱ��ֵ
	TH1 = 0>>8;			    					//���ö�ʱ��ֵ
	TR1 = 1;		        					//��ʱ��1��ʼ��ʱ
	ET1 = 1;            						//ʹ�ܶ�ʱ��1�ж�
	T1times = 0;
}

void DisableTimer1()
{
	TR1 = 0;		        //��ʱ��1ֹͣ��ʱ
	ET1 = 0;            	//�رն�ʱ��1�ж�	
}

//Timer1�жϺ���
void Timer1_ISR() interrupt 3       			//���ڷ�������ʹ�ô����ж�,���������ڱ���ж���ֱ�ӷ�����
{
    //EA = 0;
	if(T1times<1000)
	{	
		T1times++;
	}
	else
	{
		T1times=0;
	}
	//EA = 1;
	if(T1times == 4)
	{
		;
		//SendData('?');
	}
}

//�ɱ䶨ʱ��Timer3
void Timer3Init(void)		//1����@11.0592MHz
{
	T4T3M &= 0xfb;		    //��ʱ��3�ڲ�ϵͳʱ��
	T4T3M |= 0x02;          //��ʱ��3 1Tģʽ
	T4T3M &= 0xf7;		    //��ʱ��3ֹͣ��ʱ
	IE2 &= 0xdf;            //�رն�ʱ��3�ж�
}

void EnableTimer3()
{
	T4T3M |= 0x08;		    //��ʱ��3��ʼ��ʱ
	IE2 |= 0x20;            //������ʱ��3�ж�
	T3L = TIMS;				//���ö�ʱ��ֵ
	T3H = TIMS>>8;			//���ö�ʱ��ֵ
	T3times = 0;
	T3TimesValue = 400;
}

void DisableTimer3()
{
	T4T3M &= 0xf7;		    //��ʱ��3ֹͣ��ʱ
	IE2 &= 0xdf;            //�رն�ʱ��3�ж�	
}

//Timer3�жϺ���
void Timer3_ISR() interrupt 19       			//���ڷ�������ʹ�ô����ж�,���������ڱ���ж���ֱ�ӷ�����
{
    EA = 0;
	if(T3times<T3TimesValue)
	{	
	    Buzzer();								//������
		T3times++;
	}
	else
	{
	    P55 = 0;
	    FASTSPEED = 0;						
	    TIMER3STOP = 1;
		T3times=0;
	}
	EA = 1;
}

//�ɱ䶨ʱ��Timer4
void Timer4Init(void)		//1����@11.0592MHz
{
	T4T3M &= 0xbf;		    //��ʱ��4�ڲ�ϵͳʱ��
	T4T3M |= 0x20;          //��ʱ��4 1Tģʽ
	T4L = TIMS;				//���ö�ʱ��ֵ
	T4H = TIMS>>8;			//���ö�ʱ��ֵ
	T4T3M &= 0x7f;		    //��ʱ��4ֹͣ��ʱ
	IE2 &= 0xbf;            //�رն�ʱ��4�ж�
	P25 = 0;
}

void EnableTimer4()
{
	T4T3M |= 0x80;		    //��ʱ��4��ʼ��ʱ
	IE2 |= 0x40;            //������ʱ��4�ж�
	T4times = 0;
}

void DisableTimer4()
{
	T4T3M &= 0x7f;		    //��ʱ��4ֹͣ��ʱ
	IE2 &= 0xbf;            //�رն�ʱ��4�ж�	
}

//Timer4�жϺ���
void Timer4_ISR() interrupt 20       			//���ڷ�������ʹ�ô����ж�,���������ڱ���ж���ֱ�ӷ�����
{
    //EA = 0;
	if(T4times<1000)
	{	
		T4times++;
	}
	else
	{
		T4times=0;
		P25=!P25;
		//SendString("ABCDE");
		//ReadTemperature();                    //��ȡ�¶�ʪ�ȴ�����ֵ
		//ReadAir();                            //��ȡ����������ֵ
		//WifiSend();                           //Wifi��������
	}
	//EA = 1;
}

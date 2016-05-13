#include "stc15.h"
//#include "Temperature.h"
#include "UART.h"

#define TFOSC 11059200L
#define TIMS (65536-TFOSC/1000)  //1Tģʽ

unsigned int T0times;     	//��ʱ��0�жϴ���
unsigned int T1times;     	//��ʱ��1�жϴ���
unsigned int T3times;     	//��ʱ��3�жϴ���
unsigned int T4times;     	//��ʱ��4�жϴ���
//sbit P20 = P2^0;

//�̶���ʱ��Timer0
void Timer0Init(void)		//1����@11.0592MHz
{
	AUXR |= 0x80;		    //��ʱ��0ʱ��1Tģʽ
	TMOD &= 0xfc;			//���ö�ʱ��0 16λ�Զ���װ��
	TL0 = TIMS;				//���ö�ʱ��ֵ
	TH0 = TIMS>>8;			//���ö�ʱ��ֵ
	TR0 = 1;		        //��ʱ��0��ʼ��ʱ
	ET0 = 1;            	//ʹ�ܶ�ʱ��0�ж�
	T0times = 0;
}

//Timer0�жϺ���
void Timer0_ISR() interrupt 1 using 2			//���ڷ�������ʹ�ô����ж�,���������ڱ���ж���ֱ�ӷ�����
{
    //EA = 0;
	if(T0times<500)
	{	
		T0times++;
	}
	else
	{
	    P20 = !P20;
		T0times=0;
		//SendString("ABCDE");
		//ReadTemperature();                    //��ȡ�¶�ʪ�ȴ�����ֵ
		//ReadAir();                            //��ȡ����������ֵ
		//WifiSend();                           //Wifi��������
	}
	//EA = 1;
}

//�ɱ䶨ʱ��Timer1
void Timer1Init(void)		//1����@11.0592MHz
{
	AUXR |= 0x40;		    //��ʱ��1ʱ��1Tģʽ
	TMOD &= 0xf3;			//���ö�ʱ��1 16λ�Զ���װ��
	TL1 = TIMS;				//���ö�ʱ��ֵ
	TH1 = TIMS>>8;			//���ö�ʱ��ֵ
	TR1 = 0;		        //��ʱ��1ֹͣ��ʱ
	ET1 = 0;            	//�رն�ʱ��1�ж�
}

void EnableTimer1()
{
	TR1 = 1;		        //��ʱ��1��ʼ��ʱ
	ET1 = 1;            	//ʹ�ܶ�ʱ��1�ж�
	T1times = 0;
}

void DisableTimer1()
{
	TR1 = 0;		        //��ʱ��1ֹͣ��ʱ
	ET1 = 0;            	//�رն�ʱ��1�ж�	
}

//Timer1�жϺ���
void Timer1_ISR() interrupt 3 using 2			//���ڷ�������ʹ�ô����ж�,���������ڱ���ж���ֱ�ӷ�����
{
    //EA = 0;
	if(T1times<1000)
	{	
		T1times++;
	}
	else
	{
		P24 = !P24;
		T1times=0;
		//SendString("ABCDE");
		//ReadTemperature();                    //��ȡ�¶�ʪ�ȴ�����ֵ
		//ReadAir();                            //��ȡ����������ֵ
		//WifiSend();                           //Wifi��������
	}
	//EA = 1;
}

//�ɱ䶨ʱ��Timer3
void Timer3Init(void)		//1����@11.0592MHz
{
	T4T3M &= 0xfb;		    //��ʱ��3�ڲ�ϵͳʱ��
	T4T3M |= 0x02;          //��ʱ��3 1Tģʽ
	T3L = TIMS;				//���ö�ʱ��ֵ
	T3H = TIMS>>8;			//���ö�ʱ��ֵ
	T4T3M &= 0xf7;		    //��ʱ��3ֹͣ��ʱ
	IE2 &= 0xdf;            //�رն�ʱ��3�ж�
}

void EnableTimer3()
{
	T4T3M |= 0x08;		    //��ʱ��3��ʼ��ʱ
	IE2 |= 0x20;            //������ʱ��3�ж�
	T3times = 0;
}

void DisableTimer3()
{
	T4T3M &= 0xf7;		    //��ʱ��3ֹͣ��ʱ
	IE2 &= 0xdf;            //�رն�ʱ��3�ж�	
}

//Timer3�жϺ���
void Timer3_ISR() interrupt 19 using 2			//���ڷ�������ʹ�ô����ж�,���������ڱ���ж���ֱ�ӷ�����
{
    //EA = 0;
	if(T3times<2000)
	{	
		T3times++;
	}
	else
	{
		P25 = !P25;
		T3times=0;
		//SendString("ABCDE");
		//ReadTemperature();                    //��ȡ�¶�ʪ�ȴ�����ֵ
		//ReadAir();                            //��ȡ����������ֵ
		//WifiSend();                           //Wifi��������
	}
	//EA = 1;
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
void Timer4_ISR() interrupt 20 using 2			//���ڷ�������ʹ�ô����ж�,���������ڱ���ж���ֱ�ӷ�����
{
    //EA = 0;
	if(T4times<4000)
	{	
		T4times++;
	}
	else
	{
		P26 = !P26;
		T4times=0;
		//SendString("ABCDE");
		//ReadTemperature();                    //��ȡ�¶�ʪ�ȴ�����ֵ
		//ReadAir();                            //��ȡ����������ֵ
		//WifiSend();                           //Wifi��������
	}
	//EA = 1;
}

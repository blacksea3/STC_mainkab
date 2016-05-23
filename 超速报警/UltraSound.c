#include "stc15.h"
#include "UART.h"
#include "Timer.h"
#include "Delay.h"
#include "LCD12864.h"

sbit trig=P1^0;	//�������� ����Ϊ10us����
sbit echo=P1^1;	//��������ź�

unsigned long RawTime;
signed int RealDistance;

#define FOSC_FENZHIYI 9.0422e-5                	//�ȳ�1000,����ֱ�ӵ��ɺ���

char bat[7]; //���泬�������������ݣ���λΪm   	//1������9.0422e-8   1000��e-5    RawTime*FOSC_FENZHIYI s
void delay(unsigned int i)
{
	while(i--);
}

void UltraSoundInit()
{
	Timer1Init();
}

void super_start()	//������������ʼ����
{
	trig=1;
	delay(10);	 //����10us
	trig=0;	
}
signed int super_count()	  //���㳬��������ĸߵ�ƽ����ʱ��,ֱ��ȡ��TH1�Ͷ�TL1		 0-25m/s
{
    RawTime=65536*T1times+TH1*256+TL1;
	RealDistance=(RawTime*FOSC_FENZHIYI*170);   //��ȷ��mm								 3m         1/11059200 s  ����1m ��t=2m/340m/s  
					  															 //0-4000	   pp * 	 9.0422e-8 s * 170m/s = 4000mm
	//bat[0]=RealDistance/1000 + 48;
	//bat[1]=(unsigned char)('.');
	//bat[2]=RealDistance%1000/100 + 48;
	//bat[3]=RealDistance%100/10 + 48;
	//bat[4]=RealDistance%10 + 48;
	//bat[5]=(unsigned char)('m');
	//bat[6]=(unsigned char)('m');
	//bat[7]='\0';
	return RealDistance;
}

signed int DisUltraSound()
{
    signed int t;
	super_start();
	while(echo==0);	  //�ȴ��ߵ�ƽ
	EnableTimer1();	  //�򿪶�ʱ��1��ʼ��ʱ
	while(echo==1);	  //�ȴ��ߵ�ƽ��ȥ��ɵ͵�ƽ���ߵ�ƽ������
	DisableTimer1();  //�رն�ʱ��1
	t=super_count();
	return t;
}

void UltraSoundDisplay()
{     
    //unsigned char bat1[8]={0,0,0,0,0,0};
	//unsigned char bat2[8]={0,0,0,0,0,0};
	//unsigned char bat3[10]={'+',0,'.',0,0,0,'m','/','s','\0'};
	  
	unsigned char VelocityData[] = {"�ٶ�Ϊ000000000"};
	unsigned long Dis1,Dis2,DDis;
	unsigned char i;

	//for(i=0;i<5;i++)
    //{
	//	Delay10ms();
	//}
    Dis1=DisUltraSound();          	  //������ģ����
    for(i=0;i<5;i++)
	{
		Delay50ms();
	}
    Dis2=DisUltraSound();          	  //������ģ����

    if(Dis1>4000 || Dis2>4000)								  //��λmm
	{
		VelocityData[7] = 'N';
		VelocityData[8] = '/';
		VelocityData[9] = 'A';
		VelocityData[10] = 'N';
		VelocityData[11] = '/';
		VelocityData[12] = 'A';
		VelocityData[13] = 'N';
		VelocityData[14] = 'A';
		VelocityData[15]='\0';

		Display_String(1,VelocityData);
	}
	else
	{
	//bat1[0]=Dis1/10000 + 48;
	//bat1[1]=Dis1%10000/1000 + 48;
	/*bat1[0]=Dis1%10000/1000 + 48;
	bat1[1]=(unsigned char)('.');								
	bat1[2]=Dis1%1000/100 + 48;
	bat1[3]=Dis1%100/10 + 48;
	bat1[4]=Dis1%10 + 48;
	bat1[5]=(unsigned char)('m');
	bat1[6]='\0';
    Display_String(2,bat1);

    Delay1ms();
																	 
	bat2[0]=Dis2%10000/1000 + 48;
	bat2[1]=(unsigned char)('.');								
	bat2[2]=Dis2%1000/100 + 48;
	bat2[3]=Dis2%100/10 + 48;
	bat2[4]=Dis2%10 + 48;
	bat2[5]=(unsigned char)('m');
	bat2[6]='\0';
	Display_String(3,bat2);

    Delay1ms();*/

    if(Dis1>Dis2)
	{
		DDis=Dis1-Dis2;
		VelocityData[6] = '+';
	}
	else
	{
		DDis=Dis2-Dis1;
		VelocityData[6] = '-';
	}
														   //��λmm  ʱ�� 50ms      4000mm/50ms  ->  4000*20=80m/s
	DDis*=4;
	
	VelocityData[7]=DDis%10000/1000 + 48;
	VelocityData[8]=(unsigned char)('.');							
	VelocityData[9]=DDis%1000/100 + 48;
	VelocityData[10]=DDis%100/10 + 48;
	VelocityData[11]=DDis%10 + 48;
	VelocityData[12]=(unsigned char)('m');
	VelocityData[13]=(unsigned char)('/');
	VelocityData[14]=(unsigned char)('s');
	VelocityData[15]='\0';

	Display_String(1,VelocityData);
	}	
}
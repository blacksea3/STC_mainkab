#include "stc15.h"
#include "Delay.h"
#include "LCD12864.H"
#include "main.h"
#include "Timer.h"
#include "Button.h"

unsigned char code SetString[] = {"���ã�"};

void INT0_Init()
{	
	IT0=1;	               				// ����Ϊ�½��ش���
	EX0=1;                 				// ���ⲿ�ж�0			  P3.2 INT0
}
void EXT0(void) interrupt 0
{
    Delay20ms();
	Delay20ms();
	if(P32==0)							// ��Ȼ�ǰ���״̬
	{
		if(ISSETTING==0)
		{ 
		    P20 = !P20;
			ISSETTING=1;
			DisableTimer0();			// �رճ����� ��ʪ�ȵĶ�ʱ���ж�
			DisableTimer3();			// �رշ������Ķ�ʱ���ж�
		}
		else
		{
		    //P26 = !P26;
		    ISSETTING=0;
			EXITSETTING=1; 
			EnableTimer0();				// �رճ����� ��ʪ�ȵĶ�ʱ���ж�
			EnableTimer3();				// �رշ������Ķ�ʱ���ж�			
		}
	}
	else								// ����
	{
		; 
	} 	
}

void EnterSetting()
{
    //unsigned char VelocityUp[] = {"�ٶ����ޣ�   m/s"};
	//LCD12864ClearScreen();
	//unsigned char Velocity1 = (unsigned char)(0 + 48);
	//unsigned char Velocity2 = (unsigned char)(0 + 48);
	//unsigned char VeloCitySetting1[] = {"�ٶ����ޣ�0."};
   	//unsigned char VeloCitySetting2[] = {"�ٶ����ޣ�0.0m"};
	unsigned char Loc = 1;

	unsigned char LeftVelocityValue = 0;
	unsigned char RightVelocityValue = 0;

    LCD12864SettingInit();
	//Display_String(1,SetString);
	//Display_String(2,VelocityUp);

	 
	do	                             			   	//������̰���ɨ��
	{
	    KeyOut1 = 0;
		if(KeyIn1 == 0)									   	//��������
		{  	
    		Delay20ms();
			Delay20ms();
			if(KeyIn1 == 1) continue;
			//sbit KeyOut1 = P3^5;
//sbit KeyIn1  = P3^4;
//sbit KeyIn2  = P3^6;
			P20 = !P20;
			if(Loc == 1)						   
			{
				Loc = 2;
				//Display_String(1,VeloCitySetting2);
				LCD12864DisplayChar(1,6,RightVelocityValue + 48);
			}
			else
			{
				Loc = 1;
				LCD12864DisplayChar(1,4,LeftVelocityValue + 48);
				//Display_String(1,VeloCitySetting1);				
			}
			while(KeyIn1==0);
		}
		else										//����ֵ
		{
    		Delay20ms();
			Delay20ms();
			if(KeyIn2 == 1) continue;
			if(Loc == 1)						   
			{
			    if(LeftVelocityValue==9)
				{	
				    LeftVelocityValue = 0;
			    	LCD12864DisplayChar(1,4,0+48);
				}
				else
				{
					LCD12864DisplayChar(1,4,++LeftVelocityValue+48);
				}
			}
			else
			{
			    if(RightVelocityValue==9)
				{	
				    RightVelocityValue = 0;
			    	LCD12864DisplayChar(1,6,0+48);
				}
				else
				{
					LCD12864DisplayChar(1,6,++RightVelocityValue+48);
				}				
			}
			while(KeyIn2==0);			
		}
	}	
	while(EXITSETTING == 0);					   	//��û���˳�ѭ��ʱ������
	LCD12864SettingExit();
	EXITSETTING = 0;
}
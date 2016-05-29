#include "stc15.h"
#include "Delay.h"
#include "LCD12864.H"
#include "main.h"
#include "Timer.h"
#include "Button.h"
#include "FLASH.h"

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
    unsigned char VeloCitySettingFull1[] = {"�ٶ����ޣ�"};
	unsigned char VeloCitySettingFull2[] = {"6 . 6 6 km/h"};
	unsigned int TempVelocity;				                			//��ʱ����
	unsigned char VelocityValue1 = 0;									//��λ
	unsigned char VelocityValue2 = 0;									//ʮ��֮һλ
	unsigned char VelocityValue3 = 0; 				  					//�ٷ�֮һλ
    unsigned char Loc = 1;
	//unsigned char VelocityUp[] = {"�ٶ����ޣ�   m/s"};
	//LCD12864ClearScreen();
	//unsigned char Velocity1 = (unsigned char)(0 + 48);
	//unsigned char Velocity2 = (unsigned char)(0 + 48);
	//unsigned char VeloCitySetting1[] = {"�ٶ����ޣ�0."};
   	//unsigned char VeloCitySetting2[] = {"�ٶ����ޣ�0.0m"};
	
    LCD12864SettingInit();
	//Display_String(1,SetString);
	//Display_String(2,VelocityUp);
	Display_String(1,VeloCitySettingFull1);
	Display_String(2,VeloCitySettingFull2);

    TempVelocity = GetVelocityThreshold();
	//SendData(VelocityValue1);
	//SendData(VelocityValue1);
	//SendData(VelocityValue1);
	VelocityValue1 = TempVelocity%10000/1000;
	VelocityValue2 = TempVelocity%1000/100;
	VelocityValue3 = TempVelocity%100/10;  
	
	//LCD12864DisplayChar(2,2,'.');
	LCD12864DisplayChar(2,3,VelocityValue2 + 48);
	LCD12864DisplayChar(2,4,VelocityValue3 + 48);
	LCD12864DisplayChar(2,1,VelocityValue1 + 48);
	 
	do	                             			             	      	//������̰���ɨ��
	{
	    KeyOut1 = 0;
		KeyOut2 = 1;
		Delay20ms();
		if(KeyIn1 == 0)									   	       		//��������
		{  	
    		Delay20ms();
			Delay20ms();
			if(KeyIn1 == 1) continue;
			//P20 = !P20;
			if(Loc == 1)						   
			{
				Loc = 2;
				LCD12864DisplayChar(2,3,VelocityValue2 + 48);
			}
			else if(Loc == 2)
			{
				Loc = 3;
				LCD12864DisplayChar(2,4,VelocityValue3 + 48);				
			}
			else
			{
				Loc = 1;
				LCD12864DisplayChar(2,1,VelocityValue1 + 48);
			}
			while(KeyIn1==0);
		}

		if(KeyIn2 == 0)									   	       		//��������
		{  	
    		Delay20ms();
			Delay20ms();
			if(KeyIn2 == 1) continue;
			//P20 = !P20;
			if(Loc == 3)						   
			{
				Loc = 2;
				LCD12864DisplayChar(2,3,VelocityValue2 + 48);
			}
			else if(Loc == 1)
			{
				Loc = 3;
				LCD12864DisplayChar(2,4,VelocityValue3 + 48);				
			}
			else
			{
				Loc = 1;
				LCD12864DisplayChar(2,1,VelocityValue1 + 48);
			}
			while(KeyIn2==0);
		}
																	//����ֵ
	    KeyOut1 = 1;
		KeyOut2 = 0;
		Delay20ms();
 		if(KeyIn1 == 0)
		{
    		Delay20ms();
			Delay20ms();
			if(KeyIn1 == 1) continue;
			if(Loc == 1)						   
			{
		    	if(VelocityValue1==0)
				{	
				    VelocityValue1 = 9;
		    		LCD12864DisplayChar(2,1,VelocityValue1 + 48);
					}
				else
				{
					LCD12864DisplayChar(2,1,--VelocityValue1 + 48);
				}
			}
			else if(Loc == 2)
			{
			    if(VelocityValue2==0)
				{	
				    VelocityValue2 = 9;
			    	LCD12864DisplayChar(2,3,VelocityValue2 + 48);
				}
				else
				{
					LCD12864DisplayChar(2,3,--VelocityValue2 + 48);
				}				
			}
			else
			{
			    if(VelocityValue3==0)
				{	
				    VelocityValue3 = 9;
			    	LCD12864DisplayChar(2,4,VelocityValue3 + 48);
				}
				else
				{
					LCD12864DisplayChar(2,4,--VelocityValue3 + 48);
				}										   
			}
			while(KeyIn1==0);
		}

		if(KeyIn2 == 0)
		{
    		Delay20ms();
			Delay20ms();
			if(KeyIn2 == 1) continue;
			if(Loc == 1)						   
			{
		    	if(VelocityValue1==9)
				{	
				    VelocityValue1 = 0;
		    		LCD12864DisplayChar(2,1,VelocityValue1 + 48);
				}
				else
				{
					LCD12864DisplayChar(2,1,++VelocityValue1 + 48);
				}
			}
			else if(Loc == 2)
			{
			    if(VelocityValue2==9)
				{	
				    VelocityValue2 = 0;
			    	LCD12864DisplayChar(2,3,VelocityValue2 + 48);
				}
				else
				{
					LCD12864DisplayChar(2,3,++VelocityValue2 + 48);
				}				
			}
			else
			{
			    if(VelocityValue3==9)
				{	
				    VelocityValue3 = 0;
			    	LCD12864DisplayChar(2,4,VelocityValue3 + 48);		    
				}
				else
				{
					LCD12864DisplayChar(2,4,++VelocityValue3 + 48);
				}										   
			}
			while(KeyIn2==0);
		}			
	}	
	while(EXITSETTING == 0);					   	//��û���˳�ѭ��ʱ������
	
	TempVelocity = 1000*VelocityValue1 + 100*VelocityValue2 + 10*VelocityValue3;	//�����ٶ���ֵ
	SetVelocityThreshold(TempVelocity,0);         	//�����ٶ���ֵ
	LCD12864SettingExit();
	EXITSETTING = 0;
}
#include "stc15.h"
#include "intrins.h"
#include "Delay.h"

#define uchar unsigned char 
#define LCMdata P0 
#define Busy 0x80  

sbit RS=P2^7;  //P1.3
sbit RW=P2^6;  //P1.2
sbit E=P2^5;   //P1.1  
sbit REST=P2^4;  //P2.0

void Display_String(uchar line,uchar *string); 
void Wr_Data(uchar wrdata); //д����   
void Wr_Command(uchar wrcommand,busyc); //дָ�� 
void RDbf(void);//��æ״̬ 
void Lcm_Init(void);//Һ����ʼ��		  

void Display_String(uchar line,uchar *string)//��ʾ�ַ��� 
{   
uchar addr;  
if(line==1)   addr=0x80;  
else if(line==2)   addr=0x90;  
else if(line==3)   addr=0x88;   
else if(line==4)   addr=0x98;      
Wr_Command(addr,1);  
while(*string)
{
	Wr_Data(*string++); 
}
    
}

void Wr_Data(uchar wrdata) //д���� 
{  
RDbf();  
RS=1;  
RW=0;
Delay5us();  
E=1;  
LCMdata=wrdata;
Delay5us();  
E=0;  
}  

void Wr_Command(uchar wrcommand,busyc) //дָ�� 
{  
if(busyc)   RDbf();  
RS=0;  
RW=0;
Delay5us();  
E=1;  
LCMdata=wrcommand;
Delay5us();  
E=0; 
}  

void RDbf(void) //��æ״̬ 
{ 
while(1)  
{   
RS=0;    
RW=1; 
E=0;   
LCMdata=0xFF;     
E=1;   
if((LCMdata&Busy)==0)   break;  
} 
}

void LCD12864DisplayChar(unsigned char line, unsigned char column, unsigned char Str)
{	
	switch(line)
	{
		case 1:
		{   
			Wr_Command(0x80 + column -1 ,1);
			break;  
		}
		case 2:
		{
			Wr_Command(0x90 + column -1 ,1);
			break;
		}
		case 3:
		{
			Wr_Command(0x88 + column -1 ,1);
			break;  
		}
		case 4:
		{
			Wr_Command(0x98 + column -1 ,1);
			break;
		}
		default:
		{
			;
		}	
	}
	
	Wr_Data(Str);
}

void LCD12864SettingInit()
{
    unsigned char VeloCitySettingFull[] = {"v ����0 . 0 m/s "};				  //���ڲ�֪��ô�����в�����ÿ�ζ�ˢ������

   	//unsigned char VeloCityFullSetting[] = {"�ٶ����ޣ�0.0m/s"};

	Wr_Command(0x30,0);  
	Delay5Ms();  
	Wr_Command(0x30,0); //2 ����ʾģʽ���ã�������æ  
	Delay5Ms();
	Wr_Command(0x0d,1);	//������ʾON �α�ON �α�λ��OFF
	Delay5Ms();	  
	//Wr_Command(0x0C,1); //����ʾ���������  			  
	//Delay5Ms(); 
	Wr_Command(0x01,1); //��ʾ����  
	Delay5Ms();  
	Delay5Ms();  
	Wr_Command(0x06,1); //��ʾ����ƶ�����  
	Delay5Ms();
	Display_String(1,VeloCitySettingFull);
	LCD12864DisplayChar(1,4,0+48);	
}

void LCD12864SettingExit()
{
	Wr_Command(0x30,0);  
	Delay5Ms();  
	Wr_Command(0x30,0); //2 ����ʾģʽ���ã�������æ  
	Delay5Ms();  
	Wr_Command(0x0C,1); //������ʾON �α�OFF �α�λ��OFF   			  
	Delay5Ms(); 
	Wr_Command(0x01,1); //��ʾ����  
	Delay5Ms();  
	Delay5Ms();  
	Wr_Command(0x06,1); //��ʾ����ƶ�����  
	Delay5Ms();	
}

void Lcm_Init(void) 
{  
	Delay5Ms();     
	REST=1;     
	REST=0;     
	REST=1;   
	Wr_Command(0x30,0);  
	Delay5Ms();  
	Wr_Command(0x30,0); //2 ����ʾģʽ���ã�������æ  
	Delay5Ms();  
	Wr_Command(0x0C,1); //������ʾON �α�OFF �α�λ��OFF  			  
	Delay5Ms(); 
	Wr_Command(0x01,1); //��ʾ����  
	Delay5Ms();  
	Delay5Ms();  
	Wr_Command(0x06,1); //��ʾ����ƶ�����  
	Delay5Ms(); 
}


#include "stc15.h"
#include "Delay.h"
#include "LCD12864.H"
#include "main.h"
#include "Timer.h"
#include "Button.h"
#include "FLASH.h"
#include "DS1302.h"

unsigned char code SetString[] = {"设置："};

void INT0_Init()
{	
	IT0=1;	               				// 设置为下降沿触发
	EX0=1;                 				// 开外部中断0			  P3.2 INT0
}
void EXT0(void) interrupt 0
{
    Delay20ms();
	Delay20ms();
	//P20 = !P20;
	if(P32==0)							// 仍然是按下状态
	{
		if(ISSETTING==0)
		{
			ISSETTING=1;
			//P21 = !P21;
			DisableTimer0();			// 关闭超声波 温湿度的定时器中断
			DisableTimer3();			// 关闭蜂鸣器的定时器中断				
		}
		else if(ISSETTING==1)
		{
			ISSETTING=2;
			EXITSETTING=1;
			WIFINEEDDELAY=1;
		}
		else
		{
			ISSETTING=0;
			WIFINEEDDELAY=0; 
			EnableTimer0();				// 开启超声波 温湿度的定时器中断
			EnableTimer3();				// 开启蜂鸣器的定时器中断			
		}
		/*if(ISSETTING==0)
		{ 
		    //P21 = !P21;
			ISSETTING=1;
			DisableTimer0();			// 关闭超声波 温湿度的定时器中断
			DisableTimer3();			// 关闭蜂鸣器的定时器中断
		}
		else
		{
		    //P26 = !P26;
		    ISSETTING=0;
			EXITSETTING=1; 
			EnableTimer0();				// 开启超声波 温湿度的定时器中断
			EnableTimer3();				// 开启蜂鸣器的定时器中断			
		}*/
	}
	else								// 抖动
	{
		P22 = !P22; 
	} 	
}

/*调整时间部分,注意:不会自动调整星期的！因为用不到它,时间不会进位、不会退位
  不判断闰年...自行调整,别调过了*/
void AddYear(unsigned char *year, signed char mode)
{
	if(mode==1)
	{
		if((*year)<30)
			(*year)++;
		else
			(*year)=0;
	}
	else
	{
		if((*year)>0)
			(*year)--;
		else
			(*year)=30;
	}
}

void AddMonth(unsigned char *month, signed char mode)
{
	if(mode==1)
	{
		if((*month)<12)
			(*month)++;
		else
			(*month)=1;
	}
	else
	{
		if((*month)>1)
			(*month)--;
		else
			(*month)=12;
	}
}

void AddDay(unsigned char *day, signed char mode)
{
	if(mode==1)
	{
		if((*day)<31)
			(*day)++;
		else
			(*day)=1;
	}
	else
	{
		if((*day)>1)
			(*day)--;
		else
			(*day)=31;
	}
}

void AddHour(unsigned char *hour, signed char mode)
{
	if(mode==1)
	{
		if((*hour)<23)
			(*hour)++;
		else
			(*hour)=0;
	}
	else
	{
		if((*hour)>0)
			(*hour)--;
		else
			(*hour)=23;
	}
}

void AddMinute(unsigned char *minute, signed char mode)
{
	if(mode==1)
	{
		if((*minute)<59)
			(*minute)++;
		else
			(*minute)=0;
	}
	else
	{
		if((*minute)>0)
			(*minute)--;
		else
			(*minute)=59;
	}
}

void AddSecond(unsigned char *second, signed char mode)
{
	if(mode==1)
	{
		if((*second)<59)
			(*second)++;
		else
			(*second)=0;
	}
	else
	{
		if((*second)>0)
			(*second)--;
		else
			(*second)=59;
	}
}

/*
 * 进入设置模式
 */
void EnterSetting()
{
    unsigned char VeloCitySettingFull1[] = {"速度上限："};
	unsigned char VeloCitySettingFull2[] = {"6 . 6 6 km/h"};
	unsigned char TimeSettingFull1[] = {"00年00月00日"};
	unsigned char TimeSettingFull2[] = {"00时00分00秒"};
	unsigned int TempVelocity;				                			//临时变量
	unsigned char TempTime[6] = {0,0,0,0,0,0};
	unsigned char SaveTime[7] = {0,0,0,0,0,0,0};	
	unsigned char VelocityValue1 = 0;									//个位
	unsigned char VelocityValue2 = 0;									//十分之一位
	unsigned char VelocityValue3 = 0; 				  					//百分之一位
    unsigned char Loc = 1;
	//unsigned char VelocityUp[] = {"速度上限：   m/s"};
	//LCD12864ClearScreen();
	//unsigned char Velocity1 = (unsigned char)(0 + 48);
	//unsigned char Velocity2 = (unsigned char)(0 + 48);
	//unsigned char VeloCitySetting1[] = {"速度上限：0."};
   	//unsigned char VeloCitySetting2[] = {"速度上限：0.0m"};
	
    LCD12864SettingInit();
	//Display_String(1,SetString);
	//Display_String(2,VelocityUp);
	Display_String(1,VeloCitySettingFull1);
	Display_String(2,VeloCitySettingFull2);
	Display_String(3,TimeSettingFull1);
	Display_String(4,TimeSettingFull2);

    TempVelocity = GetVelocityThreshold();
	DS1302_readoutTime(TempTime);
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
	
	LCD12864DisplayTwoChar(3,1,TempTime[0]/10 + 48,TempTime[0]%10 + 48);
	LCD12864DisplayTwoChar(3,3,TempTime[1]/10 + 48,TempTime[1]%10 + 48);
	LCD12864DisplayTwoChar(3,5,TempTime[2]/10 + 48,TempTime[2]%10 + 48);
	LCD12864DisplayTwoChar(4,1,TempTime[3]/10 + 48,TempTime[3]%10 + 48);
	LCD12864DisplayTwoChar(4,3,TempTime[4]/10 + 48,TempTime[4]%10 + 48);
	LCD12864DisplayTwoChar(4,5,TempTime[5]/10 + 48,TempTime[5]%10 + 48);
	 
	do	                             			             	      	//矩阵键盘按键扫描
	{
	    KeyOut1 = 0;
		KeyOut2 = 1;
		Delay20ms();
		if(KeyIn1 == 0)									   	       		//调节左右
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
			else if(Loc == 3)
			{
				Loc = 4;
				LCD12864DisplayTwoChar(3,1,TempTime[0]/10 + 48,TempTime[0]%10 + 48);
				//年
			}
			else if(Loc == 4)
			{
				Loc = 5;
				LCD12864DisplayTwoChar(3,3,TempTime[1]/10 + 48,TempTime[1]%10 + 48);
				//月
			}
			else if(Loc == 5)
			{
				Loc = 6;
				LCD12864DisplayTwoChar(3,5,TempTime[2]/10 + 48,TempTime[2]%10 + 48);
				//日
			}
			else if(Loc == 6)
			{
				Loc = 7;
				LCD12864DisplayTwoChar(4,1,TempTime[3]/10 + 48,TempTime[3]%10 + 48);
				//时
			}
			else if(Loc == 7)
			{
				Loc = 8;
				LCD12864DisplayTwoChar(4,3,TempTime[4]/10 + 48,TempTime[4]%10 + 48);
				//分
			}
			else if(Loc == 8)
			{
				Loc = 9;
				LCD12864DisplayTwoChar(4,5,TempTime[5]/10 + 48,TempTime[5]%10 + 48);
				//秒
			}
			else
			{
				Loc = 1;
				LCD12864DisplayChar(2,1,VelocityValue1 + 48);
			}
			while(KeyIn1==0);
		}

		if(KeyIn2 == 0)									   	       		//调节左右
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
			else if(Loc == 4)
			{
				Loc = 3;
				LCD12864DisplayChar(2,4,VelocityValue3 + 48);				
			}
			else if(Loc == 5)
			{
				Loc = 4;
				LCD12864DisplayTwoChar(3,1,TempTime[0]/10 + 48,TempTime[0]%10 + 48);
				//年
			}
			else if(Loc == 6)
			{
				Loc = 5;
				LCD12864DisplayTwoChar(3,3,TempTime[1]/10 + 48,TempTime[1]%10 + 48);
				//月
			}
			else if(Loc == 7)
			{
				Loc = 6;
				LCD12864DisplayTwoChar(3,5,TempTime[2]/10 + 48,TempTime[2]%10 + 48);
				//日
			}
			else if(Loc == 8)
			{
				Loc = 7;
				LCD12864DisplayTwoChar(4,1,TempTime[3]/10 + 48,TempTime[3]%10 + 48);
				//时
			}
			else if(Loc == 9)
			{
				Loc = 8;
				LCD12864DisplayTwoChar(4,3,TempTime[4]/10 + 48,TempTime[4]%10 + 48);
				//分
			}
			else if(Loc == 1)
			{
				Loc = 9;
				LCD12864DisplayTwoChar(4,5,TempTime[5]/10 + 48,TempTime[5]%10 + 48);
				//秒
			}
			else
			{
				Loc = 1;
				LCD12864DisplayChar(2,1,VelocityValue1 + 48);
			}
			while(KeyIn2==0);
		}
																	//调节值
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
			else if(Loc == 3)
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
			else if(Loc == 4)
			{
			    //年份加一
				AddYear(&TempTime[0],1);
				LCD12864DisplayTwoChar(3,1,TempTime[0]/10 + 48,TempTime[0]%10 + 48);
			}
			else if(Loc == 5)
			{
			    //月份加一
				AddMonth(&TempTime[1],1);
				LCD12864DisplayTwoChar(3,3,TempTime[1]/10 + 48,TempTime[1]%10 + 48);
			}
			else if(Loc == 6)
			{
			    //日期加一
				AddDay(&TempTime[2],1);
				LCD12864DisplayTwoChar(3,5,TempTime[2]/10 + 48,TempTime[2]%10 + 48);
			}
			
			else if(Loc == 7)
			{
			    //小时加一
				AddHour(&TempTime[3],1);
				LCD12864DisplayTwoChar(4,1,TempTime[3]/10 + 48,TempTime[3]%10 + 48);
			}
			else if(Loc == 8)
			{
			    //分钟加一
				AddMinute(&TempTime[4],1);
				LCD12864DisplayTwoChar(4,3,TempTime[4]/10 + 48,TempTime[4]%10 + 48);
			}
			
			else
			{
			    //秒加一
				AddSecond(&TempTime[5],1);
				LCD12864DisplayTwoChar(4,5,TempTime[5]/10 + 48,TempTime[5]%10 + 48);
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
			else if(Loc == 3)
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
			else if(Loc == 4)
			{
			    //年份减一
				AddYear(&TempTime[0],-1);
				LCD12864DisplayTwoChar(3,1,TempTime[0]/10 + 48,TempTime[0]%10 + 48);
			}
			else if(Loc == 5)
			{
			    //月份减一
				AddMonth(&TempTime[1],-1);
				LCD12864DisplayTwoChar(3,3,TempTime[1]/10 + 48,TempTime[1]%10 + 48);
			}
			else if(Loc == 6)
			{
			    //日期减一
				AddDay(&TempTime[2],-1);
				LCD12864DisplayTwoChar(3,5,TempTime[2]/10 + 48,TempTime[2]%10 + 48);
			}
			
			else if(Loc == 7)
			{
			    //小时减一
				AddHour(&TempTime[3],-1);
				LCD12864DisplayTwoChar(4,1,TempTime[3]/10 + 48,TempTime[3]%10 + 48);
			}
			else if(Loc == 8)
			{
			    //分钟减一
				AddMinute(&TempTime[4],-1);
				LCD12864DisplayTwoChar(4,3,TempTime[4]/10 + 48,TempTime[4]%10 + 48);
			}
			
			else
			{
			    //秒减一
				AddSecond(&TempTime[5],-1);
				LCD12864DisplayTwoChar(4,5,TempTime[5]/10 + 48,TempTime[5]%10 + 48);
			}
			while(KeyIn2==0);
		}			
	}	
	while(EXITSETTING == 0);					   	//当没有退出循环时在里面
	
	TempVelocity = 1000*VelocityValue1 + 100*VelocityValue2 + 10*VelocityValue3;	//计算速度阈值
	SetVelocityThreshold(TempVelocity,0);         	//保存速度阈值

    SaveTime[0] = TempTime[5];
	SaveTime[1] = TempTime[4];
	SaveTime[2] = TempTime[3];
	SaveTime[3] = TempTime[2];
	SaveTime[4] = TempTime[1];
	SaveTime[5] = 5;
	SaveTime[6] = TempTime[0];
    set_time(SaveTime);

	LCD12864SettingExit();
	EXITSETTING = 0;
}

/*
 进入Wifi设置模式
 */
/*void EnterWifiSetting()
{
	//do	                             			             	      	//矩阵键盘按键扫描
	//{
	    KeyOut1 = 0;
		KeyOut2 = 1;
		Delay20ms();
		if(KeyIn1 == 0)									   	     
		{  	
    		Delay20ms();
			Delay20ms();
			if(KeyIn1 == 1) continue;
			SendString2("AT+CIPMUX=1\r\n");
			SendString2("AT+CIPSERVER=1,333\r\n");
			SendString2("AT+CIPSTO=7200\r\n");
			while(KeyIn1==0);
		}

		if(KeyIn2 == 0)									   	       	
		{  	
    		Delay20ms();
			Delay20ms();
			if(KeyIn2 == 1) continue;
			while(KeyIn2==0);
		}
																	
	    KeyOut1 = 1;
		KeyOut2 = 0;
		Delay20ms();
 		if(KeyIn1 == 0)
		{
    		Delay20ms();
			Delay20ms();
			if(KeyIn1 == 1) continue;
			while(KeyIn1==0);
		}

		if(KeyIn2 == 0)
		{
    		Delay20ms();
			Delay20ms();
			if(KeyIn2 == 1) continue;
			while(KeyIn2==0);
		}			
	//}while(0);	
}*/
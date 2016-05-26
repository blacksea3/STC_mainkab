																			//****************************************************************//
//	参考来源   DHT21使用范例 
//公司  雁凌电子    
//****************************************************************//

#include <stc15.h>
//#include <intrins.h>
#include "Delay.h"
#include "UART.h"
#include "LCD12864.h"

unsigned char U8FLAG,U8temp;
unsigned char TempearatureH, TempearatureL, HumidityH, HumidityL, CHECK;
unsigned char U8T_data_H_temp,U8T_data_L_temp,U8RH_data_H_temp,U8RH_data_L_temp,U8checkdata_temp;
unsigned char U8comdata;  

sbit DHTDATA = P3^7;

//----------------------------------------------//
//----------------IO口定义区--------------------//
//----------------------------------------------//
	
void COM(void)
{
    unsigned char i;     
    for(i=0;i<8;i++)	   
	{	
	    U8FLAG=2;	
	   	while((!DHTDATA)&&U8FLAG++);
		Delay10us();
		Delay10us();
		Delay10us();
	  	U8temp=0;
	    if(DHTDATA)U8temp=1;
		    U8FLAG=2;
		while((DHTDATA)&&U8FLAG++);
	   	//超时则跳出for循环		  
	   	if(U8FLAG==1)break;
	   	//判断数据位是0还是1	 
	   	   
		// 如果高电平高过预定0高电平值则数据位为 1 
	   	 
		U8comdata<<=1;
	   	U8comdata|=U8temp;        //0
	}
}

//--------------------------------
//-----湿度读取子程序 ------------
//--------------------------------
//----以下变量均为全局变量--------
//----温度高8位== U8T_data_H------
//----温度低8位== U8T_data_L------
//----湿度高8位== U8RH_data_H-----
//----湿度低8位== U8RH_data_L-----
//----校验 8位 == U8checkdata-----
//----调用相关子程序如下----------
//---- 18个Delay1ms();, Delay10us();,COM(); 

// 返回值 unsigned char[5] 数组 {温度高8位,温度低8位,湿度高8位,湿度低8位,校验 8位} 


//--------------------------------

void ReadDHT11(void)
{
    unsigned char DelayTemp;
	//主机拉低18ms 
    DHTDATA=0;
	for(DelayTemp = 0; DelayTemp < 18; DelayTemp++)
	{
	    Delay1ms();
	}
	DHTDATA=1;
	//总线由上拉电阻拉高 主机延时40us
	Delay10us();
	Delay10us();
	Delay10us();
	Delay10us();
	//主机设为输入 判断从机响应信号 
	DHTDATA=1;
	//判断从机是否有低电平响应信号 如不响应则跳出，响应则向下运行	  
	if(!DHTDATA)		 //T !	  
	{
	   	U8FLAG=2;
	   	//判断从机是否发出 80us 的低电平响应信号是否结束	 
	   	while((!DHTDATA)&&U8FLAG++);
		U8FLAG=2;
		//判断从机是否发出 80us 的高电平，如发出则进入数据接收状态
	   	while((DHTDATA)&&U8FLAG++);
		//数据接收状态		 
	   	COM();
	   	U8RH_data_H_temp=U8comdata;
	   	COM();
	   	U8RH_data_L_temp=U8comdata;
	   	COM();
	   	U8T_data_H_temp=U8comdata;
	   	COM();
	   	U8T_data_L_temp=U8comdata;
	  	COM();
	   	U8checkdata_temp=U8comdata;
	   	DHTDATA=1;
	 	//数据校验 
	 
	   	U8temp=(U8T_data_H_temp+U8T_data_L_temp+U8RH_data_H_temp+U8RH_data_L_temp);
	   	if(U8temp==U8checkdata_temp)
	   	{
	   	  	TempearatureH=U8RH_data_H_temp;
	   	  	TempearatureL=U8RH_data_L_temp;
		  	HumidityH=U8T_data_H_temp;
	   	  	HumidityL=U8T_data_L_temp;
	   	  	CHECK=U8checkdata_temp;
	   	}
		else
		{
	   	  	TempearatureH=0x01;
	   	  	TempearatureL=0x01;
		  	HumidityH=0x01;
	   	  	HumidityL=0x01;
	   	  	CHECK=0x01;			
		}
	}
}

/*
 显示DHT11的温湿度
 */
void DisplayDHT11()
{
	unsigned char HumidityAndTemperatureData[] = {"湿度00% 温度00度"};
	//unsigned char TemperatureData[] = {""};
	
	ReadDHT11();
	//SendString(StrHello);
    
	//HumidityData[6]=TempearatureH%100/10+48;
	//HumidityData[7]=TempearatureH%10+48;								
    //Display_String(2,HumidityData);

	//SendData(TempearatureL);	  //正常为00

	//TemperatureData[6]=HumidityH%100/10+48;
	//TemperatureData[7]=HumidityH%10+48;
	//Display_String(3,TemperatureData);

	//SendData(HumidityL);		  //正常为00
    //SendData(CHECK);			  //校验位	
    HumidityAndTemperatureData[4] = TempearatureH%100/10+48;
	HumidityAndTemperatureData[5] = TempearatureH%10+48;
    HumidityAndTemperatureData[12] = HumidityH%100/10+48;
	HumidityAndTemperatureData[13] = HumidityH%10+48;
	Display_String(2,HumidityAndTemperatureData);
}
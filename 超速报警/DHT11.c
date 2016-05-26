																			//****************************************************************//
//	�ο���Դ   DHT21ʹ�÷��� 
//��˾  �������    
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
//----------------IO�ڶ�����--------------------//
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
	   	//��ʱ������forѭ��		  
	   	if(U8FLAG==1)break;
	   	//�ж�����λ��0����1	 
	   	   
		// ����ߵ�ƽ�߹�Ԥ��0�ߵ�ƽֵ������λΪ 1 
	   	 
		U8comdata<<=1;
	   	U8comdata|=U8temp;        //0
	}
}

//--------------------------------
//-----ʪ�ȶ�ȡ�ӳ��� ------------
//--------------------------------
//----���±�����Ϊȫ�ֱ���--------
//----�¶ȸ�8λ== U8T_data_H------
//----�¶ȵ�8λ== U8T_data_L------
//----ʪ�ȸ�8λ== U8RH_data_H-----
//----ʪ�ȵ�8λ== U8RH_data_L-----
//----У�� 8λ == U8checkdata-----
//----��������ӳ�������----------
//---- 18��Delay1ms();, Delay10us();,COM(); 

// ����ֵ unsigned char[5] ���� {�¶ȸ�8λ,�¶ȵ�8λ,ʪ�ȸ�8λ,ʪ�ȵ�8λ,У�� 8λ} 


//--------------------------------

void ReadDHT11(void)
{
    unsigned char DelayTemp;
	//��������18ms 
    DHTDATA=0;
	for(DelayTemp = 0; DelayTemp < 18; DelayTemp++)
	{
	    Delay1ms();
	}
	DHTDATA=1;
	//������������������ ������ʱ40us
	Delay10us();
	Delay10us();
	Delay10us();
	Delay10us();
	//������Ϊ���� �жϴӻ���Ӧ�ź� 
	DHTDATA=1;
	//�жϴӻ��Ƿ��е͵�ƽ��Ӧ�ź� �粻��Ӧ����������Ӧ����������	  
	if(!DHTDATA)		 //T !	  
	{
	   	U8FLAG=2;
	   	//�жϴӻ��Ƿ񷢳� 80us �ĵ͵�ƽ��Ӧ�ź��Ƿ����	 
	   	while((!DHTDATA)&&U8FLAG++);
		U8FLAG=2;
		//�жϴӻ��Ƿ񷢳� 80us �ĸߵ�ƽ���緢����������ݽ���״̬
	   	while((DHTDATA)&&U8FLAG++);
		//���ݽ���״̬		 
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
	 	//����У�� 
	 
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
 ��ʾDHT11����ʪ��
 */
void DisplayDHT11()
{
	unsigned char HumidityAndTemperatureData[] = {"ʪ��00% �¶�00��"};
	//unsigned char TemperatureData[] = {""};
	
	ReadDHT11();
	//SendString(StrHello);
    
	//HumidityData[6]=TempearatureH%100/10+48;
	//HumidityData[7]=TempearatureH%10+48;								
    //Display_String(2,HumidityData);

	//SendData(TempearatureL);	  //����Ϊ00

	//TemperatureData[6]=HumidityH%100/10+48;
	//TemperatureData[7]=HumidityH%10+48;
	//Display_String(3,TemperatureData);

	//SendData(HumidityL);		  //����Ϊ00
    //SendData(CHECK);			  //У��λ	
    HumidityAndTemperatureData[4] = TempearatureH%100/10+48;
	HumidityAndTemperatureData[5] = TempearatureH%10+48;
    HumidityAndTemperatureData[12] = HumidityH%100/10+48;
	HumidityAndTemperatureData[13] = HumidityH%10+48;
	Display_String(2,HumidityAndTemperatureData);
}
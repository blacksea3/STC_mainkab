#include "uart_debug.h"
#include "stc15.h"
#include "fifo.h"
#include "Sensor.h"
#include "intrins.h"
/****************��51��Ƭ����������-����STC15W4Kϵ�С��������� *************
 �������������������������
 ��51��Ƭ����������-����STC15W4Kϵ�С� һ���Ѿ��ɱ�����������ʽ���淢�С�
  �������ִ�������̲����׵�51˫��ʵ���(2��MCU)�Գ������ء����ԡ����淽�㣬����Ҫ�ⲿ
  ��������������������Ʒ�ʽ���׽����ϵͳ�ж��������ȼ�˭Ҳ������˭���жϾ������⡣
  �Ա����ַ��https://shop117387413.taobao.com
  QQȺ��STC51-STM32(3) ��515624099 �� STC51-STM32(2)��99794374��
        ��֤��Ϣ��STC15��Ƭ��
  ���䣺xgliyouquan@126.com
  �������������������������*/

unsigned char cur_status=0;

unsigned char buf[192];

unsigned char ISBUSY;

void int0(void) interrupt 0
{
 if(ISBUSY)
 {
 	;
 }
 else
 {
 EX0=0; //�ر��ж�

 if(cur_status==0) //�����ʱ״̬Ϊ0����˵����һ��ͼ��Ŀ�ʼ����ʼ��FIFO��������
 {
  FIFO_WEN=1;
  cur_status=1; //���Ϊ1
        
  
 }
 else 
  if(cur_status==1)	//˵���˴�Ϊͼ��Ľ������༴��һͼ��Ŀ�ʼ
  {
   FIFO_WEN=0;
   cur_status=2;
  }
  P26 = !P26;
  EX0=1;	//���ж�
  }
}

void port_mode()            // �˿�ģʽ
{
	P0M1=0x00; P0M0=0x00;P1M1=0x00; P1M0=0x00;P2M1=0x00; P2M0=0x00;P3M1=0x00; P3M0=0x00;
	P4M1=0x00; P4M0=0x00;
	//P5M1=0x00; P5M0=0x00;P6M1=0x00; P6M0=0x00;P7M1=0x00; P7M0=0x00; 
}

void Delay1000ms()		//@22.1184MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 85;
	j = 12;
	k = 155;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

void main()
{
	//unsigned char a=0x55;
	//unsigned int b=0xAB98;
	//unsigned long c=1234567890; 
	//unsigned char Buf[]="��ӭʹ��STC15��Ƭ��!\n"; //�ַ������ڴ��β��Ȼ��һ�������ַ���\0
	unsigned char value=100;
	unsigned char idata i=0;
	unsigned char idata temp=11;
	ISBUSY = 0;
	port_mode();				 
	UART_init();						 //	�����ʣ�9600 /22.1184MHZ
	UART_Send_Str("����������ϣ�123ABC\n"); 	 // �����ַ���
 
 	FIFO_OE=0;

 	P1M1=0x40;	//P16��������
 	P1M0=0xa8; //���ʹ��51��Ƭ��������SD��ģ�飬�����죬��ǿIO��������

 	IT0=1; //�½��ش���
 	EX0=1;	//���ⲿ�ж�0
 	EA=1;	//�����ж�

 	//cur_status = 2;
	//temp = Sensor_init();
	while(temp = !Sensor_init());  

    UART_Put_Inf("FUCKYOU:",temp);

	UART_Send_Str("Ӵ��\n");
 	//SendString("����ͷоƬ��ʼ�����.");//UART_Send_Enter(); 
							 // ������ֵ
//	UART_Send_StrNum("��ֵ=:",c);  		  // �����ַ���+��ֵ
	//UART_Send_Hex(b) ;	                  // ����16����
	//UART_Send_binary(a);                  // ����2����
	for(i=0;i<10;i++)
	{	
		Delay1000ms();
		Delay1000ms();
		Delay1000ms();
		Delay1000ms();
		Delay1000ms();		
	}
	
	
	while(1)
	{
  		if(cur_status==2) //Խ��FIFO���Ѿ�����һ��������ͼ��
  		{
		    EX0=0;
   			ISBUSY = 1;
			cur_status=0;

   			//UART_Send_Str("ͼ���ѻ�ȡ.");//UART_Send_Enter();

   			FIFO_Reset_Read_Addr(); //��FIFO�Ķ�ָ��ָ��0��ַ

   			//UART_Send_Str("��ʼ��ͼ��д���ļ�.");//UART_Send_Enter();

   			for(i=0;i<200;i++)
   			{
    			FIFO_Read_Words(96,buf); //��FIFO��ȡ����
				UART_Send_Str(buf);
    //znFAT_WriteData(&fileinfo,192,buf); //������д���ļ�
    			FIFO_Read_Words(96,buf); //��FIFO��ȡ����
				UART_Send_Str(buf);
    //znFAT_WriteData(&fileinfo,192,buf); //������д���ļ�
    			FIFO_Read_Words(96,buf); //��FIFO��ȡ����
				UART_Send_Str(buf);
    //znFAT_WriteData(&fileinfo,192,buf); //������д���ļ�
    			FIFO_Read_Words(96,buf); //��FIFO��ȡ����
				UART_Send_Str(buf);
    //znFAT_WriteData(&fileinfo,192,buf); //������д���ļ�
   			}

			for(i=0;i<10;i++)
			{	
				Delay1000ms();
				Delay1000ms();
				Delay1000ms();
				Delay1000ms();
				Delay1000ms();		
			}


   			//UART_Send_Str("ͼ��д�����.");//UART_Send_Enter();

   //znFAT_Close_File(&fileinfo);

   //znFAT_Flush_FS();
   			ISBUSY = 0;
			EX0=1;
		}
	}
}
#include "uart_debug.h"
#include "stc15.h"
#include "fifo.h"
#include "Sensor.h"
#include "intrins.h"
/****************《51单片机轻松入门-基于STC15W4K系列》配套例程 *************
 ★★★★★★★★★★★★★★★★★★★★★★★★
 《51单片机轻松入门-基于STC15W4K系列》 一书已经由北航出版社正式出版发行。
  作者亲手创作的与教材配套的51双核实验板(2个MCU)对程序下载、调试、仿真方便，不需要外部
  仿真器与编程器，这种设计方式彻底解决了系统中多个最高优先级谁也不能让谁的中断竞争问题。
  淘宝店地址：https://shop117387413.taobao.com
  QQ群：STC51-STM32(3) ：515624099 或 STC51-STM32(2)：99794374。
        验证信息：STC15单片机
  邮箱：xgliyouquan@126.com
  ★★★★★★★★★★★★★★★★★★★★★★★★*/

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
 EX0=0; //关闭中断

 if(cur_status==0) //如果此时状态为0，则说明是一个图像的开始，开始向FIFO罐入数据
 {
  FIFO_WEN=1;
  cur_status=1; //标记为1
        
  
 }
 else 
  if(cur_status==1)	//说明此处为图像的结束，亦即下一图像的开始
  {
   FIFO_WEN=0;
   cur_status=2;
  }
  P26 = !P26;
  EX0=1;	//打开中断
  }
}

void port_mode()            // 端口模式
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
	//unsigned char Buf[]="欢迎使用STC15单片机!\n"; //字符串在内存结尾必然有一个附加字符：\0
	unsigned char value=100;
	unsigned char idata i=0;
	unsigned char idata temp=11;
	ISBUSY = 0;
	port_mode();				 
	UART_init();						 //	波特率：9600 /22.1184MHZ
	UART_Send_Str("串口设置完毕：123ABC\n"); 	 // 发送字符串
 
 	FIFO_OE=0;

 	P1M1=0x40;	//P16高阻输入
 	P1M0=0xa8; //如果使用51单片机来调试SD卡模块，打开推挽，增强IO驱动能力

 	IT0=1; //下降沿触发
 	EX0=1;	//打开外部中断0
 	EA=1;	//打开总中断

 	//cur_status = 2;
	//temp = Sensor_init();
	while(temp = !Sensor_init());  

    UART_Put_Inf("FUCKYOU:",temp);

	UART_Send_Str("哟呵\n");
 	//SendString("摄像头芯片初始化完成.");//UART_Send_Enter(); 
							 // 发送数值
//	UART_Send_StrNum("数值=:",c);  		  // 发送字符串+数值
	//UART_Send_Hex(b) ;	                  // 发送16进制
	//UART_Send_binary(a);                  // 发送2进制
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
  		if(cur_status==2) //越级FIFO中已经罐入一个完整的图像
  		{
		    EX0=0;
   			ISBUSY = 1;
			cur_status=0;

   			//UART_Send_Str("图像已获取.");//UART_Send_Enter();

   			FIFO_Reset_Read_Addr(); //把FIFO的读指针指向0地址

   			//UART_Send_Str("开始将图像写入文件.");//UART_Send_Enter();

   			for(i=0;i<200;i++)
   			{
    			FIFO_Read_Words(96,buf); //从FIFO读取数据
				UART_Send_Str(buf);
    //znFAT_WriteData(&fileinfo,192,buf); //将数据写入文件
    			FIFO_Read_Words(96,buf); //从FIFO读取数据
				UART_Send_Str(buf);
    //znFAT_WriteData(&fileinfo,192,buf); //将数据写入文件
    			FIFO_Read_Words(96,buf); //从FIFO读取数据
				UART_Send_Str(buf);
    //znFAT_WriteData(&fileinfo,192,buf); //将数据写入文件
    			FIFO_Read_Words(96,buf); //从FIFO读取数据
				UART_Send_Str(buf);
    //znFAT_WriteData(&fileinfo,192,buf); //将数据写入文件
   			}

			for(i=0;i<10;i++)
			{	
				Delay1000ms();
				Delay1000ms();
				Delay1000ms();
				Delay1000ms();
				Delay1000ms();		
			}


   			//UART_Send_Str("图像写入完成.");//UART_Send_Enter();

   //znFAT_Close_File(&fileinfo);

   //znFAT_Flush_FS();
   			ISBUSY = 0;
			EX0=1;
		}
	}
}
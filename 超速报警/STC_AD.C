#include "stc15.h"
#include "LCD12864.h"
#include "main.h"

//#include "UART.H"   // 包含STC115F2K60S2单片机寄存器定义文件 
#define VCC 4.970   // 存放用万用表实测的单片机供电电压
unsigned int ADvalue;	    // 存放AD转换返回的结果

void  ADC_P12_init()
{
	unsigned int i;		    // 用于软件延时程序
	ADC_CONTR|=0x80;      	// 开AD转换电源,第一次使用时要打开内部模拟电源
    for (i=0;i<10000;i++);  // 适当延时等待AD转换供电稳定，一般延时1ms以内即可，为了缩短AD
// 调用时间，可把这2行剪切到主程序中去。
	P1ASF|=0x04;			// 选择P1.2作为AD转换通道，0x02= 0000 0100	
	ADC_CONTR=0xE1;         // 选择P1.2作为AD转换通道，最高转换速度，清转换完成标志。
	for (i=0;i<1000;i++);   // 如果是多通道模拟量进行AD转换，则更换AD转换通道后要适当延时，
// 使输入电压稳定，延时量取20μs～200μs即可，与输入电压源的内阻有关，如果输入电压信号源的内
// 阻在10K以下，可不加延时，如果是单通道模拟量转换，则不需要更换AD转换通道，也不需要加延时。 	
}  

/*
 ADC启动
 */
void ADCStart()
{
	ADC_CONTR|=0x08;        		// 启动 A/D 转换，ADC_START=1。
	EADC=1;
	ADCSTARTREADY = 0; 
}

void ADCStop()
{
    ADC_CONTR&=0xf7;          		// 关闭 A/D 转换，ADC_START=0。
	EADC=0;
	ADCSTOPREADY = 0; 				
	ADCDISPLAYREADY = 1;			// ADC显示开启 	
}

void ADC(void) interrupt 5
{
	unsigned int AD_Dat=0;    		// 10位AD转换值
	unsigned char Tmp=0;	  		// 临时变量用于将AD转换出来的2个字节合成一个字节
	ADC_CONTR&=0xE7;     	  		// 将ADC_FLAG清0， 0xE7=1110 0111B，ADC_FLAG=0，ADC_START=0。	
	AD_Dat = ADC_RES;		  		// 默认高字节高8位。
	AD_Dat <<= 2;
	Tmp = ADC_RESL;           		// 默认低字节低2位。
	Tmp &= 0x03;              		// 屏蔽无关位
	AD_Dat|= Tmp;	          		// 高低字节拼接成一个10位数。
	ADvalue=AD_Dat;			   	 	// 值:0-1023
	ADCSTOPREADY = 1; 				// ADC准备停止
	
}

/* 
 显示空气传感器数值
 */
void ADCDisplay()
{
    unsigned char TempData[]={"空气质量：0000"};
    TempData[10]=ADvalue%10000/1000 + 48;							
	TempData[11]=ADvalue%1000/100 + 48;
	TempData[12]=ADvalue%100/10 + 48;
	TempData[13]=ADvalue%10 + 48;
	Display_String(3,TempData);
	ADCDISPLAYREADY = 0; 			
}
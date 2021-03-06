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

#include "stc15.h" 
#include "LCD12864.h"
#include "UART.h"
#include "DS1302.h"

#include <intrins.h> 
	

#define DS1302_W_ADDR 0x80	   // 写起始地址
#define DS1302_R_ADDR 0x81 	   // 读起始地址

/*
 * 时间格式化，
 * 参数:StartTime[7]7位数组,待格式化,time8位数组格式化后的数据
 */
void Data_Swap(unsigned char StartTime[7],unsigned char time[8])             // 数据交换
{
	time[0]=StartTime[5];	 // 秒
	time[1]=StartTime[4];	 // 分
	time[2]=StartTime[3];	 // 时
	time[3]=StartTime[2];	 // 日
	time[4]=StartTime[1];	 // 月
	time[5]=StartTime[6];	 // 周
	time[6]=StartTime[0];	 // 年
	time[7]=0;				 // 最后一字节写保护为0可写入数据
}

// 写字节，低位在前，高位在后	
void write_ds1302_byte(unsigned char dat) 
{
	unsigned char i;
	for (i=0;i<8;i++) 
	{ 
		SDA = dat & 0x01;  // bit型变量取值范围是0和1,赋值非0结果为1
		SCK = 1;		   // 放入数据到时钟上升沿时间 50nS
		dat >>= 1;		   // 时钟上升沿后数据保持时间 70nS
		SCK = 0;		   // 时钟低电平时间与时钟高电平时间250nS
	}
}  
// 清除写保护
void clear_ds1302_WP(void) 
{
	RST = 0;	   // RST引脚既是CE引脚
	SCK = 0;
	RST = 1;
	write_ds1302_byte(0x8E);
	write_ds1302_byte(0);
	SDA = 0;
	RST = 0;
}	
// 设置写保护 
void set_ds1302_WP(void) 
{
	RST = 0;	   // RST引脚既是CE引脚
	SCK = 0;
	RST = 1;
	write_ds1302_byte(0x8E);
	write_ds1302_byte(0x80);
	SDA = 0;
	RST = 0;
}

// 连续写入8个寄存器数据，dat-待写入数据指针  
void write_ds1302_nbyte(unsigned char *dat)
{
    unsigned char i;
    RST=0;
	SCK=0;
    RST = 1;
    write_ds1302_byte(0xBE);  // 发送突发写寄存器指令
    for (i=0; i<8; i++)       // 连续写入8字节数据
    {
        write_ds1302_byte(dat[i]);
    }
    RST = 0;
}
// 设定时钟数据
void set_time(unsigned char *timedata)
{
	unsigned char i, tmp;
	for (i=0; i<7; i++)  // 转化为压缩BCD格式（一个字节存放2个BCD码）
	{
		tmp = timedata[i] / 10;		        // 获取高位BCD码
		timedata[i] = timedata[i] % 10;		// 获取低位BCD码
		timedata[i] = timedata[i] + tmp*16;	// 合并成压缩BCD码
	}
	clear_ds1302_WP();		             	// 清除写保护
	write_ds1302_nbyte(timedata);		    // 连续写入8字节数据
	set_ds1302_WP();		                // 写保护
}  

// 读字节，低位在前，高位在后
unsigned char read_ds1302_byte(void) 
{
	unsigned char i, dat=0;	
	for (i=0;i<8;i++)
	{	
		dat >>= 1;
		if (SDA) dat |= 0x80;
		SCK = 1;
		SCK = 0;
	} 
	return dat;
} 

// 单字节压缩BCD码转换成ASCII码另一常见格式示例，dat/16 + 0x30，
// dat/16本质就是右移4位，数字0--9加上0x30即得数字0--9的ASCII码
void LCD12864_Send_TIME(unsigned char *tmp)
{
	unsigned char str[19];           // 字符串转换缓冲区
	unsigned char *nowstrloc;
	nowstrloc = &str[4];
	//str[0] = '2';                    // 添加年份的高2位：20
    //str[1] = '0';
    str[1] = (tmp[6] >> 4) + '0';    //“年”高位数字转换为ASCII码
    str[2] = (tmp[6]&0x0F) + '0';    //“年”低位数字转换为ASCII码
    str[3] = '-';  //添加日期分隔符
    str[4] = (tmp[4] >> 4) + '0';    //“月”
    str[5] = (tmp[4]&0x0F) + '0';
    str[6] = '-';
    str[7] = (tmp[3] >> 4) + '0';    //“日”
    str[8] = (tmp[3]&0x0F) + '0';
    str[9] = ',';
    //str[10] = '\0';					 // 字符串结束符
	//UART_Send_Str(str);				 // 输出  年、月、日
	//UART_Send_Str("   ");
  
    str[10] = (tmp[2] >> 4) + '0';    //“时”
    str[11] = (tmp[2]&0x0F) + '0';
    str[12] = ':';  //添加时间分隔符
    str[13] = (tmp[1] >> 4) + '0';    //“分”
    str[14] = (tmp[1]&0x0F) + '0';
    str[15] = ':';
	//str[15] = '\0';
    str[16] = (tmp[0] >> 4) + '0';    //“秒”
    str[17] = (tmp[0]&0x0F) + '0';
    str[18] = '\0';
	Display_String(4,nowstrloc);			  //LCD12864输出

    //UART_Send_Str(str);  	         // 输出  时、分、秒         
	//UART_Send_Str("   ");   
	
	/*UART_Send_Str("week = "); 
	str[0] = (tmp[5]&0x0F) + '0';    //“星期”
    str[1] = '\0'; 
	UART_Send_Str(str); 			 // 输出  星期  
	UART_Send_Str("   ");*/ 
}

// 连续读取8个寄存器的数据//dat-读取数据的接收指针  
void DS3231read_time()
{
    unsigned char i;
	unsigned char TempData[8] = {0,0,0,0,0,0,0,0};
	//unsigned char TimeData[8] = {0,0,0,0,0,0,0,0};  
	RST=0;
	SCK=0;
    RST = 1;
    write_ds1302_byte(0xBF);  //发送突发读寄存器指令
    for (i=0; i<8; i++)     //连续读取8个字节
    {
        TempData[i] = read_ds1302_byte();
    }
    RST = 0;
	LCD12864_Send_TIME(TempData);
} 
 
void delay_1S(void)
{
   unsigned char i,j,k;
   for(i=85;i>0;i--)      // 注意后面没分号
   for(j=254;j>0;j--)     // 注意后面没分号
   for(k=255;k>0;k--);    // 注意后面有分号
}

/*
 * DS1302读出时间
 * 参数:无
 * 返回:[年,月,日,时,分,秒]
 */
void DS1302_readoutTime(unsigned char result[6])
{
    unsigned char i;
	unsigned char TempData[8] = {0,0,0,0,0,0,0,0};
	//unsigned char result[6] = {0,0,0,0,0,0};
	//unsigned char TimeData[8] = {0,0,0,0,0,0,0,0};  
	RST=0;
	SCK=0;
    RST = 1;
    write_ds1302_byte(0xBF);  //发送突发读寄存器指令
    for (i=0; i<8; i++)     //连续读取8个字节
    {
        TempData[i] = read_ds1302_byte();
    }
    RST = 0;
	//转换格式					  
	result[0] = (unsigned char)(10*(TempData[6] >> 4)) + (unsigned char)(TempData[6]&0x0F);
	result[1] = (unsigned char)(10*(TempData[4] >> 4)) + (unsigned char)(TempData[4]&0x0F);
	result[2] = (unsigned char)(10*(TempData[3] >> 4)) + (unsigned char)(TempData[3]&0x0F);
	result[3] = (unsigned char)(10*(TempData[2] >> 4)) + (unsigned char)(TempData[2]&0x0F);
	result[4] = (unsigned char)(10*(TempData[1] >> 4)) + (unsigned char)(TempData[1]&0x0F);
	result[5] = (unsigned char)(10*(TempData[0] >> 4)) + (unsigned char)(TempData[0]&0x0F);
}

/*
 * DS1302用Wifi发送时间
 */
void DS1302SendTimeByUART()
{
	unsigned char Time[6];
	unsigned char String[13];
	DS1302_readoutTime(Time);
	String[0] = Time[0]/10;
	String[1] = Time[0]%10;
	String[2] = Time[1]/10;
	String[3] = Time[1]%10;
	String[4] = Time[2]/10;
	String[5] = Time[2]%10;
	String[6] = Time[3]/10;
	String[7] = Time[3]%10;
	String[8] = Time[4]/10;
	String[9] = Time[4]%10;
	String[10] = Time[5]/10;
	String[11] = Time[5]%10;
	String[12] = '\0';
	//SendString2(String);
	SendData(String[0]+48);
	SendData(String[1]+48);
	SendData('-');
	SendData(String[2]+48);
	SendData(String[3]+48);
	SendData('-');
	SendData(String[4]+48);
	SendData(String[5]+48);
	SendData(' ');
	SendData(String[6]+48);
	SendData(String[7]+48);
	SendData(':');
	SendData(String[8]+48);
	SendData(String[9]+48);
	SendData(':');
	SendData(String[10]+48);
	SendData(String[11]+48);
}

/*
 * DS1302用Wifi发送时间
 */
void DS1302SendTimeByWifi()
{
	unsigned char Time[6];
	unsigned char String[26];
	DS1302_readoutTime(Time);
	String[0] = Time[0]/10 + 48;
	String[1] = Time[0]%10 + 48;
	String[2] = '-';
	String[3] = Time[1]/10 + 48;
	String[4] = Time[1]%10 + 48;
	String[5] = '-';
	String[6] = Time[2]/10 + 48;
	String[7] = Time[2]%10 + 48;
	String[8] = ' ';
	String[9] = Time[3]/10 + 48;
	String[10] = Time[3]%10 + 48;
	String[11] = ':';
	String[12] = Time[4]/10 + 48;
	String[13] = Time[4]%10 + 48;
	String[14] = ':';
	String[15] = Time[5]/10 + 48;
	String[16] = Time[5]%10 + 48;
	String[17] = '\0';
	SendString2Length(String,17);
}

/*
 * DS1302初始化
 * 参数:时间数组StartTime[7]=	{14,11,30,23,50,59,7} 
 */
void DS1302Init(unsigned char StartTime[7])    
{
	//unsigned char StartTime[7]=	{14,11,30,23,50,59,7};  // 年月日时分秒周 14-11-30 23:50:59
	unsigned char time[8];  								// 秒分时日月周年  
	Data_Swap(StartTime,time);        						// 数据交换
 	set_time(time);     									// 设定初始时间值 ,数组名就代表数组首地址		
	//read_time();          							    // 秒分时日月周年	
}
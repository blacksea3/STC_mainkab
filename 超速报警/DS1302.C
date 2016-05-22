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

#include <intrins.h> 

sbit SCK = P2^7;	           // 时钟	
sbit SDA = P4^5;   	           // 数据	
sbit RST = P4^6;               // DS1302复位(片选)
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
	str[0] = '2';                    // 添加年份的高2位：20
    str[1] = '0';
    str[2] = (tmp[6] >> 4) + '0';    //“年”高位数字转换为ASCII码
    str[3] = (tmp[6]&0x0F) + '0';    //“年”低位数字转换为ASCII码
    str[4] = '-';  //添加日期分隔符
    str[5] = (tmp[4] >> 4) + '0';    //“月”
    str[6] = (tmp[4]&0x0F) + '0';
    str[7] = '-';
    str[8] = (tmp[3] >> 4) + '0';    //“日”
    str[9] = (tmp[3]&0x0F) + '0';
    //str[10] = '\0';					 // 字符串结束符
	//UART_Send_Str(str);				 // 输出  年、月、日
	//UART_Send_Str("   ");
  
    str[10] = (tmp[2] >> 4) + '0';    //“时”
    str[11] = (tmp[2]&0x0F) + '0';
    str[12] = ':';  //添加时间分隔符
    str[13] = (tmp[1] >> 4) + '0';    //“分”
    str[14] = (tmp[1]&0x0F) + '0';
    str[15] = ':';
    str[16] = (tmp[0] >> 4) + '0';    //“秒”
    str[17] = (tmp[0]&0x0F) + '0';
    str[18] = '\0';
	Display_String(4,str);			  //LCD12864输出

    //UART_Send_Str(str);  	         // 输出  时、分、秒         
	//UART_Send_Str("   ");   
	
	/*UART_Send_Str("week = "); 
	str[0] = (tmp[5]&0x0F) + '0';    //“星期”
    str[1] = '\0'; 
	UART_Send_Str(str); 			 // 输出  星期  
	UART_Send_Str("   ");*/ 
}

// 连续读取8个寄存器的数据//dat-读取数据的接收指针  
void read_time()
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

void port_mode()            // 端口模式
{
	P0M1=0x00; P0M0=0x00;P1M1=0x00; P1M0=0x00;P2M1=0x00; P2M0=0x00;P3M1=0x00; P3M0=0x00;
	P4M1=0x00; P4M0=0x00;P5M1=0x00; P5M0=0x00;P6M1=0x00; P6M0=0x00;P7M1=0x00; P7M0=0x00; 
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
	//read_time();          							// 秒分时日月周年	
}
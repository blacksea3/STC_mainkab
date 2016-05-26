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

//******************************************************************************
//                此文件直接复制使用，用户无需任何更改。
//******************************************************************************
#include "FLASH.h"  
//#include "UART.h"

// 寄存器定义，虽然头文件已有定义，但不会冲突，这里列出来方便理解程序。
sfr ISP_DATA  = 0xC2;
sfr ISP_ADDRH = 0xC3;
sfr ISP_ADDRL = 0xC4;
sfr ISP_CMD   = 0xC5;
sfr ISP_TRIG  = 0xC6;
sfr ISP_CONTR = 0xC7;

///////////////////////////////// FLASH 操作延时等待参数  ////////////////////////////
#if (SYSclk >= 24000000L)
	#define		ISP_WAIT_FREQUENCY	0
#elif (SYSclk >= 20000000L)
	#define		ISP_WAIT_FREQUENCY	1
#elif (SYSclk >= 12000000L)
	#define		ISP_WAIT_FREQUENCY	2
#elif (SYSclk >= 6000000L)
	#define		ISP_WAIT_FREQUENCY	3
#elif (SYSclk >= 3000000L)
	#define		ISP_WAIT_FREQUENCY	4
#elif (SYSclk >= 2000000L)
	#define		ISP_WAIT_FREQUENCY	5
#elif (SYSclk >= 1000000L)
	#define		ISP_WAIT_FREQUENCY	6
#else
	#define		ISP_WAIT_FREQUENCY	7
#endif	  
//////////////////////////////////////////////////////////////////////////////////////////////

/*************************禁止操作FLASH ( 固定不变 )********************************************/

void DisableEEPROM(void)		// 以下语句可以不用，只是出于安全考虑而已
{
	ISP_CONTR = 0;				// 禁止ISP/IAP操作
	ISP_CMD   = 0;				// 去除ISP/IAP命令
	ISP_TRIG  = 0;				// 防止ISP/IAP命令误触发
	ISP_ADDRH = 0xff;			// 指向非EEPROM区，防止误操作
	ISP_ADDRL = 0xff;			// 指向非EEPROM区，防止误操作
}
/******************** 写N个字节函数 最多255字节一次( 固定不变 ) *****************/
void EEPROM_write_n(unsigned int EE_address,unsigned char *DataAddress,unsigned char lenth)
{
	EA = 0;		         // 禁止中断									   
	ISP_CONTR = 0x80 + ISP_WAIT_FREQUENCY;  // 允许操作FLASH + 延时等待时间，送一次就够
	ISP_CMD = 2	;	                        // 字节写命令，命令不需改变时，不需重新送命令
	do
	{
		ISP_ADDRH = EE_address / 256;		// 送地址高字节（地址需要改变时才需重新送地址）
		ISP_ADDRL = EE_address % 256;		// 送地址低字节
		ISP_DATA  = *DataAddress;			// 送数据到ISP_DATA，只有数据改变时才需重新送
		ISP_TRIG = 0x5A;					// ISP触发命令,先送5AH，再送A5H到ISP/IAP触发寄存器，每次都需要如此
		ISP_TRIG = 0xA5;					// ISP触发命令,写字节最长需要55uS，因此本行语句会暂停55uS以上的时间
		_nop_();
		_nop_();
		_nop_();
		_nop_();		
		EE_address++;						// 下一个地址
		DataAddress++;						// 下一个数据
	}while(--lenth);						// 直到结束
	DisableEEPROM();
	EA = 1;	          	// 重新允许中断
}

/******************** 读N个字节函数 最多255字节一次 ( 固定不变 )*****************/
void EEPROM_read_n(unsigned int EE_address,unsigned char *DataAddress,unsigned char lenth)
{
	EA = 0;		        // 禁止中断
	ISP_CONTR = 0x80 + ISP_WAIT_FREQUENCY;  // 允许操作FLASH + 延时等待时间，送一次就够
	ISP_CMD = 1	;		// 字节读命令，命令不需改变时，不需重新送命令
	do
	{
		ISP_ADDRH = EE_address / 256;		// 送地址高字节（地址需要改变时才需重新送地址）
		ISP_ADDRL = EE_address % 256;		// 送地址低字节
		ISP_TRIG = 0x5A;					// ISP触发命令
		ISP_TRIG = 0xA5;					// ISP触发命令，读一个字节最长需要2个时钟，因此本行语句会暂停2个时钟以上的时间
		_nop_();
		_nop_();
		_nop_();
		_nop_();		
		*DataAddress = ISP_DATA;			// 读出的数据送往外部变量地址
		EE_address++;
		DataAddress++;
	}while(--lenth);
	DisableEEPROM();					    
	EA = 1;		       // 重新允许中断
}  

/******************** 扇区擦除函数( 固定不变 )  *****************/
void EEPROM_SectorErase(unsigned int EE_address)
{
	EA = 0;		         // 禁止中断
	// 只有扇区擦除，没有字节擦除，512字节/扇区，扇区中任意一个字节地址都是扇区地址。
	ISP_ADDRH = EE_address / 256;			// 送扇区地址高字节（地址需要改变时才需重新送地址）
	ISP_ADDRL = EE_address % 256;			// 送扇区地址低字节
	ISP_CONTR = 0x80 + ISP_WAIT_FREQUENCY;  // 允许操作FLASH + 延时等待时间，送一次就够
	ISP_CMD = 3;		                    // 送扇区擦除命令，命令不需改变时，不需重新送命令
	ISP_TRIG = 0x5A;						// ISP触发命令
	ISP_TRIG = 0xA5;						// ISP触发命令，擦除最长需要21mS，因此本行语句会暂停21mS以上的时间
	_nop_();		
	DisableEEPROM();						// 禁止命令
	EA = 1;		        // 重新允许中断
}

/**********************存放速度阈值******************************/
/*
 用户程序空白区域用FF填的,首先检测是否全FF,如果不那么不用初始化速度阈值了
 注意，小心不要和程序混起来，IAP系列用户程序和EEPROM混在一起的...
 首数据存入E000h
 参数:速度,是否开始的初始化
 */	
void SetVelocityThreshold(unsigned int Velocity,unsigned char IsInit)
{
    unsigned char TempData[4];				 		//临时变量
	//判断是否已经有了合法速度阈值
    EEPROM_read_n(0xE000,TempData,4);
	if(IsInit&&(TempData[0]!=0xFF||TempData[1]!=0xFF||TempData[2]!=0xFF||TempData[3]!=0xFF))	 //初始化时调用并且已经存过数据了
	{
		;
	}
	else
	{
	    EEPROM_SectorErase(0xE000);					//先擦除
    	P20 = !P20;
		//转为10进制数存储
		//VelocityData[7]=DDis%100000/10000 + 48;
		TempData[0] = Velocity%10000/1000;
		TempData[1] = Velocity%1000/100;
		TempData[2] = Velocity%100/10;
		TempData[3] = Velocity%10;
		EEPROM_write_n(0xE000,TempData,4);
	}
}

/**********************读取速度阈值******************************/
/*
 注意，小心不要和程序混起来，IAP系列用户程序和EEPROM混在一起的...
 首数据存入E000h
 */	
unsigned int GetVelocityThreshold()
{
	unsigned char TempData[4];				 //临时变量
	unsigned int Velocity;
	EEPROM_read_n(0xE000,TempData,4);
	Velocity = 1000*TempData[0] + 100*TempData[1] + 10*TempData[2] + TempData[3]; 	//计算速度阈值
	
	return Velocity;
}



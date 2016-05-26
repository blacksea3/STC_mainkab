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

#ifndef __FLASH_H__
#define __FLASH_H__ 
#include "stc15.h "		 //	FLASH操作要控制中断开关EA  
#include <intrins.h>		     // FLASH读写要用到 _nop_();
// FLASH读写擦除延时等待时间需要用到晶振频率	
#define SYSclk		11059200L	// 定义CPU实际运行的系统时钟
#define	EEP_address	0x0000	    // 主程序从0000地址开始读写数据 

/******************** 写N个字节函数 最多255字节一次 *****************/
void EEPROM_write_n(unsigned int EE_address,unsigned char *DataAddress,unsigned char lenth);

/******************** 读N个字节函数 最多255字节一次 *****************/
void EEPROM_read_n(unsigned int EE_address,unsigned char *DataAddress,unsigned char lenth);	 

/******************** 扇区擦除函数 *****************/
void EEPROM_SectorErase(unsigned int EE_address);

unsigned int GetVelocityThreshold();
void SetVelocityThreshold(unsigned int ,unsigned char);
#endif
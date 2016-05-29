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

void UART_init(void);			         // 串口1初始化：9600 /22.1184MHZ
void UART_Send_Str(char *s);	         // 发送字符串
void UART_Send_Num(unsigned long dat);	 // 发送数值
void UART_Send_StrNum(char *inf,unsigned long dat);	  // 发送字符串+数值
void UART_Send_Hex(unsigned int hex);				  // 发送16进制(整数范围)
void UART_Send_binary(unsigned char dat);			  // 发送2进制

void UART_Put_Inf(char *inf,unsigned long dat);	  // 发送字符串+数值
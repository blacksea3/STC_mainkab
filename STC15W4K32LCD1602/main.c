#include "LCD1602.H"
#include "myfun.H"
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

void port_mode()            // 端口模式
{
	P0M1=0x00; P0M0=0x00;P1M1=0x00; P1M0=0x00;P2M1=0x00; P2M0=0x00;P3M1=0x00; P3M0=0x00;
	P4M1=0x00; P4M0=0x00;P5M1=0x00; P5M0=0x00;P6M1=0x00; P6M0=0x00;P7M1=0x00; P7M0=0x00; 
}
void main()
{	
	unsigned char xPos,yPos;		 // X坐标、Y坐标
	unsigned char *s="TTESTTESTTESTESTTTTTTTTTTTT";
	unsigned char DispBuf[9];            // 存放4个待发送ASCII码
	unsigned long TestDat=123456789;     // 临时变量
    unsigned char tmp;
		
    for(tmp=0;tmp<10;tmp++)
	{
		delay100ms();
	}

	port_mode();	      // 所有IO口设为准双向弱上拉方式。
	LCD1602_Init();
	SetCur(CurFlash);     // 开光标显示、闪烁，NoCur——有显示无光标,NoDisp——无显示，	
                          // CurNoFlash——有光标但不闪烁	，CurFlash——有光标且闪烁
	xPos=0;	              // xPos表示水平右移字符数(0-15)
	yPos=1;	              // yPos表示垂直下移字符数(0-1)
	WriteString(0,0,"~!@#$%^&*()_+");	  // X坐标、Y坐标、字符串，屏幕左上角为坐标原点，水平：0-15，垂直：0-1
	Long_Str(TestDat,DispBuf);			  // 同第4章8节"利用串口调试程序"
	//WriteString(xPos,yPos,s);	  // X坐标、Y坐标、字符串，屏幕左上角为坐标原点	
	xPos=5;	               
	yPos=1;	
	//WriteString(xPos,yPos,DispBuf);
	WriteString(xPos,yPos,s);	
	while(1)
	{
		/*delay100ms();
		delay100ms();
		delay100ms();
		delay100ms();
		delay100ms();
		delay100ms();
		delay100ms();
		delay100ms();
		delay100ms();
		delay100ms();
		P0=0x00;
		P1=0x00;
		P2=0x00;
		P3=0x00;
		P4=0x00;
		delay100ms();
		delay100ms();
		delay100ms();
		delay100ms();
		delay100ms();
		delay100ms();
		delay100ms();
		delay100ms();
		delay100ms();
		delay100ms();
		P0=0xff;
		P1=0xff;
		P2=0xff;
		P3=0xff;
		P4=0xff;*/
	}
} 
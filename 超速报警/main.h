#ifndef MAIN_H
#define MAIN_H

extern unsigned char DS3231ISREADY;					//1==DS1302需要读取
extern unsigned char DHT11ISREADY;					//1==DHT11需要获取
extern unsigned char ULTRAISREADY;					//1==超声波速度需要获取
extern unsigned char FASTSPEED;						//1==超速了需要报警
extern unsigned char BUZZERTIMER3STOP;				//1==蜂鸣器使用的定时器3需要停止
extern unsigned char ISSETTING;						//1==进入设置模式,0==进入普通模式
extern unsigned char EXITSETTING;					//1==已经退出设置模式

extern unsigned char ADCSTARTREADY;
extern unsigned char ADCSTOPREADY;
extern unsigned char ADCDISPLAYREADY;

extern unsigned char UART1RIREADY;
extern unsigned char UART2RIREADY;
extern unsigned char WIFINEEDDELAY;

extern unsigned char VelocityThreshold;			   	//速度阈值km/h

#endif
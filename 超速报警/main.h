#ifndef MAIN_H
#define MAIN_H

extern unsigned char DS3231ISREADY;					//1==DS1302��Ҫ��ȡ
extern unsigned char DHT11ISREADY;					//1==DHT11��Ҫ��ȡ
extern unsigned char ULTRAISREADY;					//1==�������ٶ���Ҫ��ȡ
extern unsigned char FASTSPEED;						//1==��������Ҫ����
extern unsigned char BUZZERTIMER3STOP;				//1==������ʹ�õĶ�ʱ��3��Ҫֹͣ
extern unsigned char ISSETTING;						//1==��������ģʽ,0==������ͨģʽ
extern unsigned char EXITSETTING;					//1==�Ѿ��˳�����ģʽ

extern unsigned char ADCSTARTREADY;
extern unsigned char ADCSTOPREADY;
extern unsigned char ADCDISPLAYREADY;

extern unsigned char UART1RIREADY;
extern unsigned char UART2RIREADY;
extern unsigned char WIFINEEDDELAY;

extern unsigned char VelocityThreshold;			   	//�ٶ���ֵkm/h

#endif
#include "stc15.h"

static unsigned char VAILDSPEED=20;         //合法速度
unsigned char ForceTime;      				//中断次数

//------------开一个中断
void ForceInit()
{
	ForceTime = 0;			  				//中断次数清零
	//开中断0
	
}

//------------计算速度
unsigned char CalSpeed()
{
	unsigned char TempSpeed=0;
	//取两个时间,减一下（注意处理定时器值飞出去的情况,当前未处理）
	return TempSpeed;
}

//------------处理压力传感器值和时间,判断并发送数据
void AnalyseForce()
{
    unsigned char speed;
	//读时间
	//读大小?
	;
	if(++ForceTime==2)		  				//第二次中断
	{
		ForceTime = 0;        				//中断次数清零
		speed=CalSpeed();	  				//计算速度
		if(speed>VAILDSPEED)  				//如果超速
		{
		    OpenCamera();                   //开摄像头
			WifiSend();                     //发送数据(拿摄像头的数据连续发,这个认为较难实现)
			WifiSend();                     //发送数据,给蜂鸣器用的
		}
		else                                //如果未超速,不处理
		{
		    ;
		}
	}
}

//------------中断0函数
/*void ???? using interrupt ??()
{
	AnalyseForce();             //读一下值和时间()?
}*/
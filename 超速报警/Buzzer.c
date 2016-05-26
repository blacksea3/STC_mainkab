#include "stc15.h"

//(使用定时器3)
void Buzzer()
{
	//P55 = !P55;				//无源蜂鸣器
	P55 = 0;					//有源蜂鸣器
}
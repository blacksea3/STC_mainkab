#include "stc15.h"

//(使用定时器3)
void Buzzer()
{
	P07 = !P07;
}
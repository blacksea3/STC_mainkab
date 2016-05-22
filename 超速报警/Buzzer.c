#include "stc15.h"

//(使用定时器3)
void Buzzer()
{
	P55 = !P55;
}
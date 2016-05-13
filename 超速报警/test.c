#include "stc15.h"
sbit led0 = P0^0;

void delay(unsigned int x) 
{ 
	unsigned int a,b;
	for(a=0;a<x;a++)
		for(b=1000;b>0;b--);
}

void main() 
{
	while(1)
	{
		led0 = 0;
		delay(1000);
		led0 = 1;
		delay(1000);
	}
}
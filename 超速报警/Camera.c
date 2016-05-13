#include "stc15.h"

//----------------初始化摄像头
void CameraInit()
{
	//准备定时器1但并不开启,用于延时中断
	;
}

//----------------关摄像头
void CloseCamera()
{
	;
}

//----------------定时器1中断函数
/*void xxx using interrupt xxx()
{
	CloseCamera();
}*/


//----------------开摄像头
void OpenCamera()			  
{
	//开定时器1,几十ms后触发中断并关闭摄像头
	;
}


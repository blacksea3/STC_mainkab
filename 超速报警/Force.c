#include "stc15.h"

static unsigned char VAILDSPEED=20;         //�Ϸ��ٶ�
unsigned char ForceTime;      				//�жϴ���

//------------��һ���ж�
void ForceInit()
{
	ForceTime = 0;			  				//�жϴ�������
	//���ж�0
	
}

//------------�����ٶ�
unsigned char CalSpeed()
{
	unsigned char TempSpeed=0;
	//ȡ����ʱ��,��һ�£�ע�⴦��ʱ��ֵ�ɳ�ȥ�����,��ǰδ����
	return TempSpeed;
}

//------------����ѹ��������ֵ��ʱ��,�жϲ���������
void AnalyseForce()
{
    unsigned char speed;
	//��ʱ��
	//����С?
	;
	if(++ForceTime==2)		  				//�ڶ����ж�
	{
		ForceTime = 0;        				//�жϴ�������
		speed=CalSpeed();	  				//�����ٶ�
		if(speed>VAILDSPEED)  				//�������
		{
		    OpenCamera();                   //������ͷ
			WifiSend();                     //��������(������ͷ������������,�����Ϊ����ʵ��)
			WifiSend();                     //��������,���������õ�
		}
		else                                //���δ����,������
		{
		    ;
		}
	}
}

//------------�ж�0����
/*void ???? using interrupt ??()
{
	AnalyseForce();             //��һ��ֵ��ʱ��()?
}*/
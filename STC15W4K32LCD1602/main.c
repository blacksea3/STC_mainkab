#include "LCD1602.H"
#include "myfun.H"
/****************��51��Ƭ����������-����STC15W4Kϵ�С��������� *************
 �������������������������
 ��51��Ƭ����������-����STC15W4Kϵ�С� һ���Ѿ��ɱ�����������ʽ���淢�С�
  �������ִ�������̲����׵�51˫��ʵ���(2��MCU)�Գ������ء����ԡ����淽�㣬����Ҫ�ⲿ
  ��������������������Ʒ�ʽ���׽����ϵͳ�ж��������ȼ�˭Ҳ������˭���жϾ������⡣
  �Ա����ַ��https://shop117387413.taobao.com
  QQȺ��STC51-STM32(3) ��515624099 �� STC51-STM32(2)��99794374��
        ��֤��Ϣ��STC15��Ƭ��
  ���䣺xgliyouquan@126.com
  �������������������������*/

void port_mode()            // �˿�ģʽ
{
	P0M1=0x00; P0M0=0x00;P1M1=0x00; P1M0=0x00;P2M1=0x00; P2M0=0x00;P3M1=0x00; P3M0=0x00;
	P4M1=0x00; P4M0=0x00;P5M1=0x00; P5M0=0x00;P6M1=0x00; P6M0=0x00;P7M1=0x00; P7M0=0x00; 
}
void main()
{	
	unsigned char xPos,yPos;		 // X���ꡢY����
	unsigned char *s="TTESTTESTTESTESTTTTTTTTTTTT";
	unsigned char DispBuf[9];            // ���4��������ASCII��
	unsigned long TestDat=123456789;     // ��ʱ����
    unsigned char tmp;
		
    for(tmp=0;tmp<10;tmp++)
	{
		delay100ms();
	}

	port_mode();	      // ����IO����Ϊ׼˫����������ʽ��
	LCD1602_Init();
	SetCur(CurFlash);     // �������ʾ����˸��NoCur��������ʾ�޹��,NoDisp��������ʾ��	
                          // CurNoFlash�����й�굫����˸	��CurFlash�����й������˸
	xPos=0;	              // xPos��ʾˮƽ�����ַ���(0-15)
	yPos=1;	              // yPos��ʾ��ֱ�����ַ���(0-1)
	WriteString(0,0,"~!@#$%^&*()_+");	  // X���ꡢY���ꡢ�ַ�������Ļ���Ͻ�Ϊ����ԭ�㣬ˮƽ��0-15����ֱ��0-1
	Long_Str(TestDat,DispBuf);			  // ͬ��4��8��"���ô��ڵ��Գ���"
	//WriteString(xPos,yPos,s);	  // X���ꡢY���ꡢ�ַ�������Ļ���Ͻ�Ϊ����ԭ��	
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
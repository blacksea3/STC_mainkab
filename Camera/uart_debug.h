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

void UART_init(void);			         // ����1��ʼ����9600 /22.1184MHZ
void UART_Send_Str(char *s);	         // �����ַ���
void UART_Send_Num(unsigned long dat);	 // ������ֵ
void UART_Send_StrNum(char *inf,unsigned long dat);	  // �����ַ���+��ֵ
void UART_Send_Hex(unsigned int hex);				  // ����16����(������Χ)
void UART_Send_binary(unsigned char dat);			  // ����2����

void UART_Put_Inf(char *inf,unsigned long dat);	  // �����ַ���+��ֵ
#include "Sensor.h"
#include "myfun.h"
#include "stc15.h"
#include "uart_debug.h"
const  char OV7670_reg[OV7670_REG_NUM][2]=
{       
       
        {0x3a, 0x04},
        {0x40, 0xd0},
        {0x12, 0x14},
        {0x32, 0x80},
        {0x17, 0x16},
        {0x18, 0x04},
        {0x19, 0x02},
        {0x1a, 0x7b},//0x7a,
        {0x03, 0x06},//0x0a,
        {0x0c, 0x00},
        {0x3e, 0x00},//
        {0x70, 0x00},
        {0x71, 0x81},
        {0x72, 0x11},
        {0x73, 0x00},//
        {0xa2, 0x02},
        {0x11, 0x81},
        {0x7a, 0x20},
        {0x7b, 0x1c},
        {0x7c, 0x28},
        {0x7d, 0x3c},
        {0x7e, 0x55},
        {0x7f, 0x68},
        {0x80, 0x76},
        {0x81, 0x80},
        {0x82, 0x88},
        {0x83, 0x8f},
        {0x84, 0x96},
        {0x85, 0xa3},
        {0x86, 0xaf},
        {0x87, 0xc4},
        {0x88, 0xd7},
        {0x89, 0xe8},
        {0x13, 0xe0},
        {0x00, 0x00},//AGC
        {0x10, 0x00},
        {0x0d, 0x00},
        {0x14, 0x28},//0x38, limit the max gain
        {0xa5, 0x05},
        {0xab, 0x07},
        {0x24, 0x75},
        {0x25, 0x63},
        {0x26, 0xA5},
        {0x9f, 0x78},
        {0xa0, 0x68},
        {0xa1, 0x03},//0x0b,
        {0xa6, 0xdf},//0xd8,
        {0xa7, 0xdf},//0xd8,
        {0xa8, 0xf0},
        {0xa9, 0x90},
        {0xaa, 0x94},
        {0x13, 0xe5},
        {0x0e, 0x61},
        {0x0f, 0x4b},
        {0x16, 0x02},
        {0x1e, 0x07},//0x07,
        {0x21, 0x02},
        {0x22, 0x91},
        {0x29, 0x07},
        {0x33, 0x0b},
        {0x35, 0x0b},
        {0x37, 0x1d},
        {0x38, 0x71},
        {0x39, 0x2a},
        {0x3c, 0x78},
        {0x4d, 0x40},
        {0x4e, 0x20},
        {0x69, 0x55},
        {0x6b, 0x40},//PLL
        {0x74, 0x19},
        {0x8d, 0x4f},
        {0x8e, 0x00},
        {0x8f, 0x00},
        {0x90, 0x00},
        {0x91, 0x00},
        {0x92, 0x00},//0x19,//0x66
        {0x96, 0x00},
        {0x9a, 0x80},
        {0xb0, 0x84},
        {0xb1, 0x0c},
        {0xb2, 0x0e},
        {0xb3, 0x82},
        {0xb8, 0x0a},
        {0x43, 0x14},
        {0x44, 0xf0},
        {0x45, 0x34},
        {0x46, 0x58},
        {0x47, 0x28},
        {0x48, 0x3a},
        {0x59, 0x88},
        {0x5a, 0x88},
        {0x5b, 0x44},
        {0x5c, 0x67},
        {0x5d, 0x49},
        {0x5e, 0x0e},
        {0x64, 0x04},
        {0x65, 0x20},
        {0x66, 0x05},
        {0x94, 0x04},
        {0x95, 0x08},
        {0x6c, 0x0a},
        {0x6d, 0x55},
        {0x6e, 0x11},
        {0x6f, 0x9f},//0x9e for advance AWB
        {0x6a, 0x40},
        {0x01, 0x40},
        {0x02, 0x40},
        {0x13, 0xe7},
        {0x15, 0x00},
        {0x4f, 0x80},
        {0x50, 0x80},
        {0x51, 0x00},
        {0x52, 0x22},
        {0x53, 0x5e},
        {0x54, 0x80},
        {0x55, 0x0A},//����
        {0x56, 0x4A},//�Աȶ�
        {0x58, 0x9e},       
        {0x41, 0x08},
        {0x3f, 0x05},//��Ե��ǿ����
        {0x75, 0x05},
        {0x76, 0xe1},
        {0x4c, 0x0F},//��������ǿ��
        {0x77, 0x0a},
        {0x3d, 0xc2},        //0xc0,
        {0x4b, 0x09},
        {0xc9, 0x60},
        {0x41, 0x38},
        {0x56, 0x40},//0x40,  change according to Jim's request       
        {0x34, 0x11},
        {0x3b, 0x02},//0x00,//0x02,
        {0xa4, 0x89},//0x88,
        {0x96, 0x00},
        {0x97, 0x30},
        {0x98, 0x20},
        {0x99, 0x30},
        {0x9a, 0x84},
        {0x9b, 0x29},
        {0x9c, 0x03},
        {0x9d, 0x4c},
        {0x9e, 0x3f},
        {0x78, 0x04},       
        {0x79, 0x01},
        {0xc8, 0xf0},
        {0x79, 0x0f},
        {0xc8, 0x00},
        {0x79, 0x10},
        {0xc8, 0x7e},
        {0x79, 0x0a},
        {0xc8, 0x80},
        {0x79, 0x0b},
        {0xc8, 0x01},
        {0x79, 0x0c},
        {0xc8, 0x0f},
        {0x79, 0x0d},
        {0xc8, 0x20},
        {0x79, 0x09},
        {0xc8, 0x80},
        {0x79, 0x02},
        {0xc8, 0xc0},
        {0x79, 0x03},
        {0xc8, 0x40},
        {0x79, 0x05},
        {0xc8, 0x30},
        {0x79, 0x26},
        {0x09, 0x02},
        {0x3b, 0x42},//0x82,//0xc0,//0xc2,        //night mode
};




/*char code OV7670_reg[OV7670_REG_NUM][2]=
{	 
 
     *//*����ΪOV7670 QVGA RGB565���� 
  	{0x3a, 0x04},//dummy
	{0x40, 0x10},//565
	{0x12, 0x14},//@
	{0x32, 0x80},//HREF control	bit[2:0] HREF start 3 LSB	 bit[5:3] HSTOP HREF end 3LSB
	{0x17, 0x16},//              HSTART start high 8-bit MSB
        
	{0x18, 0x04},//5	 HSTOP end high 8-bit
	{0x19, 0x02},
	{0x1a, 0x7b},//0x7a,
	{0x03, 0x06},//0x0a,
	{0x0c, 0x0c},
        {0x15, 0x02},
	{0x3e, 0x00},//10
	{0x70, 0x00},
	{0x71, 0x01},
	{0x72, 0x11},
	{0x73, 0x09},//
        
	{0xa2, 0x02},//15
	{0x11, 0x03}, //fenpin
	{0x7a, 0x20},
	{0x7b, 0x1c},
	{0x7c, 0x28},
        
	{0x7d, 0x3c},//20
	{0x7e, 0x55},
	{0x7f, 0x68},
	{0x80, 0x76},
	{0x81, 0x80},
        
	{0x82, 0x88},
	{0x83, 0x8f},
	{0x84, 0x96},
	{0x85, 0xa3},
	{0x86, 0xaf},
        
	{0x87, 0xc4},//30
	{0x88, 0xd7},
	{0x89, 0xe8},
	{0x13, 0xe0},
	{0x00, 0x00},//AGC
        
	{0x10, 0x00},
	{0x0d, 0x00},
	{0x14, 0x20},//0x38, limit the max gain
	{0xa5, 0x05},
	{0xab, 0x07},
        
	{0x24, 0x75},//40
	{0x25, 0x63},
	{0x26, 0xA5},
	{0x9f, 0x78},
	{0xa0, 0x68},
        
	{0xa1, 0x03},//0x0b,
	{0xa6, 0xdf},//0xd8,
	{0xa7, 0xdf},//0xd8,
	{0xa8, 0xf0},
	{0xa9, 0x90},
        
	{0xaa, 0x94},//50
	{0x13, 0xe5},
	{0x0e, 0x61},
	{0x0f, 0x4b},
	{0x16, 0x02},
        
	{0x1e, 0x37},//0x07,
	{0x21, 0x02},
	{0x22, 0x91},
	{0x29, 0x07},
	{0x33, 0x0b},
        
	{0x35, 0x0b},//60
	{0x37, 0x1d},
	{0x38, 0x71},
	{0x39, 0x2a},
	{0x3c, 0x78},
        
	{0x4d, 0x40},
	{0x4e, 0x20},
	{0x69, 0x5d},
	{0x6b, 0x40},//PLL
	{0x74, 0x19},
	{0x8d, 0x4f},
        
	{0x8e, 0x00},//70
	{0x8f, 0x00},
	{0x90, 0x00},
	{0x91, 0x00},
	{0x92, 0x00},//0x19,//0x66
        
	{0x96, 0x00},
	{0x9a, 0x80},
	{0xb0, 0x84},
	{0xb1, 0x0c},
	{0xb2, 0x0e},
        
	{0xb3, 0x82},//80
	{0xb8, 0x0a},
	{0x43, 0x14},
	{0x44, 0xf0},
	{0x45, 0x34},
        
	{0x46, 0x58},
	{0x47, 0x28},
	{0x48, 0x3a},
	{0x59, 0x88},
	{0x5a, 0x88},
        
	{0x5b, 0x44},//90
	{0x5c, 0x67},
	{0x5d, 0x49},
	{0x5e, 0x0e},
	{0x64, 0x04},
	{0x65, 0x20},
        
	{0x66, 0x05},
	{0x94, 0x04},
	{0x95, 0x08},
	{0x6c, 0x0a},
	{0x6d, 0x55},
        
        
	{0x4f, 0x80},
	{0x50, 0x80},
	{0x51, 0x00},
	{0x52, 0x22},
	{0x53, 0x5e},
	{0x54, 0x80},
        
	//{0x54, 0x40},//110
        
        
	{0x6e, 0x11},//100
	{0x6f, 0x9f},//0x9e for advance AWB
        {0x55, 0x00},//����
        {0x56, 0x20},//�Աȶ�
        {0x57, 0x40},//0x40,  change according to Jim's request	      

};*/

//=================================================
//���ܣ�дOV7660�Ĵ���
//���أ�1-�ɹ�	0-ʧ��
//=================================================

unsigned char wr_Sensor_Reg(unsigned char regID, unsigned char regDat)
{
 startSCCB(); //����SCCB ���߿�ʼ��������

 if(0==SCCBwriteByte(0x42))//д��ַ
 {	
  stopSCCB();//����SCCB ����ֹͣ��������
  return(0);//���󷵻�
 }

 //delay(50000);

 UART_Put_Inf("wr_Sensor_Reg:",1);
 //SendString("wr_Sensor_Reg:");

 if(0==SCCBwriteByte(regID))//������ID
 {
  stopSCCB();//����SCCB ����ֹͣ��������
  return(0); //���󷵻�
 }

 //delay(50000);
 UART_Put_Inf("wr_Sensor_Reg:",2);
 //SendString("wr_Sensor_Reg:");

 if(0==SCCBwriteByte(regDat))//д���ݵ�������
 {
  stopSCCB();//����SCCB ����ֹͣ��������
  return(0);//���󷵻�
 }

 stopSCCB();//����SCCB ����ֹͣ��������

 UART_Put_Inf("wr_Sensor_Reg:",3);
 //SendString("wr_Sensor_Reg:");

 return(1);//�ɹ�����
}

//=================================================
//���ܣ���OV7660�Ĵ���
//���أ�1-�ɹ�	0-ʧ��
//===================================================
unsigned char rd_Sensor_Reg(unsigned char regID, unsigned char *regDat)
{
 //ͨ��д�������üĴ�����ַ

 startSCCB();

 if(0==SCCBwriteByte(0x42))//д��ַ
 {
  stopSCCB();//����SCCB ����ֹͣ��������
  return(0);//���󷵻�
 }
	
 //delay(50000);

 if(0==SCCBwriteByte(regID))//������ID
 {
  stopSCCB();//����SCCB ����ֹͣ��������
  return(0);//���󷵻�
 }
 stopSCCB();//����SCCB ����ֹͣ��������
	
 //delay(50000);
	
 //���üĴ�����ַ�󣬲��Ƕ�
 startSCCB();

 if(0==SCCBwriteByte(0x43))//����ַ
 {
  stopSCCB();//����SCCB ����ֹͣ��������
  return(0);//���󷵻�
 }
	
 //delay(50000);
 *regDat=SCCBreadByte();//���ض�����ֵ

 noAck();//����NACK����
 stopSCCB();//����SCCB ����ֹͣ��������

 return(1);//�ɹ�����
}

//===============================================
//Sensor_init()  ����ͷоƬ��ʼ��
//����1�ɹ�������0ʧ��
//=================================================
unsigned char Sensor_init(void)
{
 unsigned char idata temp;	
 unsigned int idata i=0;

 //XCLK_init_ON();//����MCO���� �ṩʱ�Ӹ�CMOS������
 //uchar ovidmsb=0,ovidlsb=0;
 //Sensor_GPIO_Init();
 //SCCB_GPIO_Config();//io init..
        
 temp=0x80;
	
 if(0==wr_Sensor_Reg(0x12,temp)) //Reset SCCB
 {
  UART_Put_Inf("Reset SCCB error",temp);
  return 0 ;//���󷵻�
 }
	
 //delay(50000);
 UART_Put_Inf("Sensor_init:",1);
 //SendString("Sensor_init:");


	
 
 //SendString("Sensor_init2:");

 while(temp!=0x73)
 {
 	if(0==rd_Sensor_Reg(0x0b, &temp))//��ID
 	{
 	 UART_Put_Inf("Read Id error",temp);
 	 return 0 ;//���󷵻�
 	}
 	UART_Put_Inf("Sensor_init:",temp);
 }

 if(temp==0x73)//OV7670
 {
  for(i=0;i<OV7670_REG_NUM;i++)
  {
   //delay(500);
   UART_Put_Inf("Sensor_init:cycle",i);
   //SendString("Sensor_init:cycle");

   if(0==wr_Sensor_Reg(OV7670_reg[i][0],OV7670_reg[i][1]))
   {
    UART_Put_Inf("Sensor_init:cycle error",i);
	return 0;//���󷵻�
   }
  }
 }

 UART_Put_Inf("Sensor_init:ok",0);
 //SendString("Sensor_init:ok");

 return 1; //ok
} 

///////////////////

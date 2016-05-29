#include "fifo.h"
#include "myfun.h"

int FIFO_Reset_Read_Addr(void)
{
 FIFO_RRST=0;
 delay(10);
 FIFO_RCLK=0; 
 delay(10);
 FIFO_RCLK=1;
 delay(10); 
 FIFO_RCLK=0; 
 delay(10);

 FIFO_RRST=1;  
 delay(10);
 FIFO_RCLK=1; 
 delay(10);
 return 0; 
}

int FIFO_Read_Words(unsigned int len,unsigned char *pbuf)
{
 unsigned int idata i=0;

 DATA_PORT=0XFF;

 for(i=0;i<len;i++)
 {
  FIFO_RCLK=0;
  *(pbuf+1)=DATA_PORT;

  FIFO_RCLK=1;
  FIFO_RCLK=0;
  *(pbuf)=DATA_PORT;

  FIFO_RCLK=1;

  pbuf+=2;
 }

 return 0;
}


#ifndef UART_H
#define UART_H

typedef unsigned char BYTE;

void SendData(BYTE);
void SendString(char *s);
void UARTInit();

void SendData2(BYTE);
void SendString2(char *s);
void UART2Init();

void Uart1SendUart2String();
void Uart2SendUart1String();
void SendString2Length(char s[], unsigned char length);

extern unsigned char UART1Temp;
extern unsigned char UART2Temp;

#endif
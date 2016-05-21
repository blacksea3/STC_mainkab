#ifndef UART_H
#define UART_H

typedef unsigned char BYTE;

void SendData(BYTE);
void SendString(char *s);
void UARTInit();

#endif
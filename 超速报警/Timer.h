#ifndef TIMER_H
#define TIMER_H

void Timer0Init();
void Timer1Init();
void EnableTimer1();
void DisableTimer1();
void Timer3Init();
void EnableTimer3();
void DisableTimer3();
void Timer4Init();
void EnableTimer4();
void DisableTimer4();

extern unsigned int T1times;

#endif
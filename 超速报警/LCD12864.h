#ifndef LCD12864_H
#define LCD12864_H

void Lcm_Init();
void LCD12864SettingInit();
void Display_String(unsigned char,unsigned char *);
void LCD12864DisplayChar(unsigned char , unsigned char , unsigned char );
void LCD12864SettingExit();

#endif
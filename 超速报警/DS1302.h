void DS1302Init(unsigned char *);
void DS3231read_time();
void DS1302_readoutTime(unsigned char *);
void set_time(unsigned char *);

sbit SCK = P2^3;	           // ʱ��	
sbit SDA = P2^2;   	           // ����
sbit RST = P2^1;               // DS1302��λ(Ƭѡ)
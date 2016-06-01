void DS1302Init(unsigned char *);
void DS3231read_time();
void DS1302_readoutTime(unsigned char *);
void set_time(unsigned char *);

sbit SCK = P2^3;	           // 时钟	
sbit SDA = P2^2;   	           // 数据
sbit RST = P2^1;               // DS1302复位(片选)
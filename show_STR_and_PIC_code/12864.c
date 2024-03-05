#include<reg51.h>
#include"12864.h"
//基础的引脚定义
#define DB P0
sbit lcd12864_EN = P2^7;
sbit lcd12864_RS = P2^6;
sbit lcd12864_RW = P2^5;

sbit lcd12864_RST = P3^4;

/*------------------延时函数----------------------*/

void delay100us(void)   //误差 0us
{
    unsigned char a,b;
    for(b=19;b>0;b--)
        for(a=1;a>0;a--);
}

void delay200us(void)   //误差 0us
{
    unsigned char a,b;
    for(b=39;b>0;b--)
        for(a=1;a>0;a--);
}

void delay15ms(void)   //误差 0us
{
    unsigned char a,b,c;
    for(c=5;c>0;c--)
        for(b=28;b>0;b--)
            for(a=52;a>0;a--);
}


void delay50ms(void)   //误差 0us
{
    unsigned char a,b;
    for(b=165;b>0;b--)
        for(a=150;a>0;a--);
}

/*-------------------基础时序---------------------------*/
//忙检测  + 超时检测
void Is_busy(){
	unsigned int timeout = 0;
 	lcd12864_EN = 0;
 	lcd12864_RS = 0;
 	lcd12864_RW = 1;
 	lcd12864_EN = 1;
 	while((DB & 0x80) && ++timeout != 0);  //忙状态检测，等待超时时间为60ms
 	lcd12864_EN = 0;	
}
//写指令
void lcd12864_write_command(unsigned char command){
	Is_busy();
	lcd12864_EN = 0;
	lcd12864_RS = 0;
	lcd12864_RW = 0;
	DB = command;
	lcd12864_EN = 1;
	delay100us();
	lcd12864_EN = 0;
	delay100us();	
}

//读数据

unsigned char lcd12864_read_data(){
	unsigned char dat;
	Is_busy();
	lcd12864_EN = 0;
	DB = 0xff; // DB设置高电平代表我要读引脚的数据了
	lcd12864_RS = 1;
	lcd12864_RW = 1;
	lcd12864_EN = 1;
	delay100us();
	dat = DB;
	return dat;
}

//写数据
void lcd12864_write_data(unsigned char dat){
	Is_busy();
	lcd12864_EN = 0;
	lcd12864_RW = 0;
	lcd12864_RS = 1;
	lcd12864_EN = 1;
	DB = dat;
	delay100us();
	lcd12864_EN = 0; 	
}

/*----------------高层时序------------------*/
//清屏操作
void clear_screen(){
	Is_busy();
	lcd12864_write_command(0x01);
}

//初始化函数
void lcd12864_init(){
	delay50ms();
	lcd12864_RST = 1;
	lcd12864_RST = 0;
	delay50ms();
	lcd12864_RST = 1;//设置reset
	
	lcd12864_write_command(0x30);//设置为基本指令集
	delay200us();
	lcd12864_write_command(0x30);
	delay200us();
	
	lcd12864_write_command(0x0c); //display control显示，光标，闪烁都开，让我知道光标在哪里
	delay200us();

	lcd12864_write_command(0x01); //清屏模式 DDRAM数据指针清零
	delay15ms();

	lcd12864_write_command(0x06); //进入模式设置
	//===初始化结束------
}

//在指定的位置显示指定的内容
void lcd12864_display_pos(unsigned char x,unsigned char y){	 //注意128代表的是列的数目，64代表的是行的数目。因此，x代表的是列。y是行。【这句话没有错】
	//针对汉字：整个屏幕被分成四行，列数是8.针对字符：行数是4，列数是16。【具体去看图】
	unsigned char displaystart = 0;
	if(y==0)displaystart=0x80+x;
	if(y==1)displaystart=0x90+x;
	if(y==2)displaystart=0x88+x;
	if(y==3)displaystart=0x98+x;
	lcd12864_write_command(displaystart);//调用写指令函数确定在哪开始显示我们需要的内容	
}
//显示具体的字符
void lcd12864_show_str(unsigned char x,unsigned char y,char mystr){
	lcd12864_display_pos(x,y);
	lcd12864_write_data(mystr);	
}
//显示具体的汉字
void lcd12864_show_string(unsigned char x,unsigned char y,char mystr[]){
	lcd12864_display_pos(x,y);
	while(*mystr!='\0'){
		lcd12864_write_data(*mystr++);
	}
		
}

//尝试画图


















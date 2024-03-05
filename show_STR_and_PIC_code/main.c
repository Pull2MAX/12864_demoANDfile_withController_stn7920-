#include"12864.h"
#include<intrins.h>
#include"tupian.h"

/*
NOTE:
1.液晶显示模块是128×64 点阵的汉字图形型液晶显示模块，
可显示汉字及图形，内置国标GB2312 码简体中文字库（16X16 点阵）、128 个字符（8X16 点阵）及64X256 点阵显示RAM（GDRAM）。
可与CPU 直接接口，提供两种界面来连接微处理机：8-位并行及串行两种连接方式。具有多种功能：光标显示、画面移位、睡眠模式等。

2.ST7920的字符显示RAM (DDRAM)最多可以控制16字元*4行，LCD的显示范围为16字元*2行。

*/

void delay1s(void)   //误差 0us
{
    unsigned char a,b,c;
    for(c=46;c>0;c--)
        for(b=152;b>0;b--)
            for(a=70;a>0;a--);
    _nop_();  //if Keil,require use intrins.h	 
}

void show_zifu_hanzi(){
lcd12864_write_command(0x30);
delay1s();
lcd12864_init();
lcd12864_show_string(2,0,"lcd 12864");
lcd12864_show_string(3,1,"周2");
lcd12864_show_string(4,2,"の晚上");
lcd12864_show_str(0,3,0x03);//注意，显示半宽字符是用str
lcd12864_show_string(2,3,"19点13分");
lcd12864_show_str(7,3,0x03);	
}

void show_picture(unsigned char *tupian){
	unsigned char i,j;
	lcd12864_init();	

	lcd12864_write_command(0x34);//图片指令属于是扩展指令集
	
	for(i = 0;i < 32;++i){	 //显示的是上半屏
		lcd12864_write_command(0x80+i);//这个控制的是行,行数是不会加1的
		lcd12864_write_command(0x80);//这个控制的是列，列数是自动加1
		for(j = 0;j < 16;++j){
		lcd12864_write_data(*tupian++);	
		}		
	}

	for(i = 0;i < 32;++i){	 //显示的是下半屏
		lcd12864_write_command(0x80+i);//这个控制的是行,行数是不会加1的
		lcd12864_write_command(0x88);//这个控制的是列，列数是自动加1
		for(j = 0;j < 16;++j){
		lcd12864_write_data(*tupian++);	
		}		
	}
	
	lcd12864_write_command(0x36);//开启绘图功能
	lcd12864_write_command(0x30);//回到基本指令集

		
}
void main(){
	//show_zifu_hanzi();

	show_picture(picture);
	while(1);
}



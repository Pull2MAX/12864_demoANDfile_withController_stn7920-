#ifndef	_12864_H_
#define _12864_H_

void clear_screen();//≤‚ ‘”√£¨
void lcd12864_write_command(unsigned char command);
void lcd12864_write_data(unsigned char dat);
void lcd12864_init();
void lcd12864_display_pos(unsigned char x,unsigned char y);
void lcd12864_show_str(unsigned char x,unsigned char y,char mystr);
void lcd12864_show_string(unsigned char x,unsigned char y,char mystr[]);


#endif
#include"12864.h"
#include<intrins.h>
#include"tupian.h"

/*
NOTE:
1.Һ����ʾģ����128��64 ����ĺ���ͼ����Һ����ʾģ�飬
����ʾ���ּ�ͼ�Σ����ù���GB2312 ����������ֿ⣨16X16 ���󣩡�128 ���ַ���8X16 ���󣩼�64X256 ������ʾRAM��GDRAM����
����CPU ֱ�ӽӿڣ��ṩ���ֽ���������΢�������8-λ���м������������ӷ�ʽ�����ж��ֹ��ܣ������ʾ��������λ��˯��ģʽ�ȡ�

2.ST7920���ַ���ʾRAM (DDRAM)�����Կ���16��Ԫ*4�У�LCD����ʾ��ΧΪ16��Ԫ*2�С�

*/

void delay1s(void)   //��� 0us
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
lcd12864_show_string(3,1,"��2");
lcd12864_show_string(4,2,"������");
lcd12864_show_str(0,3,0x03);//ע�⣬��ʾ����ַ�����str
lcd12864_show_string(2,3,"19��13��");
lcd12864_show_str(7,3,0x03);	
}

void show_picture(unsigned char *tupian){
	unsigned char i,j;
	lcd12864_init();	

	lcd12864_write_command(0x34);//ͼƬָ����������չָ�
	
	for(i = 0;i < 32;++i){	 //��ʾ�����ϰ���
		lcd12864_write_command(0x80+i);//������Ƶ�����,�����ǲ����1��
		lcd12864_write_command(0x80);//������Ƶ����У��������Զ���1
		for(j = 0;j < 16;++j){
		lcd12864_write_data(*tupian++);	
		}		
	}

	for(i = 0;i < 32;++i){	 //��ʾ�����°���
		lcd12864_write_command(0x80+i);//������Ƶ�����,�����ǲ����1��
		lcd12864_write_command(0x88);//������Ƶ����У��������Զ���1
		for(j = 0;j < 16;++j){
		lcd12864_write_data(*tupian++);	
		}		
	}
	
	lcd12864_write_command(0x36);//������ͼ����
	lcd12864_write_command(0x30);//�ص�����ָ�

		
}
void main(){
	//show_zifu_hanzi();

	show_picture(picture);
	while(1);
}



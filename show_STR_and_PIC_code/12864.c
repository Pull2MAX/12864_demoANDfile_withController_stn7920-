#include<reg51.h>
#include"12864.h"
//���������Ŷ���
#define DB P0
sbit lcd12864_EN = P2^7;
sbit lcd12864_RS = P2^6;
sbit lcd12864_RW = P2^5;

sbit lcd12864_RST = P3^4;

/*------------------��ʱ����----------------------*/

void delay100us(void)   //��� 0us
{
    unsigned char a,b;
    for(b=19;b>0;b--)
        for(a=1;a>0;a--);
}

void delay200us(void)   //��� 0us
{
    unsigned char a,b;
    for(b=39;b>0;b--)
        for(a=1;a>0;a--);
}

void delay15ms(void)   //��� 0us
{
    unsigned char a,b,c;
    for(c=5;c>0;c--)
        for(b=28;b>0;b--)
            for(a=52;a>0;a--);
}


void delay50ms(void)   //��� 0us
{
    unsigned char a,b;
    for(b=165;b>0;b--)
        for(a=150;a>0;a--);
}

/*-------------------����ʱ��---------------------------*/
//æ���  + ��ʱ���
void Is_busy(){
	unsigned int timeout = 0;
 	lcd12864_EN = 0;
 	lcd12864_RS = 0;
 	lcd12864_RW = 1;
 	lcd12864_EN = 1;
 	while((DB & 0x80) && ++timeout != 0);  //æ״̬��⣬�ȴ���ʱʱ��Ϊ60ms
 	lcd12864_EN = 0;	
}
//дָ��
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

//������

unsigned char lcd12864_read_data(){
	unsigned char dat;
	Is_busy();
	lcd12864_EN = 0;
	DB = 0xff; // DB���øߵ�ƽ������Ҫ�����ŵ�������
	lcd12864_RS = 1;
	lcd12864_RW = 1;
	lcd12864_EN = 1;
	delay100us();
	dat = DB;
	return dat;
}

//д����
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

/*----------------�߲�ʱ��------------------*/
//��������
void clear_screen(){
	Is_busy();
	lcd12864_write_command(0x01);
}

//��ʼ������
void lcd12864_init(){
	delay50ms();
	lcd12864_RST = 1;
	lcd12864_RST = 0;
	delay50ms();
	lcd12864_RST = 1;//����reset
	
	lcd12864_write_command(0x30);//����Ϊ����ָ�
	delay200us();
	lcd12864_write_command(0x30);
	delay200us();
	
	lcd12864_write_command(0x0c); //display control��ʾ����꣬��˸����������֪�����������
	delay200us();

	lcd12864_write_command(0x01); //����ģʽ DDRAM����ָ������
	delay15ms();

	lcd12864_write_command(0x06); //����ģʽ����
	//===��ʼ������------
}

//��ָ����λ����ʾָ��������
void lcd12864_display_pos(unsigned char x,unsigned char y){	 //ע��128��������е���Ŀ��64��������е���Ŀ����ˣ�x��������С�y���С�����仰û�д�
	//��Ժ��֣�������Ļ���ֳ����У�������8.����ַ���������4��������16��������ȥ��ͼ��
	unsigned char displaystart = 0;
	if(y==0)displaystart=0x80+x;
	if(y==1)displaystart=0x90+x;
	if(y==2)displaystart=0x88+x;
	if(y==3)displaystart=0x98+x;
	lcd12864_write_command(displaystart);//����дָ���ȷ�����Ŀ�ʼ��ʾ������Ҫ������	
}
//��ʾ������ַ�
void lcd12864_show_str(unsigned char x,unsigned char y,char mystr){
	lcd12864_display_pos(x,y);
	lcd12864_write_data(mystr);	
}
//��ʾ����ĺ���
void lcd12864_show_string(unsigned char x,unsigned char y,char mystr[]){
	lcd12864_display_pos(x,y);
	while(*mystr!='\0'){
		lcd12864_write_data(*mystr++);
	}
		
}

//���Ի�ͼ


















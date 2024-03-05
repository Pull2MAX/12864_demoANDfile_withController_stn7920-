//***************************************************************************
//连线表:  CPU=89C52                                                        *
//RS=P3.0            RW=P3.1             E=P3.2                             *
//FOSC=12MHz         D0-D7=P1.0-P1.7                                        *
//***************************************************************************
#include <reg52.h>

sbit RS = P3^0;
sbit RW = P3^1;
sbit E  = P3^2;
sbit bf = P1^7;
#define data_bus P1
#define com  0
#define dat  1

void wr_lcd(unsigned char dat_com,unsigned char content);
void ck_busy(void);
void delay_us(unsigned int us_value);
void delay_ms(unsigned int ms_value);
void init_lcd (void);
void cn_disp(unsigned char code *cn);
void txt_disp(unsigned char code *cn,unsigned char adr);
void gra_disp(unsigned char code *img);
void lat_disp(unsigned char data1,unsigned char data2);
void con_disp(unsigned char data1,unsigned char x0,unsigned char y0,unsigned char xl,unsigned char yl);
void lcd_txt_clr(void);
void lcd_gra_clr(void);

unsigned char code tab2[]={
"128X64全点阵显示"      //第一行汉字内容
"可显示8X4 个中文"      //第二行汉字内容
"内带标准中文字库"      //第三行汉字内容
"含8192个中文字型"      //第四行汉字内容
};      

unsigned char code tab1[]={
"欢迎选购我们产品"
"限公司液晶显示模"       //第一行汉字内容
"无锡斯迈诺科技有"      //第二行汉字内容
//"限公司液晶显示模"      //第三行汉字内容
"块采用ST7920控制"      //第四行汉字内容

};      
unsigned char code str1[]="无锡斯迈诺有限公司";
unsigned char code str2[]="欢迎选购我们产品";
unsigned char code str3[]=" www.yab-lcm.com  ";
unsigned char code gra1[];

/******************************************************************************************
*函数名称： init_lcd
*功能描述： 初始化
******************************************************************************************/
void init_lcd (void)
{
 wr_lcd(com,0x30);  /*30---功能设定,8-bit模式*/   
 wr_lcd(com,0x30);  /*30---功能设定,基本指令*/  
 wr_lcd(com,0x01);  /*清屏，地址指针指向00H*/
 wr_lcd(com,0x06);  /*光标的移动方向*/
 wr_lcd(com,0x0c);  /*开显示，关游标*/
}

/******************************************************************************************
*函数名称： cn_disp
*功能描述： 显示全屏汉字或字符
******************************************************************************************/
void cn_disp(unsigned char code *cn)
{
 unsigned char i,j;
 wr_lcd(com,0x30);
 wr_lcd(com,0x80);
 for(j=0;j<4;j++)
  {
   for(i=0;i<16;i++)
   wr_lcd(dat,cn[j*16+i]);
  }
 delay_ms(800);
}

/******************************************************************************************
*函数名称： txt_disp
*功能描述： 显示一行汉字或字符
******************************************************************************************/
void txt_disp(unsigned char code *cn,unsigned char adr)
{
 unsigned char i,j;
 wr_lcd(com,0x30);
 wr_lcd(com,adr);   //第一到四行地址分别是0x80,0x90,0x88,0x98
 j=0;
 for(i=0;i<16;i++)
  wr_lcd(dat,cn[j*16+i]);
}

/******************************************************************************************
*函数名称： img_disp
*功能描述： 显示图形
******************************************************************************************/
void gra_disp(unsigned char code *img)
{
 unsigned char i,j;
 for(j=0;j<32;j++)
  {
   for(i=0;i<8;i++)
    {
     wr_lcd(com,0x34);
     wr_lcd(com,0x80+j);
     wr_lcd(com,0x80+i);
     wr_lcd(com,0x30);
     wr_lcd(dat,img[j*16+i*2]);
     wr_lcd(dat,img[j*16+i*2+1]);
    }
  }
  for(j=32;j<64;j++)
   {
    for(i=0;i<8;i++)
     {
      wr_lcd(com,0x34);
      wr_lcd(com,0x80+j-32);
      wr_lcd(com,0x88+i);
      wr_lcd(com,0x30);
      wr_lcd(dat,img[j*16+i*2]);
      wr_lcd(dat,img[j*16+i*2+1]);
     }
  	}
 wr_lcd (com,0x36);   //图形模式开
 delay_ms(2600);
 wr_lcd (com,0x34);   //图形模式关
}

/******************************************************************************************
*函数名称： lat_disp
*功能描述： 显示网点
******************************************************************************************/
void lat_disp(unsigned char data1,unsigned char data2)
{
 unsigned char i,j;
 for(j=0;j<16;j++)
  {
   for(i=0;i<16;i++)
    {
     wr_lcd(com,0x34);
     wr_lcd(com,0x80+j*2);
     wr_lcd(com,0x80+i);
     wr_lcd(com,0x30);
     wr_lcd(dat,data1);
     wr_lcd(dat,data1);
    }
   for(i=0;i<16;i++)
    {
     wr_lcd(com,0x34);
     wr_lcd(com,0x80+j*2+1);
     wr_lcd(com,0x80+i);
     wr_lcd(com,0x30);
     wr_lcd(dat,data2);
     wr_lcd(dat,data2);
    }
  }
 wr_lcd(com,0x36);
 delay_ms(2600);
 wr_lcd (com,0x34);   //图形模式关
}

/******************************************************************************************
*函数名称： con_disp
*功能描述： 当data=0xff时,在x0,y0处显示长为16xl宽为yl的方块
******************************************************************************************/
void con_disp(unsigned char data1,unsigned char x0,unsigned char y0,unsigned char xl,unsigned char yl)
{
 unsigned char i,j;
 for(j=0;j<yl;j++)
  {
   for(i=0;i<xl;i++)
    {
     wr_lcd(com,0x34);
     wr_lcd(com,y0+j);
     wr_lcd(com,x0+i);
     wr_lcd(com,0x30);
     wr_lcd(dat,data1);
     wr_lcd(dat,data1);
    }
  }
 wr_lcd(com,0x36);
 delay_ms(800);
 wr_lcd (com,0x34);   //图形模式关
}

/******************************************************************************************
*函数名称：wr_lcd
*功能描述：写入指令/数据子程序 
******************************************************************************************/
void wr_lcd(unsigned char dat_com,unsigned char content)
{
 ck_busy ();
 if(dat_com)
  {
   RS = 1;   //data
   RW = 0;   //write
  }
 else 
  {
   RS = 0;   //command
   RW = 0;   //write
  }
 P1=content;   //iutput data or com
 E = 1;
 ;
 E = 0;
}

/******************************************************************************************
*函数名称：ck_busy
*功能描述：查忙子程序 
******************************************************************************************/
void ck_busy(void)
{
  P1 = 0xff;
  RS = 0;
  RW = 1;
  E = 1;
  while(bf==1);
  E = 0;
}

/******************************************************************************************
*函数名称：delay_us
*功能描述：延时us 
******************************************************************************************/
void delay_us(unsigned int us_value)   //delay time
{
 while(us_value--);
}

/******************************************************************************************
*函数名称：delay_ms
*功能描述：延时ms
******************************************************************************************/
void delay_ms(unsigned int ms_value)
{
 unsigned int i,j;
 for(i=0;i<ms_value;i++)
  for(j=0;j<15;j++)
   delay_us(1);
}

/******************************************************************************************
*函数名称： lcd_txt_clr
*功能描述： 文本区清RAM函数
******************************************************************************************/
void lcd_txt_clr(void)
{
 wr_lcd(com,0x30);
 wr_lcd(com,0x01);
}

/******************************************************************************************
*函数名称： lcd_gra_clr
*功能描述： 图形区清RAM函数
******************************************************************************************/
void lcd_gra_clr(void)
{
 unsigned char i,j;
 wr_lcd(com,0x34);      //8Bit扩充指令集,即使是36H也要写两次
 wr_lcd(com,0x36);      //绘图ON,基本指令集里面36H不能开绘图
 for(j=0;j<32;j++)
  {
   for(i=0;i<16;i++)
    {
     wr_lcd(com,0x34);
     wr_lcd(com,0x80+j);
     wr_lcd(com,0x80+i);
     wr_lcd(com,0x30);
     wr_lcd(dat,0);
     wr_lcd(dat,0);
    }
  }
}

/******************************************************************************************
*函数名称： main
*功能描述： 主程序
******************************************************************************************/
void main()
{
 delay_ms(40);
 init_lcd();
 while(1)
  {
   cn_disp(tab1);            //欢迎页面
   delay_ms(2600);
   cn_disp(tab2);
   delay_ms(2600);
   lcd_txt_clr();
   lat_disp(0xff,0xff);      //显示全屏
   //con_disp(0xff,0x8c,0x80,2,16);
   lat_disp(0xaa,0x55);      //显示网点
   lat_disp(0x55,0xaa);      //错开显示网点     
   lat_disp(0xaa,0xaa);      //显示竖条
   lat_disp(0x55,0x55);      //错开显示竖条
   lat_disp(0xff,0x00);      //显示横条
   lat_disp(0x00,0xff);      //错开显示横条
   txt_disp(str1,0x80);
   txt_disp(str2,0x90);   
   txt_disp(str3,0x88);
   lcd_txt_clr();
   gra_disp(gra1);
  }
}

unsigned char code gra1[]={	  //横向取模，无字节倒序
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0X80,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X01,
0X80,0X00,0X00,0X00,0X07,0XFF,0XE0,0X0F,0XFF,0XC0,0X00,0X00,0X00,0X00,0X00,0X01,
0X80,0X00,0X00,0X00,0X03,0XFF,0XE0,0X0F,0XFF,0X80,0X00,0X00,0X00,0X00,0X00,0X01,
0X80,0X00,0X00,0X00,0X00,0X7F,0X00,0X00,0XF8,0X00,0X00,0X00,0X00,0X00,0X00,0X01,
0X80,0X00,0X00,0X00,0X00,0X3F,0X00,0X00,0XF0,0X00,0X00,0X00,0X00,0X00,0X00,0X01,
0X80,0X00,0X00,0X00,0X00,0X1F,0X00,0X00,0XE0,0X00,0X00,0X00,0X00,0X00,0X00,0X01,
0X80,0X00,0X00,0X00,0X00,0X1F,0X80,0X01,0XE0,0X00,0X00,0X00,0X00,0X00,0X00,0X01,
0X80,0X00,0X00,0X00,0X00,0X0F,0X80,0X01,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X01,
0X80,0X00,0X00,0X00,0X00,0X07,0XC0,0X03,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X01,
0X80,0X00,0X00,0X00,0X00,0X07,0XC0,0X03,0X80,0X00,0X00,0X00,0X00,0X00,0X00,0X01,
0X80,0X00,0X00,0X00,0X00,0X03,0XE0,0X07,0X80,0X00,0X00,0X00,0X00,0X00,0X00,0X01,
0X80,0X00,0X00,0X00,0X00,0X03,0XE0,0X07,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X01,
0X80,0X00,0X00,0X00,0X00,0X01,0XF0,0X0F,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X01,
0X80,0X00,0X00,0X00,0X00,0X01,0XF0,0X0E,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X01,
0X80,0X00,0X00,0X00,0X00,0X00,0XF8,0X1E,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X01,
0X80,0X00,0X00,0X00,0X00,0X00,0XF8,0X1C,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X01,
0X80,0X00,0X00,0X00,0X00,0X00,0X7C,0X3C,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X01,
0X80,0X00,0X00,0X00,0X00,0X00,0X3C,0X39,0XF7,0XFF,0X80,0X00,0X00,0X00,0X00,0X01,
0X80,0X00,0X00,0X00,0X00,0X00,0X3E,0X79,0XFF,0XFF,0XF8,0X00,0X00,0X00,0X00,0X01,
0X80,0X00,0X00,0X00,0X00,0X00,0X1E,0X70,0X3C,0X00,0X7C,0X00,0X00,0X00,0X00,0X01,
0X80,0X00,0X00,0X00,0X00,0X00,0X1F,0XF0,0X3C,0X00,0X1E,0X00,0X00,0X00,0X00,0X01,

};
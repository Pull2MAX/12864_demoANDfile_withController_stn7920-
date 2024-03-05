//***************************************************************************
//���߱�:  CPU=89C52                                                        *
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
"128X64ȫ������ʾ"      //��һ�к�������
"����ʾ8X4 ������"      //�ڶ��к�������
"�ڴ���׼�����ֿ�"      //�����к�������
"��8192����������"      //�����к�������
};      

unsigned char code tab1[]={
"��ӭѡ�����ǲ�Ʒ"
"�޹�˾Һ����ʾģ"       //��һ�к�������
"����˹��ŵ�Ƽ���"      //�ڶ��к�������
//"�޹�˾Һ����ʾģ"      //�����к�������
"�����ST7920����"      //�����к�������

};      
unsigned char code str1[]="����˹��ŵ���޹�˾";
unsigned char code str2[]="��ӭѡ�����ǲ�Ʒ";
unsigned char code str3[]=" www.yab-lcm.com  ";
unsigned char code gra1[];

/******************************************************************************************
*�������ƣ� init_lcd
*���������� ��ʼ��
******************************************************************************************/
void init_lcd (void)
{
 wr_lcd(com,0x30);  /*30---�����趨,8-bitģʽ*/   
 wr_lcd(com,0x30);  /*30---�����趨,����ָ��*/  
 wr_lcd(com,0x01);  /*��������ַָ��ָ��00H*/
 wr_lcd(com,0x06);  /*�����ƶ�����*/
 wr_lcd(com,0x0c);  /*����ʾ�����α�*/
}

/******************************************************************************************
*�������ƣ� cn_disp
*���������� ��ʾȫ�����ֻ��ַ�
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
*�������ƣ� txt_disp
*���������� ��ʾһ�к��ֻ��ַ�
******************************************************************************************/
void txt_disp(unsigned char code *cn,unsigned char adr)
{
 unsigned char i,j;
 wr_lcd(com,0x30);
 wr_lcd(com,adr);   //��һ�����е�ַ�ֱ���0x80,0x90,0x88,0x98
 j=0;
 for(i=0;i<16;i++)
  wr_lcd(dat,cn[j*16+i]);
}

/******************************************************************************************
*�������ƣ� img_disp
*���������� ��ʾͼ��
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
 wr_lcd (com,0x36);   //ͼ��ģʽ��
 delay_ms(2600);
 wr_lcd (com,0x34);   //ͼ��ģʽ��
}

/******************************************************************************************
*�������ƣ� lat_disp
*���������� ��ʾ����
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
 wr_lcd (com,0x34);   //ͼ��ģʽ��
}

/******************************************************************************************
*�������ƣ� con_disp
*���������� ��data=0xffʱ,��x0,y0����ʾ��Ϊ16xl��Ϊyl�ķ���
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
 wr_lcd (com,0x34);   //ͼ��ģʽ��
}

/******************************************************************************************
*�������ƣ�wr_lcd
*����������д��ָ��/�����ӳ��� 
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
*�������ƣ�ck_busy
*������������æ�ӳ��� 
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
*�������ƣ�delay_us
*������������ʱus 
******************************************************************************************/
void delay_us(unsigned int us_value)   //delay time
{
 while(us_value--);
}

/******************************************************************************************
*�������ƣ�delay_ms
*������������ʱms
******************************************************************************************/
void delay_ms(unsigned int ms_value)
{
 unsigned int i,j;
 for(i=0;i<ms_value;i++)
  for(j=0;j<15;j++)
   delay_us(1);
}

/******************************************************************************************
*�������ƣ� lcd_txt_clr
*���������� �ı�����RAM����
******************************************************************************************/
void lcd_txt_clr(void)
{
 wr_lcd(com,0x30);
 wr_lcd(com,0x01);
}

/******************************************************************************************
*�������ƣ� lcd_gra_clr
*���������� ͼ������RAM����
******************************************************************************************/
void lcd_gra_clr(void)
{
 unsigned char i,j;
 wr_lcd(com,0x34);      //8Bit����ָ�,��ʹ��36HҲҪд����
 wr_lcd(com,0x36);      //��ͼON,����ָ�����36H���ܿ���ͼ
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
*�������ƣ� main
*���������� ������
******************************************************************************************/
void main()
{
 delay_ms(40);
 init_lcd();
 while(1)
  {
   cn_disp(tab1);            //��ӭҳ��
   delay_ms(2600);
   cn_disp(tab2);
   delay_ms(2600);
   lcd_txt_clr();
   lat_disp(0xff,0xff);      //��ʾȫ��
   //con_disp(0xff,0x8c,0x80,2,16);
   lat_disp(0xaa,0x55);      //��ʾ����
   lat_disp(0x55,0xaa);      //����ʾ����     
   lat_disp(0xaa,0xaa);      //��ʾ����
   lat_disp(0x55,0x55);      //����ʾ����
   lat_disp(0xff,0x00);      //��ʾ����
   lat_disp(0x00,0xff);      //����ʾ����
   txt_disp(str1,0x80);
   txt_disp(str2,0x90);   
   txt_disp(str3,0x88);
   lcd_txt_clr();
   gra_disp(gra1);
  }
}

unsigned char code gra1[]={	  //����ȡģ�����ֽڵ���
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
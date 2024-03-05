#include "12864.h"
#include "delay.h"
/*
* Description  :д����
* Param       :None
* Return Code :None
*/
void Write_Com(char Cbyte)
{
    CS = 1;
    Chk_Busy();
    SendByte(0xf8);          //11111,RW(0),RS(0),0
    SendByte(0xf0&Cbyte);
    SendByte(0xf0&Cbyte<<4);
    CS = 0;
}
/*
* Description  :����Ļ���г�ʼ��
* Param       :None
* Return Code :None
*/
void Init(void)
{
    delay_ms(50);
	RES=0;
	delay_ms(50);
	RES=1;
	delay_ms(50);
	Write_Com(0x30);
	delay_ms(50);
	Write_Com(0x30);
	delay_ms(50);
	Write_Com(0x30);
	delay_ms(50);
	Write_Com(0x03);  //Entry mode set
	delay_ms(50);
	Write_Com(0x01);  //return Home
	delay_ms(50);
	Write_Com(0x06);
	delay_ms(50);
	Write_Com(0x0c); //Display on, 00001DCB, D=1:display on,0 off;
	delay_ms(50);
}
/*
* Description  :��������
* Param        :None
               :ʱ�ӵ���ʱ�Ǳ�Ҫ��
* Return Code :���յ�������
*/
unsigned char ReceiveByte(void)
{
    char i,temp1,temp2;
	temp1=0;
	temp2=0;
	for(i=0;i<8;i++)
	{
	    temp1=temp1<<1;
		SCK=0;
		delay_us(10);
		SCK=1;
		delay_us(10);
		SCK=0;
		delay_us(10);
		if(SID) temp1++;	
	}
	for(i=0;i<8;i++)
	{
	    temp2=temp2<<1;
		SCK=0;
		delay_us(10);
		SCK=1;
		delay_us(10);
		SCK=0;
		delay_us(10);
		if(SID) temp2++;	
	}
	return ((0xf0&temp1)+(0xf&temp2));
}
/*
* Description  :����һ������
* Param        :Ҫ���͵����� 
                ʱ�ӵ���ʱ�Ǳ�Ҫ��
* Return Code  :None
*/
void SendByte(char Dbyte)
{
    char i;
	for(i=0;i<8;i++)
	{
	    SCK=0;
		delay_us(10);
		if((Dbyte&0x80)==0x80) SID=1;
		else SID=0;
		Dbyte =Dbyte<<1;
		SCK=1;
		delay_us(10);
		SCK=0;
		delay_us(10);
	}
}
/*
* Description  :����ͼƬ
* Param        :*put ���ͼƬ���ݵĵ�ַ
* Return Code  :None
*/
void PutBMP(char *puts)
{
    unsigned int x=0;
    unsigned char i,j;
    Write_Com(0x34);
    Write_Com(0x36);
    for(i=0;i<32;i++)  //<32
    {
        Write_Com(0x80|i);
        Write_Com(0x80); 
        for(j=0;j<32;j++)    //<32
        {                     
             Write_Data(puts[x]);
             x++;
        }
    }
}
/*
* Description  :���ͼƬ��Ϣ
* Param        :None
* Return Code  :None
*/
void LcmClearBMP( void )
{
    unsigned char i,j;
    Write_Com(0x34);
    Write_Com(0x36);
    for(i=0;i<32;i++) //<32
    {
        Write_Com(0x80|i);
        Write_Com(0x80);
        for(j=0;j<32;j++) //<32
        Write_Data(0);
    }
}
/*
* Description  :���ͼƬ��Ϣ
* Param        :None
* Return Code  :None
*/
void LcmClearTXT( void )
{
    unsigned char i;
    Write_Com(0x30);
    Write_Com(0x80);
    for(i=0;i<64;i++)
    Write_Data(0x20);
}
/*
* Description  :
* Param        :None
* Return Code  :None
*/
void Font1(void)
{
	int i,j;
    Write_Com(0x80);  		
	delay_us(500);
	for(i=0X30;i<0X50;i++)
	{
	    Write_Data(i);
		delay_us(500);
	}

	Write_Com(0x90);
	delay_us(500);
	for(j=0x50;j<0x70;j++)
	{
	    Write_Data(j);
		delay_us(500);
	}
}
/*
* Description  :
* Param        :H L
* Return Code  :None
*/
void Font2(u8 h,u8 l)
{
	int i,j;
    Write_Com(0x80);  		//
	delay_us(500);
	for(i=0;i<16;i++)
	{
	    Write_Data(h);
		delay_us(500);
	    Write_Data(l++);
		delay_us(500);
	}
	Write_Com(0x90);
	delay_us(500);
	for(j=0;j<16;j++)
	{
	    Write_Data(h);
		delay_us(500);
	    Write_Data(l++);
		delay_us(500);
	}
}
/*
* Description  :��ʾ����
* Param        :K H
* Return Code  :None
*/
void Wr_Gdram(u8 k, u8 h)
{
    int i,j;
//	write_com(0x34);		//Set GDRAM
//	delay(1);//1
//	write_com(0x36);		//Set GDRAM
//	delay(1);//1
	for(j=0;j<64;j++)	//<64
   {
	  Write_Com(0X80|j);		//Set vertical address (Y) for GDRAM
//	  delay(10);
	  Write_Com(0x80);			//
//	  delay(10);
 	  for(i=0;i<16;i++)		//
      {
	    Write_Data(k);
//	    delay(10);			//Write D15~D8 to GDRAM (first byte)
	    Write_Data(h);
//	    delay(10);			//Write D7~D0 to GDRAM (second byte)
      }
    }	
  	Write_Com(0x34);
 	delay_us(500);//1
	Write_Com(0x36);
	delay_us(500);//1

}
/*
* Description  :
* Param        :None
* Return Code  :None
*/
void Wr_Gdram_H(void)
{
    int i,j,k;
 	//i=0X80;				//
 	k=0Xff;
	Write_Com(0x34);		//Set GDRAM
	delay_us(500);
	Write_Com(0x36);		//Set GDRAM
	delay_us(500);

	for(j=0;j<64;j++)	
   {
	for(i=0X80;i<0X90;i++)		//
   {
	Write_Com(0X80+j);		//Set vertical address (Y) for GDRAM
	delay_us(500);
	Write_Com(i);			//
	delay_us(500);
	Write_Data(k);			//Write D15~D8 to GDRAM (first byte)
	Write_Data(k);			//Write D7~D0 to GDRAM (second byte)
   }
   	k=~k;
   }	
   	Write_Com(0x34);
    delay_us(500);
	Write_Com(0x30);
    delay_us(500);
}
/*
* Description  :
* Param        :None
* Return Code  :None
*/
void Wr_Gdram_W(void)
{
    int i,j,k;
 	//i=0X80;				//
 	k=0X55;
	Write_Com(0x34);		//Set GDRAM
	delay_us(500);
	Write_Com(0x36);		//Set GDRAM
	delay_us(500);
	for(j=0;j<64;j++)	
   {
	 for(i=0X80;i<0X90;i++)		//
     {
	    Write_Com(0X80+j);		//Set vertical address (Y) for GDRAM
	    delay_us(500);
	    Write_Com(i);			//
	    delay_us(500);
	    Write_Data(k);			//Write D15~D8 to GDRAM (first byte)
	    Write_Data(k);			//Write D7~D0 to GDRAM (second byte)
     }
   	  k=~k;
   }	
   Write_Com(0x36);
   delay_ms(1600);
   Write_Com(0x34);   //ͼ��ģʽ��
}
/*
* Description  :��ʾ����
* Param        :data1 data2
* Return Code  :None
*/
void lat_disp(unsigned char data1,unsigned char data2)
{
    unsigned char i,j;
    for(j=0;j<16;j++)
    {
      for(i=0;i<16;i++)
      {
         Write_Com(0x34);
         Write_Com(0x80+j*2);
         Write_Com(0x80+i);
         Write_Com(0x30);
         Write_Data(data1);
         Write_Data(data1);
      }
      for(i=0;i<16;i++)
      {
         Write_Com(0x34);
         Write_Com(0x80+j*2+1);
         Write_Com(0x80+i);
         Write_Com(0x30);
         Write_Data(data2);
         Write_Data(data2);
     }
   }
    Write_Com(0x36);
    delay_ms(1700);
    Write_Com(0x34);   //ͼ��ģʽ��
}
/*
* Description  :��ʾ����
* Param        :*cn ��ʾ�����ĵ�ַ
* Return Code  :None
*/
void Cn_Disp(char *cn)
{
    unsigned char i,j;
    Write_Com(0x30);
    Write_Com(0x80);
    for(j=0;j<4;j++)
   {
      for(i=0;i<16;i++)
      Write_Data(cn[j*16+i]);
   }
   delay_ms(100);//200
}
/*
* Description  :д����
* Param        :Dbyte Ҫд������
* Return Code  :None
*/
void Write_Data(char Dbyte)
{
    CS=1;
	Chk_Busy ();
	SendByte (0xfa);
	SendByte (0xf0&Dbyte);
	SendByte (0xF0&Dbyte<<4);
	CS=0;
}
/*
* Description  :����Ƿ����
* Param        :Dbyte Ҫд������
* Return Code  :None
*/
void Chk_Busy(void)
{
    do
		SendByte(0xfc);
	while(0x80&ReceiveByte());
}
/*
* Description  :��ʾͼ��
* Param        :*img ͼ��ĵ�ַ
* Return Code  :None
*/
void Gra_disp(unsigned char  *img)
{
    unsigned char i,j;
    for(j=0;j<32;j++)
   {
     for(i=0;i<8;i++)
    {
      Write_Com(0x34);
      Write_Com(0x80+j);
      Write_Com(0x80+i);
      Write_Com(0x30);
      Write_Data(img[j*16+i*2]);
      Write_Data(img[j*16+i*2+1]);
    }
   }
   for(j=32;j<64;j++)
   {
    for(i=0;i<8;i++)
     {
      Write_Com(0x34);
      Write_Com(0x80+j-32);
      Write_Com(0x88+i);
      Write_Com(0x30);
      Write_Data(img[j*16+i*2]);
      Write_Data(img[j*16+i*2+1]);
     }
  	}
    Write_Com(0x36);   //ͼ��ģʽ��
    delay_ms(1200);
    Write_Com(0x34);   //ͼ��ģʽ��
}
/*
* Description  :��Ļ�ܽų�ʼ��
* Param        :*img ͼ��ĵ�ַ
* Return Code  :None
*/
void Pin_Init(void)
{
    RCC->AHB1ENR|=1<<2;//ʹ��PORTCʱ�� 
	RCC->AHB1ENR|=1<<0;//ʹ��PORTCʱ�� 
	GPIO_Set(GPIOC,PIN8|PIN9|PIN10|PIN11,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU); //PB8,PB9,PB10,PB11����
	GPIO_Set(GPIOA,PIN4,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU); //PB8,PB9,PB10,PB11����
    CS=0;  //����״̬
	SCK=0; //ʱ�ӿ���
	SID=0; //���ݿ�
	RES=1; //��λ��������
	PSB=0; //ѡ�񴮿ڣ�����Ҳ����
}



#include "sys.h"
#include "delay.h" 
#include "led.h" 
#include "usart.h"
#include "12864.h"
//ALIENTEK ������STM32F429������ ʵ��3
//����ͨ��ʵ��  
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾
char tab1[]={
"��ӭѡ�����ǲ�Ʒ"
"�޹�˾Һ����ʾģ"     
"����˹��ŵ�Ƽ���"      
"�����ST7920����"     
};
char tab2[]={
"128X64ȫ������ʾ"      //��һ�к�������
"����ʾ8X4 ������"      //�ڶ��к�������
"�ڴ���׼�����ֿ�"      //�����к�������
"��8192����������"      //�����к�������
};
 unsigned char  bmp1[]={   //����ȡģ�����ֽڵ���
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
0X80,0X00,0X00,0X00,0X00,0X00,0X0F,0XE0,0X3C,0X00,0X1F,0X00,0X00,0X00,0X00,0X01,
0X80,0X00,0X00,0X00,0X00,0X00,0X0F,0XE0,0X3C,0X00,0X1F,0X00,0X00,0X00,0X00,0X01,
0X80,0X00,0X00,0X00,0X00,0X00,0X07,0XE0,0X3C,0X00,0X1E,0X00,0X00,0X00,0X00,0X01,
0X80,0X00,0X00,0X00,0X00,0X00,0X07,0XC0,0X3C,0X00,0X3E,0X00,0X00,0X00,0X00,0X01,
0X80,0X00,0X00,0X00,0X00,0X00,0X03,0X80,0X3C,0X00,0X78,0X00,0X00,0X00,0X00,0X01,
0X80,0X00,0X00,0X00,0X00,0X00,0X03,0X80,0X3E,0X1F,0XE0,0X00,0XE0,0X00,0X00,0X01,
0X80,0X00,0X00,0X00,0X02,0X00,0X07,0X00,0X3F,0XFF,0XE0,0X03,0XE0,0X00,0X00,0X01,


};

int main(void)
{ 
//	u8 t;
//	u8 len;	
//	u16 times=0;   
	Stm32_Clock_Init(360,25,2,8);//����ʱ��,180Mhz
	delay_init(180);		//��ʼ����ʱ���� 
	uart_init(90,115200);	//���ڳ�ʼ��Ϊ115200
	LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ� 
    Pin_Init ();	
	Init ();
	delay_ms(10);	
	while(1)
	{
//		if(USART_RX_STA&0x8000)
//		{					   
//			len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
//			printf("\r\n�����͵���ϢΪ:\r\n");
//			for(t=0;t<len;t++)
//			{
//				USART1->DR=USART_RX_BUF[t];
//				while((USART1->SR&0X40)==0);//�ȴ����ͽ���
//			}
//			printf("\r\n\r\n");//���뻻��
//			USART_RX_STA=0;
//		}else
//		{
//			times++;
//			if(times%5000==0)
//			{
//				printf("\r\nALIENTEK ������STM32F4/F7������ ����ʵ��\r\n");
//				printf("����ԭ��@ALIENTEK\r\n\r\n\r\n");
//			}
//			if(times%200==0)printf("����������,�Իس�������\r\n");  
//			if(times%30==0)LED0=!LED0;//��˸LED,��ʾϵͳ��������.
//			delay_ms(10);   
//		}
    Cn_Disp (tab1);
	delay_ms(1000);
	delay_ms(1000);
//	delay_ms(1000);
//	delay_ms(1000);
	Cn_Disp (tab2);
	delay_ms(1000);
	delay_ms(1000);
//	delay_ms(1000);
//	delay_ms(1000);
	LcmClearTXT ();
	lat_disp (0xff,0xff);    //��ʾȫ��
	lat_disp (0xaa,0x55);    //��ʾ����
	lat_disp (0x55,0xaa);    //����ʾ����
	lat_disp (0xaa,0xaa);    //��ʾ����
	lat_disp (0x55,0x55);    //����ʾ����
    lat_disp (0xff,0x00);    //��ʾ����
    lat_disp (0x00,0xff);	 //����ʾ����
	LcmClearTXT ();
	Gra_disp(bmp1);
    delay_ms(500);
    LcmClearBMP();
    LcmClearTXT();
	LED0 =~LED0;
	}
}


















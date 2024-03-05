#ifndef __12864_H
#define __12864_H
#include "sys.h"
#define RES  PCout(8)
#define CS   PCout(9)
#define SCK  PCout(10)
#define SID  PCout(11)
#define PSB  PAout(4)
//定义一些函数
void Init (void);
unsigned char ReceiveByte(void);
void SendByte(char Dbyte);
void LcmClearBMP( void );
void PutBMP(char *puts);
void LcmClearTXT( void );
void Font1(void);
void Font2(u8 h,u8 l);
void Wr_Gdram_H(void);
void Wr_Gdram(u8 k, u8 h);
void lat_disp(unsigned char data1,unsigned char data2);
void Wr_Gdram_W(void);
void Cn_Disp(char *cn);
void Chk_Busy(void);
void Write_Com(char Cbyte);
void Write_Data( char Dbyte );
void Gra_disp(unsigned char  *img);
void Pin_Init(void);
#endif




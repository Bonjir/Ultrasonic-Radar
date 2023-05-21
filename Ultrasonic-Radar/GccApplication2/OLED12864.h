/************************************************************************************
*
* File name: OLED12864.h
* Project  : OLED
* Processor: Atmega8A
* Compiler : Atmel Studio
*
* Author : Bonjir
* Version: 1.00
* Date   : 2023.4.21
*
* Description: 128*64����OLEDģ�������ļ�
*
* Others: none;
*
* Function List:
*
* 1. void OLED_Init(void) -- OLED����ʼ�����򣬴˺���Ӧ�ڲ�����Ļ֮ǰ���ȵ���
* 2. void OLED_WrDat(unsigned char IIC_Data) -- ��OLED��д����
* 3. void OLED_WrCmd(unsigned char cmd) -- ��OLED��д����
* 4. void OLED_Set_Pos(unsigned char col, unsigned char page) -- ������ʾ����
* 5. void OLED_Fill(unsigned char bmp_dat) -- ȫ����ʾ(��ʾBMPͼƬʱ�Ż��õ��˹���)
* 6. void OLED_Clear(void) -- ��λ/����
* 7. void OLED_P6x8Str(unsigned char col, page,unsigned char ch[]) -- 6x8������������ʾASCII�����С���У���̫����
* 8. void OLED_P8x16Str(unsigned char col, page,unsigned char ch[]) -- 8x16������������ʾASCII�룬�ǳ�����
* 9. void OLED_P16x16Ch(unsigned char col, page, N) -- 16x16������������ʾ���ֵ���С���У������ø������塢�Ӵ֡���б���»��ߵ�
* 10. void OLED_Draw_BMP(unsigned char x0, y0,x1, y1,unsigned char BMP[]) -- ��128x64���ص�BMPλͼ��ȡ�����������ֱ�Ȼ���Ƶ�codetab�У��˺������ü���
*
*
* History: none;
*
*************************************************************************************/

#ifndef OLED12864_H_
#define OLED12864_H_

#define F_CPU 16000000UL

#include <util/delay.h>
#include "twi_fun.h"
#include "codetab.h"

#define	Brightness	0xCF
#define X_WIDTH 	128
#define Y_WIDTH 	64


void OLED_WrDat(unsigned char IIC_Data);
void OLED_WrCmd(unsigned char IIC_Command);
void OLED_Set_Pos(unsigned char col, unsigned char page);
void OLED_Fill(unsigned char bmp_dat);
void OLED_Clear(void);
void OLED_DeepClear(void);
void OLED_Init(void);

void OLED_P6x8Ch(unsigned char col, unsigned char page,char ch);
void OLED_P6x8Str(unsigned char col, unsigned char page,char ch[]);


/*
#if P8X16CHAR

void OLED_P8x16Ch(unsigned char col, unsigned char page, char ch);
void OLED_P8x16Str(unsigned char col, unsigned char page, char ch[]);

#endif*/



#endif
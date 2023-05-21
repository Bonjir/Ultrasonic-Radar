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
* Description: 128*64点整OLED模块驱动文件
*
* Others: none;
*
* Function List:
*
* 1. void OLED_Init(void) -- OLED屏初始化程序，此函数应在操作屏幕之前最先调用
* 2. void OLED_WrDat(unsigned char IIC_Data) -- 向OLED屏写数据
* 3. void OLED_WrCmd(unsigned char cmd) -- 向OLED屏写命令
* 4. void OLED_Set_Pos(unsigned char col, unsigned char page) -- 设置显示坐标
* 5. void OLED_Fill(unsigned char bmp_dat) -- 全屏显示(显示BMP图片时才会用到此功能)
* 6. void OLED_Clear(void) -- 复位/清屏
* 7. void OLED_P6x8Str(unsigned char col, page,unsigned char ch[]) -- 6x8点整，用于显示ASCII码的最小阵列，不太清晰
* 8. void OLED_P8x16Str(unsigned char col, page,unsigned char ch[]) -- 8x16点整，用于显示ASCII码，非常清晰
* 9. void OLED_P16x16Ch(unsigned char col, page, N) -- 16x16点整，用于显示汉字的最小阵列，可设置各种字体、加粗、倾斜、下划线等
* 10. void OLED_Draw_BMP(unsigned char x0, y0,x1, y1,unsigned char BMP[]) -- 将128x64像素的BMP位图在取字软件中算出字表，然后复制到codetab中，此函数调用即可
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
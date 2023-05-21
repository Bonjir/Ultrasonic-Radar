/*
 * OLED_Paint.h
 *
 * Created: 2023/4/21 23:15:25
 *  Author: Bonjir
 */ 

#ifndef OLED_PAINT_H_
#define OLED_PAINT_H_

#include "OLED12864.h"

unsigned char GetScreenPixel(unsigned char x, unsigned char y);
void SetScreenPixel(unsigned char x, unsigned char y, unsigned char b);
void OP_Init();
void OP_Clear();
void OP_Update();
void OP_PutPixel(unsigned char x, unsigned char y, unsigned char b);
void OP_Line(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned char b);
void OP_Triangle(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned char x3, unsigned char y3, unsigned char b, unsigned char bSolid);
void OP_Rect(unsigned char top, unsigned char bottom, unsigned char left, unsigned char right, unsigned char b, unsigned char bSolid);
void OP_ErrorNotice(unsigned char ucErrc);

#define CONST

#define SCR_WX 64
#define PAGE_MAX 4
#define PIXELPERPAGE 8 CONST
#define SCR_WY (PAGE_MAX * PIXELPERPAGE)

#define OPE_SUCCESS 0
#define OPE_PAINTOUTOFCANVAS 1
#define OPE_READOUTOFCANVAS 2


#endif /* OLED_PAINT_H_ */
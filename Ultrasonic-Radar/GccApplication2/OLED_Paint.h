/*
 * OLED_Paint.h
 *
 * Created: 2023/4/21 23:15:25
 * Author: Bonjir
 * 
 * Version: 1.1
 * Modification: 
 * * add double buffer
 * * make the oled display in big pixels
 * * adjust the x offset of oled
 *
 * Todo:
 * * b = 2  reverse color display
 * * static 
 * * GetScreenPixel GetCanvasPixel
 * * Circle
 * * Triangle improve
 * 
 */ 

#ifndef OLED_PAINT_H_
#define OLED_PAINT_H_

#include <stdlib.h>
#include <string.h>
#include "OLED12864.h"
#include "quickmath.h"

unsigned char GetScreenPixel(unsigned char x, unsigned char y);
void SetScreenPixel(unsigned char x, unsigned char y, unsigned char b);
void OP_Init();
void OP_Clear();
void OP_Update();
void OP_ErrorNotice(unsigned char ucErrc);
void OP_ErrorNotice(unsigned char ucErrc, uint8_t err_x, uint8_t err_y);

void OP_PutPixel(unsigned char x, unsigned char y, unsigned char b);
void OP_Line(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned char b);
void OP_Triangle(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned char x3, unsigned char y3, unsigned char b, unsigned char bSolid);
void OP_Rect(unsigned char top, unsigned char bottom, unsigned char left, unsigned char right, unsigned char b, unsigned char bSolid);
void OP_Circle(unsigned char x, unsigned char y, unsigned char r, unsigned char b, unsigned char bSolid);
void OP_TextOut(unsigned char x, unsigned char y, const char str[], unsigned char len, unsigned char b);

#ifndef __CONST__
#define __CONST__
#endif
#ifndef __VARIABLE__
#define __VARIABLE__
#endif
#ifndef __SWITCH__
#define __SWITCH__
#endif

#define SCR_WX 64 __CONST__
#define PAGE_MAX 4 __CONST__
#define PIXELPERPAGE 8 __CONST__
#define SCR_WY (PAGE_MAX * PIXELPERPAGE)
#define _OLED_X_OFFSET_ 2 __CONST__
//#define NODOUBLEBUFFER __SWITCH__ /* turn off double paint buffer to save memory */
//#define USE_V2_CODETAB __SWITCH__ /* use code table saved in flash */
#define NOERRORNOTICE __SWITCH__ /* turn off error notice */

/* The values for the parameter of OP_ErrorNotice */
#define OPE_SUCCESS 0
#define OPE_PAINTOUTOFCANVAS 1 /* notice when paint out of canvas */
#define OPE_READOUTOFCANVAS 2 /* notice when read out of canvas */


#endif /* OLED_PAINT_H_ */

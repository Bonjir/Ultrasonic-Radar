/*
 * OLED_Paint.c
 *
 * Created: 2023/4/22 21:28:15
 *  Author: Bonjir
 */ 

#include "OLED_Paint.h"

#ifdef USE_V2_CODETAB
#include "codetab_v2.0.h"
#endif

unsigned char canvas[SCR_WX][PAGE_MAX];
#ifndef NODOUBLEBUFFER
unsigned char screen[SCR_WX][PAGE_MAX];
#endif

#ifndef max
#define max(a, b) ((a) > (b) ? (a) : (b))
#endif
#ifndef abs
#define abs(a) ((a) > 0 ? (a) : -(a))
#endif

unsigned char GetCanvasPixel(unsigned char x, unsigned char y)
{
	if(x >= SCR_WX || y >= SCR_WY) 
	{
		#ifndef NOERRORNOTICE
		OP_ErrorNotice(OPE_READOUTOFCANVAS, x, y);
		#endif
		return -1;
	}
	return ((canvas[x][y / PIXELPERPAGE] >> (y % PIXELPERPAGE)) & 1);
}

void SetCanvasPixel(unsigned char x, unsigned char y, unsigned char b)
{
	if(x >= SCR_WX || y >= SCR_WY)
	{
		#ifndef NOERRORNOTICE
		OP_ErrorNotice(OPE_PAINTOUTOFCANVAS, x, y);
		#endif
		return;
	}
	if (b == 0)
		(canvas[x][y / PIXELPERPAGE] &= ~(1 << (y % PIXELPERPAGE)));
	else 
		(canvas[x][y / PIXELPERPAGE] |= (1 << (y % PIXELPERPAGE)));
}

void OP_Init()
{
	OLED_Init();
	OP_Clear();
	
#ifndef NODOUBLEBUFFER
	for(unsigned char x = 0; x < SCR_WX; x ++)
		for(unsigned page = 0; page < PAGE_MAX; page ++)
			screen[x][page] = 0;
#endif
}

void OP_Clear()
{
	for(unsigned char x = 0; x < SCR_WX; x ++)
	for(unsigned page = 0; page < PAGE_MAX; page ++)
		canvas[x][page] = 0;
}

#ifndef NODOUBLEBUFFER

void _OP_WrPxlDat(unsigned char x_bigpxl, unsigned char p_bigpxl, unsigned char bLowerPage) // bLowerPage : 0: abs_page_1; 1 : abs_page_2
{
	unsigned char small_pixel_dat = 0;
	for(unsigned char digit = 0; digit < 4; digit ++)
	{
		small_pixel_dat |= ( ( (canvas[x_bigpxl][p_bigpxl] >> (bLowerPage == 0 ? digit : digit + 4)) & 1 ) << (digit * 2));
		small_pixel_dat |= ( ( (canvas[x_bigpxl][p_bigpxl] >> (bLowerPage == 0 ? digit : digit + 4)) & 1 ) << (digit * 2 + 1));
	}
	OLED_WrDat(small_pixel_dat);
	OLED_WrDat(small_pixel_dat);
}

void _OP_UpdatePageAccordToDiff(unsigned char screen_canvas_diff[], unsigned char p, unsigned char bLowerPage)
{
	
#define SETPIXEL_SPEED_FASTER_THAN_SETPOS_RATIO 2 /* T_wrdat = 2, T_setpos = 3, 1 + (1 / T_wrdat) / (1 / T_setpos) = 2.5 */

	for(unsigned short x = 0; x < SCR_WX - 1; x ++) // abs_page1 // from x = 0 to x = SCR_WX - 1 because cannot setpos(128, p)
	{
		if(screen_canvas_diff[x] == 0)
		continue;
		
		
		OLED_Set_Pos(2 * x + _OLED_X_OFFSET_, 2 * p + bLowerPage);
		_OP_WrPxlDat(x, p, bLowerPage);
		
		while (screen_canvas_diff[x] <= SETPIXEL_SPEED_FASTER_THAN_SETPOS_RATIO) // next different pixel not too far
		{
			for (unsigned char no_diff_pxl = x + 1; no_diff_pxl <= x + screen_canvas_diff[x]; no_diff_pxl ++)
			{
				if(no_diff_pxl == SCR_WX - 1)
					break;
				
				_OP_WrPxlDat(no_diff_pxl, p, bLowerPage);
				
			}
			x = x + screen_canvas_diff[x];
		}
	}
	
	if(screen_canvas_diff[SCR_WX - 1])
	{
		if(screen_canvas_diff[SCR_WX - 2] == 0)
		{
			OLED_Set_Pos(2 * (SCR_WX - 2) + _OLED_X_OFFSET_,  2 * p + bLowerPage);
			_OP_WrPxlDat(SCR_WX - 2, p, bLowerPage);
		}
		_OP_WrPxlDat(SCR_WX - 1, p, bLowerPage);
	}
}

void OP_DoubleBufferUpdatePage(unsigned char p)
{
	unsigned char screen_canvas_diff[SCR_WX] = { 0 }; // 真假代表是否scr与can不同，对于真值，数值大小代表下一个dif_pxl 距离此pxl的距离
	for(char x = 0, last_diff_pxl = -1; x < SCR_WX; x ++)
	{
		if(canvas[(unsigned char)x][p] != screen[(unsigned char)x][p])
		{
			screen_canvas_diff[(unsigned char) x] = 255;
			
			if(last_diff_pxl >= 0)
			{
				screen_canvas_diff[(unsigned char) last_diff_pxl] = x - last_diff_pxl;
			}
			
			last_diff_pxl = x;
		}
		else screen_canvas_diff[(unsigned char) x] = 0;
	}
	
	_OP_UpdatePageAccordToDiff(screen_canvas_diff, p, 0);
	_OP_UpdatePageAccordToDiff(screen_canvas_diff, p, 1);	

	for(unsigned char x = 0; x < SCR_WX; x ++)
	{
		screen[x][p] = canvas[x][p];
	}
}


#endif

void OP_SingleBufferUpdatePage(unsigned char p)
{
	unsigned char small_pixel_dat = 0;
	OLED_Set_Pos(_OLED_X_OFFSET_, 2 * p);
	for(unsigned char x = 0; x < SCR_WX; x ++)
	{
		small_pixel_dat = 0;
		for(unsigned char digit = 0; digit < 4; digit ++)
		{
			small_pixel_dat |= ( ( (canvas[x][p] >> digit) & 1 ) << (digit * 2));
			small_pixel_dat |= ( ( (canvas[x][p] >> digit) & 1 ) << (digit * 2 + 1));
		}
		OLED_WrDat(small_pixel_dat);
		OLED_WrDat(small_pixel_dat);
	}
		
	OLED_Set_Pos(_OLED_X_OFFSET_, 2 * p + 1);
	for(unsigned char x = 0; x < SCR_WX; x ++)
	{
		small_pixel_dat = 0;
		for(unsigned char digit = 0; digit < 4; digit ++)
		{
			small_pixel_dat |= ( ( (canvas[x][p] >> (digit + 4)) & 1 ) << (digit * 2));
			small_pixel_dat |= ( ( (canvas[x][p] >> (digit + 4)) & 1 ) << (digit * 2 + 1));
		}
		OLED_WrDat(small_pixel_dat);
		OLED_WrDat(small_pixel_dat);
	}

#ifndef NODOUBLEBUFFER
	for(unsigned char x = 0; x < SCR_WX; x ++)
	{
		screen[x][p] = canvas[x][p];
	}
#endif
}

void OP_Update()
{
	
	for(unsigned char p = 0; p < PAGE_MAX; p ++)
	{
#ifndef NODOUBLEBUFFER

		OP_DoubleBufferUpdatePage(p);

#else

		OP_SingleBufferUpdatePage(p);

#endif
	}
}

void OP_PutPixel(unsigned char x, unsigned char y, unsigned char b)
{
	SetCanvasPixel(x, y, b);
}

void OP_Line(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned char b)
{
	if(x1 >= SCR_WX || x2 >= SCR_WX || y1 >= SCR_WY || y2 >= SCR_WY)
	{
		#ifndef NOERRORNOTICE
		if(x1 >= SCR_WX || y1 >= SCR_WY)
			OP_ErrorNotice(OPE_PAINTOUTOFCANVAS, x1, y1);
		if(x2 >= SCR_WX || y2 >= SCR_WY)
			OP_ErrorNotice(OPE_PAINTOUTOFCANVAS, x2, y2);
			#endif
		return;
	}
	
	for (unsigned char t = 0, tmax = max(abs(x2 - x1), abs(y2 - y1)); t <= tmax; t ++)
	{
		SetCanvasPixel(x1 + (x2 - x1) * t / tmax, y1 + (y2 - y1) * t / tmax, b);
	}
}

void __swap(unsigned char *a, unsigned char *b)
{
	*a ^= *b;
	*b ^= *a;
	*a ^= *b;
}

void OP_Triangle(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned char x3, unsigned char y3, unsigned char b, unsigned char bSolid)
{
	if(x1 >= SCR_WX || x2 >= SCR_WX || x3 >= SCR_WX || y1 >= SCR_WY || y2 >= SCR_WY || y3 >= SCR_WY)
	{
		#ifndef NOERRORNOTICE
		if(x1>= SCR_WX || y1 >= SCR_WY)
		OP_ErrorNotice(OPE_PAINTOUTOFCANVAS, x1, y1);
		if(x2 >= SCR_WX || y2 >= SCR_WY)
		OP_ErrorNotice(OPE_PAINTOUTOFCANVAS, x2, y2);
		if(x3 >= SCR_WX || y3 >= SCR_WY)
		OP_ErrorNotice(OPE_PAINTOUTOFCANVAS, x3, y3);
		#endif
		return;
	}
	
	if(!bSolid)
	{
		OP_Line(x1, y1, x2, y2, b);
		OP_Line(x3, y3, x2, y2, b);
		OP_Line(x1, y1, x3, y3, b);
	}
	else
	{
		/*if (y1>y2) __swap(&x1, &x2), __swap(&y1, &y2);
		if (y1>y3) __swap(&x1, &x3), __swap(&y1, &y3);
		if (y2>y3) __swap(&x2, &x3), __swap(&y2, &y3);
		
		unsigned char h1 = y2 - y1, h2 = y3 - y2, h13 = y3 - y1;
		for(unsigned char h = 0; h <= h1; h ++)
			OP_Line(x1 + (x2 - x1) * h / h1, y1 + h, x1 + (x3 - x1) * h / h13, y1 + h, b);
		/ *for(unsigned char h = 0; h <= h2; h ++)
			OP_Line(x3 + (x2 - x3) * h / h2, y3 - h, x3 + (x1 - x3) * h / h13, y3 - h, b);* /
		*/
		for (unsigned char t = 0, tmax = max(abs(x2 - x1), abs(y2 - y1)); t <= tmax; t ++)
		{
			if((x2 - x1) * t / tmax == (x2 - x1) * (t - 1) / tmax && (y2 - y1) * t / tmax == (y2 - y1) * (t - 1) / tmax)
			continue;
			OP_Line(x3, y3, x1 + (x2 - x1) * t / tmax, y1 + (y2 - y1) * t / tmax, b);
		}
		for (unsigned char t = 0, tmax = max(abs(x3 - x2), abs(y3 - y2)); t <= tmax; t ++)
		{
			if((x3 - x2) * t / tmax == (x3 - x2) * (t - 2) / tmax && (y3 - y2) * t / tmax == (y3 - y2) * (t - 2) / tmax)
			continue;
			OP_Line(x1, y1, x2 + (x3 - x2) * t / tmax, y2 + (y3 - y2) * t / tmax, b);
		}
		for (unsigned char t = 0, tmax = max(abs(x3 - x1), abs(y3 - y1)); t <= tmax; t ++)
		{
			if((x3 - x1) * t / tmax == (x3 - x1) * (t - 1) / tmax && (y3 - y1) * t / tmax == (y3 - y1) * (t - 1) / tmax)
			continue;
			OP_Line(x2, y2, x1 + (x3 - x1) * t / tmax, y1 + (y3 - y1) * t / tmax, b);
		}
	}
}

void OP_Rect(unsigned char top, unsigned char bottom, unsigned char left, unsigned char right, unsigned char b, unsigned char bSolid)
{
	if(left >= SCR_WX || right >= SCR_WX || top >= SCR_WY || bottom >= SCR_WY)
	{
		#ifndef NOERRORNOTICE
		if(left >= SCR_WX || top >= SCR_WY)
		OP_ErrorNotice(OPE_PAINTOUTOFCANVAS, left, top);
		if(right >= SCR_WX || bottom >= SCR_WY)
		OP_ErrorNotice(OPE_PAINTOUTOFCANVAS, right, bottom);
		#endif
		return;
	}
	
	if(!bSolid)
	{
		OP_Line(left, top, right, top, b);
		OP_Line(left, top, left, bottom, b);
		OP_Line(right, top, right, bottom, b);
		OP_Line(left, bottom, right, bottom, b);
	}
	else
	{
		if(top > bottom)
		{
			bottom ^= top;
			top ^= bottom;
			bottom ^= top;
		}
		if(left > right)
		{
			right ^= left;
			left ^= right;
			right ^= left;
		}
		
		for(unsigned char x = left; x <= right; x ++)
		{
			
			for (unsigned char y = top; y < (top / PIXELPERPAGE + 1) * PIXELPERPAGE; y ++)
			{
				SetCanvasPixel(x, y, b);
			}
			for(unsigned char p = top / PIXELPERPAGE + 1; p < bottom / PIXELPERPAGE; p ++)
			{
				canvas[x][p] = ( b ? 0xff : 0 );
			}
			for (unsigned char y = (bottom / PIXELPERPAGE) * PIXELPERPAGE; y <= bottom; y ++)
			{
				SetCanvasPixel(x, y, b);
			}
		}
	}
}

#define DEG_TO_RAD(deg) ((double)(deg) * 3.14159 / 180)

void OP_Circle(unsigned char x, unsigned char y, unsigned char r, unsigned char b, unsigned char bSolid) 
// efficiency too low, but can be improved by creating a self-made math header file, with sin and cos data arrays inside
// author too lazy to do that
{
	for(unsigned short theta = 0; theta < 360; theta ++)
	{
		if(bSolid == 0)
		{
			OP_PutPixel(x + r * quick_cos(DEG_TO_RAD(theta)), y + r * quick_sin(DEG_TO_RAD(theta)), b);
		}
		else
		{
			OP_Line(x, y, x + r * quick_cos(DEG_TO_RAD(theta)), y + r * quick_sin(DEG_TO_RAD(theta)), b);
		}
	}
}

#ifndef NOERRORNOTICE

void OP_ErrorNotice(unsigned char ucErrc)
{
	if (ucErrc == OPE_SUCCESS)
		return;
		
	if(SCR_WY < 5 || SCR_WX < 10) // canvas too small to draw
	{
		OP_Clear();
		OP_Rect(0, SCR_WY - 1, 0, SCR_WX - 1, 1, 0);
		OP_Update();
		_delay_ms(1000);
		return;
	}
	
	OP_Clear();
	switch(ucErrc)
	{
		case OPE_PAINTOUTOFCANVAS:
			OP_Rect(0, SCR_WY - 1, 0, SCR_WX - 1, 1, 0);
			OP_Rect(5, SCR_WY - 6, 5, SCR_WX - 6, 1, 1);
			OP_Update();
			_delay_ms(1000);
			break;
		
		case OPE_READOUTOFCANVAS:
			OP_PutPixel(0, 0, 1);
			OP_PutPixel(0, SCR_WY - 1, 1);
			OP_PutPixel(SCR_WX - 1, 0, 1);
			OP_PutPixel(SCR_WX - 1, SCR_WY - 1, 1);
			OP_Rect(5, SCR_WY - 6, 5, SCR_WX - 6, 1, 1);
			OP_Update();
			_delay_ms(1000);
			break;
	}
}

void OP_ErrorNotice(unsigned char ucErrc, uint8_t err_x, uint8_t err_y)
{
	if (ucErrc == OPE_SUCCESS)
	return;
	
	if(SCR_WY < 5 || SCR_WX < 10) // canvas too small to draw
	{
		OP_Clear();
		OP_Rect(0, SCR_WY - 1, 0, SCR_WX - 1, 1, 0);
		OP_Update();
		_delay_ms(1000);
		return;
	}
	
	OP_Clear();
	switch(ucErrc)
	{
		case OPE_PAINTOUTOFCANVAS:
		{
			OP_Rect(0, SCR_WY - 1, 0, SCR_WX - 1, 1, 0);
			OP_Rect(5, SCR_WY - 6, 5, SCR_WX - 6, 1, 1);
			
			char szErr[5];
			int len = 0;
			itoa(err_x, szErr, 10);
			len = strlen(szErr);
			OP_TextOut(6, 6, szErr, len, 0);
			itoa(err_y, szErr, 10);
			len = strlen(szErr);
			OP_TextOut(6, 14, szErr, len, 0);
			OP_Update();
			_delay_ms(1000);
			break;
		}
		
		case OPE_READOUTOFCANVAS:
		{
			OP_PutPixel(0, 0, 1);
			OP_PutPixel(0, SCR_WY - 1, 1);
			OP_PutPixel(SCR_WX - 1, 0, 1);
			OP_PutPixel(SCR_WX - 1, SCR_WY - 1, 1);
			OP_Rect(5, SCR_WY - 6, 5, SCR_WX - 6, 1, 1);
			
			char szErr[5];
			int len = 0;
			itoa(err_x, szErr, 10);
			len = strlen(szErr);
			OP_TextOut(6, 6, szErr, len, 0);
			itoa(err_y, szErr, 10);
			len = strlen(szErr);
			OP_TextOut(6, 14, szErr, len, 0);
			OP_Update();
			_delay_ms(1000);
			break;
		}
	}
}

#endif

void OP_TextOut(unsigned char x, unsigned char y, const char str[], unsigned char len, unsigned char b) // 6 x 8 chars
{
#define CHARWX 6 __CONST__
#define CHARWY 8 __CONST__
	for(unsigned char c = 0; c < len; c ++)
	{
		for (unsigned char col = 0; col < CHARWX; col ++)
		{
			if(x + c * CHARWX + col >= SCR_WX)
				return;
			
			#ifdef USE_V2_CODETAB
			char ch = GetP6x8ChDat_V2(str[c], col);
			#else
			char ch = GetP6x8ChDat(str[c], col);
			#endif
			
			if(b)
			{
				canvas[x + c * CHARWX + col][y / PIXELPERPAGE] |= (ch << (y % PIXELPERPAGE));
				if(y % PIXELPERPAGE != 0 && y / PIXELPERPAGE + 1 < SCR_WY)
				{
					canvas[x + c * CHARWX + col][y / PIXELPERPAGE + 1] |= (ch >> (PIXELPERPAGE - y % PIXELPERPAGE));
				}
			}
			else
			{
				canvas[x + c * CHARWX + col][y / PIXELPERPAGE] &= ~(ch << (y % PIXELPERPAGE));
				if(y % PIXELPERPAGE != 0 && y / PIXELPERPAGE + 1 < SCR_WY)
				{
					canvas[x + c * CHARWX + col][y / PIXELPERPAGE + 1] &= ~(ch >> (PIXELPERPAGE - y % PIXELPERPAGE));
				}
			}
		}
	}
}
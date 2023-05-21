/*
 * OLED_Paint.c
 *
 * Created: 2023/4/22 21:28:15
 *  Author: Bonjir
 */ 

#include "OLED_Paint.h"

unsigned char screen[SCR_WX][PAGE_MAX];

#define max(a, b) ((a) > (b) ? (a) : (b))

unsigned char GetScreenPixel(unsigned char x, unsigned char y)
{
	if(x >= SCR_WX || y >= SCR_WY) 
	{
		OP_ErrorNotice(OPE_READOUTOFCANVAS);
		return -1;
	}
	return ((screen[x][y / PIXELPERPAGE] >> (y % PIXELPERPAGE)) & 1);
}

void SetScreenPixel(unsigned char x, unsigned char y, unsigned char b)
{
	if(x >= SCR_WX || y >= SCR_WY)
	{
		OP_ErrorNotice(OPE_PAINTOUTOFCANVAS);
		return;
	}
	if (b == 0)
		(screen[x][y / PIXELPERPAGE] &= ~(1 << (y % PIXELPERPAGE)));
	else 
		(screen[x][y / PIXELPERPAGE] |= (1 << (y % PIXELPERPAGE)));
}

void OP_Clear()
{
	for(unsigned char x = 0; x < SCR_WX; x ++)
	for(unsigned page = 0; page < PAGE_MAX; page ++)
		screen[x][page] = 0;
}

void OP_Init()
{
	OLED_Init();
	OP_Clear();
}

void OP_Update()
{
	for(unsigned char p = 0; p < PAGE_MAX; p ++)
	{
		OLED_Set_Pos(0, p);
		for(unsigned char x = 0; x < SCR_WX; x ++)
		{
			OLED_WrDat(screen[x][p]);
		}
	}
}

void OP_PutPixel(unsigned char x, unsigned char y, unsigned char b)
{
	SetScreenPixel(x, y, b);
}

void OP_Line(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned char b)
{
	if(x1 >= SCR_WX || x2 >= SCR_WX || y1 >= SCR_WY || y2 >= SCR_WY)
	{
		OP_ErrorNotice(OPE_PAINTOUTOFCANVAS);
		return;
	}
	
	if(x1 > x2)
	{
		x2 ^= x1;
		x1 ^= x2;
		x2 ^= x1;
		
		y2 ^= y1;
		y1 ^= y2;
		y2 ^= y1; // swap
	}
	for (unsigned char t = 0, tmax = max(x2 - x1, y2 - y1); t <= tmax; t ++)
	{
		SetScreenPixel(x1 + (x2 - x1) * t / tmax, y1 + (y2 - y1) * t / tmax, b);
	}
}

void OP_Triangle(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned char x3, unsigned char y3, unsigned char b, unsigned char bSolid)
{
	if(x1 >= SCR_WX || x2 >= SCR_WX || x3 >= SCR_WX || y1 >= SCR_WY || y2 >= SCR_WY || y3 >= SCR_WY)
	{
		OP_ErrorNotice(OPE_PAINTOUTOFCANVAS);
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
		// TODO
	}
}

void OP_Rect(unsigned char top, unsigned char bottom, unsigned char left, unsigned char right, unsigned char b, unsigned char bSolid)
{
	if(left >= SCR_WX || right >= SCR_WX || top >= SCR_WY || bottom >= SCR_WY)
	{
		OP_ErrorNotice(OPE_PAINTOUTOFCANVAS);
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
			for (unsigned char y = top; y < (top / 8 + 1) * 8; y ++)
			{
				SetScreenPixel(x, y, b);
			}
			for(unsigned char p = top / 8 + 1; p < bottom / 8; p ++)
			{
				screen[x][p] = ( b ? 0xff : 0 );
			}
			for (unsigned char y = (unsigned char)(bottom / 8); y <= bottom; y ++)
			{
				SetScreenPixel(x, y, b);
			}
		}
	}
}

void OP_ErrorNotice(unsigned char ucErrc)
{
	if (ucErrc == OPE_SUCCESS)
		return;
		
	if(SCR_WY < 5 || SCR_WX < 10) // canvas too small to draw
	{
		OP_Clear();
		OP_Rect(1, SCR_WY - 1, 1, SCR_WX - 1, 1, 0);
		OP_Update();
		_delay_ms(1000);
		return;
	}
	
	OP_Clear();
	switch(ucErrc)
	{
		case OPE_PAINTOUTOFCANVAS:
			OP_Rect(0, SCR_WY - 1, 1, SCR_WX - 1, 1, 0);
			OP_Rect(6, SCR_WY - 5, 6, SCR_WX - 5, 1, 1);
			OP_Update();
			_delay_ms(1000);
			break;
		
		case OPE_READOUTOFCANVAS:
			OP_PutPixel(1, 1, 1);
			OP_PutPixel(1, SCR_WY - 1, 1);
			OP_PutPixel(SCR_WX - 1, 1, 1);
			OP_PutPixel(SCR_WX - 1, SCR_WY - 1, 1);
			OP_Rect(5, SCR_WY - 5, 5, SCR_WX - 5, 1, 1);
			OP_Update();
			_delay_ms(1000);
			break;
	}
}

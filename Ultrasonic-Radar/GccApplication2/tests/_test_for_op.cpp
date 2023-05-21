/*
 * GccApplication2.c
 *
 * Created: 2023/4/21 19:15:49
 * Author : Bonjir
 */ 

#include <avr/io.h>
#include "OLED_Paint.h"

int main(void)
{
	OP_Init(); //OLED初始化
	OLED_Clear();
	while(1)
	{
		TestForTextOut();
	}
}

void TestForPutPixel()
{
	OP_PutPixel(3, 3, 1);
	OP_PutPixel(4, 4, 1);
	OP_PutPixel(5, 5, 1);
	OP_PutPixel(6, 6, 1);
	OP_Update();
	_delay_ms(1000);
	OP_PutPixel(3, 3, 0);
	OP_PutPixel(4, 4, 0);
	OP_PutPixel(5, 5, 0);
	OP_PutPixel(6, 6, 0);
	OP_Update();
	_delay_ms(1000);
}

void TestForLine()
{
	OP_Line(0, 0, 10, 20, 1);
	OP_Update();
	_delay_ms(1000);
	OP_Line(0, 0, 10, 20, 0);
	OP_Update();
	_delay_ms(1000);
}

void TestForTriangle()
{
	OP_Triangle(0, 0, 0, 10, 10, 20, 1, 0);
	OP_Update();
	_delay_ms(1000);
	OP_Triangle(0, 0, 0, 10, 10, 15, 1, 1);
	OP_Update();
	_delay_ms(1000);
	OP_Clear();
	OP_Update();
	
	OP_Triangle(rand() % SCR_WX, rand() % SCR_WY, rand() % SCR_WX, rand() % SCR_WY, rand() % SCR_WX, rand() % SCR_WY, 1, 1);
	OP_Update();
	_delay_ms(1000);
	OP_Clear();
	OP_Update();
}

void TestForRect()
{
	//OP_Rect(15, 32, 13, 56, 1, 1);
	OP_Rect(0, SCR_WY - 1, 0, SCR_WX - 1, 1, 0);
	OP_Line(0, 0, 10, 10, 1);
	//OP_Line(0, 0, 0, 10, 1);
	OP_Update();
	_delay_ms(1000);

	OP_Rect(15, 33, 10, 20, 1, 1); // trigger error notice 001
	OP_Update();
	_delay_ms(1000);
	
	OP_Clear();
	OP_Update();
	_delay_ms(1000);
}

void TestForRect2()
{
	OP_ErrorNotice(OPE_READOUTOFCANVAS);
	OP_Line(0, 0, 10, 0, 1);
	OP_Line(0, 2, 10, 2, 1);
	OP_Line(0, 4, 10, 4, 1);
	OP_Line(0, 6, 10, 6, 1);
	OP_Line(0, 8, 10, 8, 1);
	OP_Line(0, 10, 10, 10, 1);
	OP_Rect(0, SCR_WY - 1, 0, SCR_WX - 1, 1, 0);
	OP_Rect(5, SCR_WY - 6, 5, SCR_WX - 6, 1, 1);
	OP_Update();
	_delay_ms(1000);
}

void TestForDoubleBuffer()
{
	/*OP_Line(0, 0, SCR_WX - 1, 0, 1);
	OP_Update();
	_delay_ms(1000);
	*/
	TestForRect(); // 两次以上第一行的横线会部分缺失
	
	OP_Line(0, 0, 0, SCR_WY - 1, 1);
	OP_Line(1, 0, 1, 5, 1);
	OP_Line(2, 0, 2, 10, 1);
	OP_Line(4, 0, 4, 20, 1);
	OP_Line(7, 0, 7, 30, 1);
	OP_Line(11, 0, 11, SCR_WY - 1, 1);
	OP_Line(16, 0, 16, 30, 1);
	
	OP_Line(SCR_WX - 1, 0, SCR_WX - 1, SCR_WY - 1, 1);
	OP_Line(SCR_WX - 2, 0, SCR_WX - 2, 10, 1);
	
	OP_Line(SCR_WX - 4, 0, SCR_WX - 4, SCR_WY - 1, 1);
	OP_Update();
	_delay_ms(1000);
	
	OP_Clear();
	OP_Update();
	_delay_ms(1000);
	
}


void TestForDoubleBuffer2() // unknown bug
{
	/*unsigned char top = 15, bottom = 33, left = 10, right = 20;
	//OP_Rect(top, bottom, left, right, 1, 1);
	*/
		unsigned char ucErrc = OPE_PAINTOUTOFCANVAS;
		if (ucErrc == OPE_SUCCESS)
		goto End;
		
		if(SCR_WY < 5 || SCR_WX < 10) // canvas too small to draw
		{
			OP_Clear();
			OP_Rect(1, SCR_WY - 1, 1, SCR_WX - 1, 1, 0);
			OP_Update();
			_delay_ms(1000);
			goto End;
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
		
	End:
		OP_Update();
		_delay_ms(1000);
	
	
	OP_Clear();
	OP_Update();
	_delay_ms(1000);
	
	
	OP_ErrorNotice(OPE_PAINTOUTOFCANVAS);
	OP_Update();
	_delay_ms(1000);
	
	OP_Clear();
	OP_Update();
	_delay_ms(1000);
	
	
	return;
}

void TestForCircle()
{
	OP_Circle(15, 15, 10, 1, 0);
	OP_Circle(40, 15, 10, 1, 1);
	OP_Update();
	_delay_ms(1000);
}

void TestForTextOut()
{
	/*char str[1] = "H";
	OP_TextOut(0, 0, str, 1, 1);
	OP_TextOut(1, 1, "H", 1, 1);
	OP_TextOut(3, 3, "H", 1, 1);
	OP_TextOut(7, 7, "H", 1, 1);
	OP_TextOut(15, 15, "H", 1, 1);*/
	OP_TextOut(2, 5, "Hello,", 6, 1);
	OP_TextOut(26, 16, "World!", 6, 1);
	OP_Update();
	_delay_ms(1000);
}
/*
 * test_for_codetab_new.cpp
 *
 * Created: 2023/4/29 20:15:36
 *  Author: Bonjir
 */ 

#define F_CPU 16000000UL
#include <util/delay.h>
#include "OLED_Paint.h"

int main(void) {
	
	OP_Init();
	
	while(1)
	{
		OP_Clear();
		OP_TextOut(0, 0, "Hello,", 6, 1);
		OP_TextOut(10, 10, "World!", 6, 1);
		OP_Update();
		
		_delay_ms(2000);
		
		OP_PutPixel(100, 100, 1);
		OP_Update();
		
		_delay_ms(2000);
	}
	while(1);
}

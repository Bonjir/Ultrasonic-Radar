/*
 * _test_for_btnmat3.cpp
 *
 * Created: 2023/5/16 20:27:44
 *  Author: Bonjir
 */ 
#define F_CPU 16000000UL
#include <stdlib.h>
#include "pins.h"
#include "btnmat.h"
#include "OLED_Paint.h"


int main(void)
{
	OP_Init();
	BTNMAT btnmat;
	PINS btnmat_pins[8] = {PINS_B4, PINS_B3, PINS_B2, PINS_B1, PINS_D5, PINS_D6, PINS_D7, PINS_B0};
	btnmat.Attach(btnmat_pins);
	
	while(1)
	{
		
		OP_Clear();
		
		uint8_t key_pressed = btnmat.GetKeyDown();
		
		char num[4];
		memset(num, 0, sizeof(num));
		itoa(key_pressed, num, 10);
		OP_TextOut(0, 0, num, strlen(num), 1);
		
		OP_Update();
		
	}
}
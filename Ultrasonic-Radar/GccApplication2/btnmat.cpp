/*
 * btnmat.cpp
 *
 * Created: 2023/5/16 20:20:18
 *  Author: Bonjir
 */ 

#define F_CPU 16000000UL
#include <util/delay.h>
#include "btnmat.h"

void BUTTONMATRIX::Attach(PINS pins[8])
{
	for(int p = 0; p < 8; p ++)
		this->pins[p] = pins[p];
	for(int p = 0; p < 4; p ++)
		SetPinMode(pins[p], INPUT);
	for(int p = 4; p < 8; p ++)
		SetPinMode(pins[p], OUTPUT);
}

uint8_t BUTTONMATRIX::GetKeyDown()
{
	for(int p = 0; p < 8; p ++)
		DigitalWrite(this->pins[p], 1);
#define ReLU(x) (max(0, (x)))
	for(int row = 0; row < 4; row ++)
	{
		DigitalWrite(this->pins[ReLU(row - 1) + 4], 1);
		DigitalWrite(this->pins[row + 4], 0);
		_delay_us(2);
		for(int i = 1; i <= 4; i ++)
		{
			if(DigitalRead(this->pins[4 - i]) == 0)
			{
				return row * 4 + i;
			}
		}
	}
	return 0;
}
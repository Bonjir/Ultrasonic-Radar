/*
 * test_for_two_servo.cpp
 *
 * Created: 2023/5/4 19:43:02
 *  Author: Bonjir
 */ 

#define F_CPU 16000000UL
#include <util/delay.h>
#include "servo.h"

int main()
{
	SetPinMode(PINS_C0, OUTPUT); // DEBUG
	
	SERVO servo1, servo2;
	servo1.Attach(PINS_D4);
	servo2.Attach(PINS_D3);
	
	while(1)
	{
		servo1.SetPos(0);
		servo2.SetPos(180);
		_delay_ms(1000);
		servo1.SetPos(90);
		servo2.SetPos(90);
		_delay_ms(1000);
		servo1.SetPos(180);
		servo2.SetPos(0);
		_delay_ms(3000);
	}
}
/*
 * test_for_servo.cpp
 *
 * Created: 2023/5/3 22:08:00
 *  Author: Bonjir
 */ 

#define F_CPU 16000000UL
#include <util/delay.h>
#include "servo.h"

int main()
{
	SetPinMode(PINS_C0, OUTPUT); // DEBUG
	
	SERVO servo;
	servo.Attach(PINS_D4);
	
	while(1)
	{
		while(SERVO::IsSetPosAvailable() == 0);
		servo.SetPos(0);
		_delay_ms(1000);
		while(SERVO::IsSetPosAvailable() == 0);
		servo.SetPos(90);
		_delay_ms(1000);
		while(SERVO::IsSetPosAvailable() == 0);
		servo.SetPos(180);
		_delay_ms(3000);
	}
}
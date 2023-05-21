/*
 * test_for_timer.cpp
 *
 * Created: 2023/5/3 20:53:57
 *  Author: Bonjir
 */ 

#include "pins.h"
#include "timer.h"

uint8_t pinLed = PINS_C0;
bool bLedMode = 0;
uint32_t timer_cnt = 0;
uint32_t timer_intv = 10000;
void TimerFunc()
{
	timer_cnt ++;
	if(timer_cnt >= 1000000 / timer_intv)
	{
		timer_cnt = 0;
		bLedMode = !bLedMode;
		DigitalWrite(pinLed, bLedMode);
	}
}

int main()
{
	SetPinMode(pinLed, 1);
	SetTimer(timer_intv, &TimerFunc);
	
	while(1);
}
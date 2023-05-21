/*
 * timer_test1.c
 *
 * Created: 2023/5/2 18:44:33
 *  Author: Bonjir
 *    Note: 1MHz外部晶振    10ms定时
*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include "pins.h"

#define CRYSTALOSCILLATOR_FREQUENCY (1 * 1000000)
#define TIMER1_CALCNT(millisec, ff) (65535 - ((millisec) * CRYSTALOSCILLATOR_FREQUENCY / 1000 / (ff)) )
//#define TIMER1_CALCNT_MILLISEC(millisec, ff) (65535 - ((millisec) * CRYSTALOSCILLATOR_FREQUENCY / 1000 / (ff)) )
#define TIMER1_CALCNT_MICROSEC(microsec, ff) (65535 - ((microsec) * (CRYSTALOSCILLATOR_FREQUENCY / 1000000) / (ff)) )
#define TIMER1_0FF 0b000
#define TIMER1_1FF 0b001
#define TIMER1_8FF 0b010
#define TIMER1_64FF 0b011
#define TIMER1_256FF 0b100
#define TIMER1_1024FF 0b101

const int timer_interval = 1000, fracfreq = 64;
int pinLed = PINS_C0;

static int32_t _fracfreq;
static int32_t _interval_microsec;

int main(void)
{
	/*
	DDRD |= (1 << DDRD4);*/
	SetPinMode(pinLed, OUTPUT);
	/*if(interval_microsec <= 0) interval_microsec = 1;
	
	if (interval_microsec <= 0xffff * 1)
	{
		_fracfreq = 1;
		TCCR1B = TIMER1_1FF;
	}
	else if (interval_microsec <= 0xffff * 8)
	{
		_fracfreq = 8;
		TCCR1B = TIMER1_8FF;
	}
	else if (interval_microsec <= 0xffff * 64)
	{
		_fracfreq = 64;
		TCCR1B = TIMER1_64FF;
	}
	else if (interval_microsec <= 0xffff * 256)
	{
		_fracfreq = 256;
		TCCR1B = TIMER1_256FF;
	}
	else if (interval_microsec <= 0xffff * 1024)
	{
		_fracfreq = 1024;
		TCCR1B = TIMER1_1024FF;
	}
	else return 0;*/
	TCCR1B = TIMER1_64FF; // DEBUG close ff decision
	/*
	TCNT1 = TIMER1_CALCNT_MICROSEC(interval_microsec, _fracfreq);*/
	_interval_microsec = 1000000;
	_fracfreq = 64;
	TCNT1 = TIMER1_CALCNT_MICROSEC(_interval_microsec, _fracfreq); // DEBUG close tcnt1 cal
	
	TIMSK |= (1 << TOIE1); // timer interruption on
	sei();
	
	/*_TimerFunc = TimerFunc;*//*
	_interval_microsec = interval_microsec;*/
	
	
	while(1);
	
	return 0;
}


int i = 0;
bool bLedMode = 0;

void TimerFunc()
{
	bLedMode = !bLedMode;
	DigitalWrite(pinLed, bLedMode);
}

ISR(TIMER1_OVF_vect)
{
	
	TCNT1 = TIMER1_CALCNT_MICROSEC(_interval_microsec, _fracfreq);
	TimerFunc();
	/*
	/ *
	i++;* /
	TCNT1 = TIMER1_CALCNT(timer_interval, fracfreq);
	/ *TCNT1 = TIMER1_CALCNT(timer_interval,fracfreq);* /
	/ *
	if(i == 100)
	{
		i = 0;* /
		bLedMode = !bLedMode;
		DigitalWrite(pinLed, bLedMode);
	/ *	/ *PORTC ^= (1 << PORTC0);* /
	}* /*/
}
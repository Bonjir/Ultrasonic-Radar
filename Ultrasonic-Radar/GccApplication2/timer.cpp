/*
 * timer.cpp
 *
 * Created: 2023/5/3 11:20:19
 *  Author: Bonjir
 */ 
//#define 
#include "timer.h"


static void (*_TimerFunc)();
static uint16_t _fracfreq;
static int32_t _interval_microsec;

bool SetTimer(int32_t interval_microsec, void (*TimerFunc)(), uint16_t ff)
{
	if(interval_microsec <= 0) interval_microsec = 1;
	
	if(ff)
	{
		_fracfreq = ff;
	}
	else
	{
		if (interval_microsec <= 65535 * 1)
		{
			_fracfreq = 1;
		}
		else if (interval_microsec <= 65535 * 8)
		{
			_fracfreq = 8;
		}
		else if (interval_microsec <= 65535 * 64)
		{
			_fracfreq = 64;
		}
		else if (interval_microsec <= 65535 * 256)
		{
			_fracfreq = 256;
		}
		else if (interval_microsec <= 65535 * 1024)
		{
			_fracfreq = 1024;
		}
		else return 0;
	}
	
	if (_fracfreq == 1)
	{
		TCCR1B = TIMER_1FF;
	}
	else if (_fracfreq == 8)
	{
		TCCR1B = TIMER_8FF;
	}
	else if (_fracfreq == 64)
	{
		TCCR1B = TIMER_64FF;
	}
	else if (_fracfreq == 256)
	{
		TCCR1B = TIMER_256FF;
	}
	else if (_fracfreq == 1024)
	{
		TCCR1B = TIMER_1024FF;
	}
	else return 0;
	
	TCNT1 = TIMER1_CALCNT_MICROSEC(interval_microsec, _fracfreq);
	
	TIMSK |= (1 << TOIE1); // timer interruption on
	sei();
	
	_TimerFunc = TimerFunc;
	_interval_microsec = interval_microsec;
	
	return true;
}


void __ResetTimerInterval(int32_t interval_microsec, uint8_t ff) // only used for set the static variable _interval_microsec and _fracfreq
{
	_interval_microsec = interval_microsec;
	_fracfreq = ff;
}


void KillTimer()
{
	cli();
	TIMSK &= ~(1 << TOIE1); // timer interruption off
}

bool IsTimerOn()
{
	return (bool)(TIMSK & (1 << TOIE1));
}

/*
void SetAutoRecoverTimerCounter(bool bAuto)
{
	auto_recover_timer_cnt = bAuto;
}
*/

/*
void SetFracFreq(uint8_t ff)
{
	_fracfreq = ff;
}
*/

static bool auto_set_timer_cnt = 0;
void AutoSetTimerInterval(bool bAuto)
{
	auto_set_timer_cnt = bAuto;
}

ISR(TIMER1_OVF_vect)
{
	if(auto_set_timer_cnt)
		TCNT1 = TIMER1_CALCNT_MICROSEC(_interval_microsec, _fracfreq);
	_TimerFunc();
}


#ifndef FREE_TIMER0

/************************************************************************/
/*  timer 0                                                             */
/************************************************************************/
static uint16_t _fracfreq0;
static uint32_t timer0_overflow_cnt = 0;

bool IsSysClockOn()
{
	return TIMSK & (1 << TOIE0);
}

void SetSysClock(uint16_t ff)
{
	if (ff == 0)
	{
		TCCR0 = TIMER_0FF;
		TIMSK &= ~(1<<TOIE0);
		return;
	}
	
	_fracfreq0 = ff;
	if (_fracfreq0 == 1)
	{
		TCCR0 = TIMER_1FF;
	}
	else if (_fracfreq0 == 8)
	{
		TCCR0 = TIMER_8FF;
	}
	else if (_fracfreq0 == 64)
	{
		TCCR0 = TIMER_64FF;
	}
	else if (_fracfreq0 == 256)
	{
		TCCR0 = TIMER_256FF;
	}
	else if (_fracfreq0 == 1024)
	{
		TCCR0 = TIMER_1024FF;
	}
	else return;
	
	timer0_overflow_cnt = 0;
	TCNT0 = 0;
	TIMSK |= (1<<TOIE0);
	sei();
}

uint32_t GetSysClock()
{
	return (timer0_overflow_cnt * TIMER0_OVERFLOW_MAX + TCNT0) * _fracfreq0 / MHZ_PER_US;
}

ISR(TIMER0_OVF_vect)
{
	timer0_overflow_cnt ++;
}

#endif
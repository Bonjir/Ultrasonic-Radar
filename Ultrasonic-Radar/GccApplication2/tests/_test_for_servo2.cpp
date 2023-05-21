/*
 * test_for_servo_2.cpp
 *
 * Created: 2023/5/4 16:37:24
 *  Author: Bonjir
 */

#include "pins.h"
#include "timer.h"

const int timer_interval = 1000, fracfreq = 1;

void TimerFunc();

int main(void)
{
	SetPinMode(PINS_D4, OUTPUT);
	SetPinMode(PINS_C0, OUTPUT);
	
	SetTimer(timer_interval, TimerFunc, 1);
	
	while(1);
	
	return 0;
}

#define MIN_PULSE_WIDTH      544     // the shortest pulse sent to a servo
#define MAX_PULSE_WIDTH      2400     // the longest pulse sent to a servo
#define DEFAULT_PULSE_WIDTH  1500     // default pulse width when servo is attached
#define REFRESH_INTERVAL    20000     // minimum time to refresh servos in microseconds

int cntLed = 0, bSendingHigh = 0;

int highlevel_delay = MAX_PULSE_WIDTH;
/*

void SetTcnt(int a, int b)
{		
	TCNT1 = TIMER1_CALCNT_MICROSEC(a, b);
}*/
/*
void (*SetTcnt_Cpy)(int, int) = SetTcnt;
*/

void TimerFunc()
{
	cntLed++;
	
	if(bSendingHigh == 0)
	{
		//ResetTimerInterval(highlevel_delay, fracfreq);
		ResetTimerInterval(highlevel_delay, fracfreq);
		//TCNT1 = TIMER1_CALCNT_MICROSEC(highlevel_delay, fracfreq);
		DigitalWrite(PINS_D4, 1);
		bSendingHigh = 1;
	}
	else
	{
		ResetTimerInterval(REFRESH_INTERVAL - highlevel_delay, fracfreq);
		//TCNT1 = TIMER1_CALCNT_MICROSEC(REFRESH_INTERVAL - highlevel_delay, fracfreq);
		DigitalWrite(PINS_D4, 0);
		bSendingHigh = 0;
		/*if(cntLed >= 30)
		{
			TIMSK &= ~(1 << TOIE1); // turn off interruption
			cli();
		}*/
	}
	
	if(cntLed == 100)
	{		
		cntLed = 0;
		PORTC ^= (1 << PORTC0);
	}
}
/*
 * timer.h
 *
 * Created: 2023/5/3 10:59:45
 *  Author: Bonjir
 *    Note: * 1 MHz Crystal Oscillator
 *			* SetSysClock() and GetSysClock() will occupy timer0 and its interruption function ISR(TIMER0_OVF_VECTOR),
 *			if you want to free timer0, please define FREE_TIMER0 in timer.h
 */ 


#ifndef TIMER_H_
#define TIMER_H_

#include <avr/interrupt.h>

#define CRYSTALOSCILLATOR_FREQUENCY (16 * 1000000)
#define TIMER1_CALCNT_MICROSEC(microsec, ff) (65535 - ((microsec) * (CRYSTALOSCILLATOR_FREQUENCY / 1000000) / (ff)) )
#define TIMER_0FF 0b000
#define TIMER_1FF 0b001
#define TIMER_8FF 0b010
#define TIMER_64FF 0b011
#define TIMER_256FF 0b100
#define TIMER_1024FF 0b101

#ifndef __CONST__
#define __CONST__
#endif
#ifndef __VARIABLE__
#define __VARIABLE__
#endif
#ifndef __SWITCH__
#define __SWITCH__
#endif

bool SetTimer(int32_t interval_microsec, void (*TimerFunc)(), uint16_t ff = 0);
void AutoSetTimerInterval(bool bAuto);
void KillTimer();
bool IsTimerOn();
void SetTcnt(int a, int b);

void __ResetTimerInterval(int32_t interval_microsec, uint8_t ff); // only used for set the static variable _interval_microsec and _fracfreq, don't use

#define ResetTimerInterval(interval_microsec, ff) \
{ \
	TCNT1 = TIMER1_CALCNT_MICROSEC((interval_microsec), (ff)); \
	__ResetTimerInterval((interval_microsec), (ff));\
}


//#define FREE_TIMER0 __SWITCH__
#ifndef FREE_TIMER0

#define TIMER0_OVERFLOW_MAX 256
#define MHZ_PER_US 16 // 16e6 Hz / 1e6
bool IsSysClockOn();
void SetSysClock(uint16_t ff); // set ff = 0 to turn off
uint32_t GetSysClock();


#endif

#endif /* TIMER_H_ */
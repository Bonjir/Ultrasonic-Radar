/*
 * servo.cpp
 *
 * Created: 2023/5/3 22:06:59
 *  Author: Bonjir
 */ 

#include "servo.h"


SERVO* SERVO::setpos_queue[MAXN_SETPOSQUEUE];
uint8_t SERVO::queue_len;
bool SERVO::queue_top_servo_pwm_sta;
uint8_t SERVO::queue_top_servo_progress;

SERVO::SERVO()
{
	this->attached_pin = 0xff;
}

void SERVO::Attach(uint8_t pin)
{
	this->attached_pin = pin;
	SetPinMode(this->attached_pin, 1);
}

void SERVO::Detach()
{
	this->attached_pin = 0xff;
}

bool SERVO::IsAttached()
{
	return IsPinValid(this->attached_pin);
}

#ifndef min
#define min(a, b) ((a) > (b) ? (b) : (a))
#endif
#ifndef max
#define max(a, b) ((a) > (b) ? (a) : (b))
#endif

uint8_t SERVO::GetPos()
{
	return this->pos;
}

#define GetPulseWidthFromPos(pos) ((MAXPINS_ULSE_WIDTH - MINPINS_ULSE_WIDTH) * (pos) / 180)

void SERVO::SetPos(int pos)
{
	if(SERVO::queue_len >= MAXN_SETPOSQUEUE)
		return;
	this->pos = pos;
	setpos_queue[queue_len] = this;
	queue_len ++;
	
	if(queue_len == 1) // the first in queue
	{
		SERVO::queue_top_servo_progress = 0;
		SERVO::queue_top_servo_pwm_sta = 0;
		SetTimer(GetPulseWidthFromPos(SERVO::setpos_queue[0]->pos), SERVO::SendPWMFunc);
	}
}

void SERVO::SendPWMFunc()
{
	if(SERVO::queue_top_servo_pwm_sta == 0)
	{
		DigitalWrite( SERVO::setpos_queue[0]->attached_pin, 1 );
		SERVO::queue_top_servo_pwm_sta = 1;
		SetTimer(GetPulseWidthFromPos(SERVO::setpos_queue[0]->pos),SERVO::SendPWMFunc);
	}
	else // sending high PWM signal
	{
		DigitalWrite( SERVO::setpos_queue[0]->attached_pin, 0 );
		SERVO::queue_top_servo_pwm_sta = 0;
		SERVO::queue_top_servo_progress ++;
		
		if(SERVO::queue_top_servo_progress >= NTIMES_PWM_SIGNAL) // queue top servo has finished setting pos, queue pop
		{
			SERVO::queue_top_servo_progress = 0;
			for(int i = 1; i < SERVO::queue_len; i ++)
				SERVO::setpos_queue[i - 1] = SERVO::setpos_queue[i];
			SERVO::queue_len --;
			
			if(SERVO::queue_len == 0)
			{
				KillTimer();
			}
			else
			{
				SetTimer(GetPulseWidthFromPos(SERVO::setpos_queue[0]->pos), SERVO::SendPWMFunc); // deal with the next queue top servo
			}
		}
		else
		{
			SetTimer(REFRESH_INTERVAL - GetPulseWidthFromPos(SERVO::setpos_queue[0]->pos), SERVO::SendPWMFunc); // queue top servo hasn't finished
		}
	}
}
/*
 * servo.cpp
 *
 * Created: 2023/5/3 22:06:59
 *  Author: Bonjir
 */ 

#include "servo.h"



//uint8_t SERVO::nServo;
bool SERVO::is_setpos_available = 1;
uint8_t SERVO::setpos_progress;
SERVO* SERVO::setpos_servo;


SERVO::SERVO()
{
	this->attached_pin = 0xff;
	this->pos = 0xff;
}

void SERVO::Attach(uint8_t pin)
{
	this->attached_pin = pin;
	this->_attached_pin_group = GETPIN_GROUP(this->attached_pin);
	this->_attached_pin_number_shifted = GETPIN_NUMBERSHIFTED(this->attached_pin);
	SetPinMode(this->attached_pin, 1);
}

void SERVO::Detach()
{
	this->attached_pin = 0xff;
	this->_attached_pin_group = GETPIN_GROUP(this->attached_pin);
	this->_attached_pin_number_shifted = GETPIN_NUMBERSHIFTED(this->attached_pin);
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
#ifndef abs
#define abs(x) ((x) > 0 ? (x) : -(x))
#endif

uint8_t SERVO::GetPos()
{
	return this->pos;
}

const int32_t k_deg_to_pulsew = (MAX_PULSE_WIDTH - MIN_PULSE_WIDTH) / 180;
//#define GetPulseWidthFromPos(pos) (k_deg_to_pulsew * (pos) + MIN_PULSE_WIDTH)
int32_t GetPulseWidthFromPos(int32_t pos)
{
	return (k_deg_to_pulsew * (pos) + MIN_PULSE_WIDTH);
}

bool SERVO::SetPos(uint8_t pos)
{
	sei();
	while(SERVO::IsSetPosAvailable() == 0) // wait for other servos
	{
		DigitalWrite(PINS_NULL, 0); // avoid being detected as a null no-end while loop 
	};
	
	/*
	if(SERVO::is_setpos_available == 0)
		return 0;
	*/
	
	SERVO::is_setpos_available = 0;
	SERVO::setpos_servo = this;
	this->_setpos_progress_max = abs(pos - this->pos) * (50 - 10) / 180 + 10; // move 0 deg -> progress_max = 10; move 180 deg -> progress_max = 50
	this->pos = pos;
	this->_pulse_width = GetPulseWidthFromPos(this->pos);
	SetTimer(100, SERVO::SendPWMFunc, FRACFREQ);
	//SetTimer(REFRESH_INTERVAL - this->_pulse_width,SERVO::SendPWMFunc, FRACFREQ);
	return 1;
}

bool SERVO::IsSetPosAvailable()
{
	return SERVO::is_setpos_available;
}

static bool bSendingPWMHigh = 0;


void SERVO::SendPWMFunc()
{
	SERVO::setpos_progress ++;
	if(bSendingPWMHigh == 0)
	{
		ResetTimerInterval(SERVO::setpos_servo->_pulse_width, FRACFREQ);
		//ResetTimerInterval(GetPulseWidthFromPos(SERVO::setpos_servo->pos), FRACFREQ);
		//DigitalWrite(SERVO::setpos_servo->attached_pin, 1);
		DigitalWrite_Fast(SERVO::setpos_servo->_attached_pin_group,SERVO::setpos_servo->_attached_pin_number_shifted, 1);
		bSendingPWMHigh = 1;
	}
	else
	{
		ResetTimerInterval(REFRESH_INTERVAL - SERVO::setpos_servo->_pulse_width, FRACFREQ);
		//ResetTimerInterval(REFRESH_INTERVAL - GetPulseWidthFromPos(SERVO::setpos_servo->pos), FRACFREQ);
		//DigitalWrite(SERVO::setpos_servo->attached_pin, 0);
		DigitalWrite_Fast(SERVO::setpos_servo->_attached_pin_group,SERVO::setpos_servo->_attached_pin_number_shifted, 0);
		bSendingPWMHigh = 0;
		if(SERVO::setpos_progress >= SERVO::setpos_servo->_setpos_progress_max)
		{
			KillTimer();
			SERVO::setpos_progress = 0;
			is_setpos_available = 1;
		}
	}
}
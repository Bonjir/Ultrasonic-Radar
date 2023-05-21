/*
 * servo.h
 *
 * Created: 2023/5/3 11:22:01
 *  Author: Bonjir
 * 
 *	  Todo: 
 *
 */ 


#ifndef SERVO_H_
#define SERVO_H_

#include "pins.h"
#include "timer.h"

#define MIN_PULSE_WIDTH      500 // 470
#define MAX_PULSE_WIDTH      2600 // 2600 
#define DEFAULT_ULSE_WIDTH		1500
#define REFRESH_INTERVAL		20000 // 20 ms as a cycle for PWM signals

#define DEFAULT_DEG 90
//#define MAXN_SERVO 5 __VARIABLE__
#define MAXN_SETPOSQUEUE 10 __VARIABLE__
#define NTIMES_PWM_SIGNAL 30 __VARIABLE__
#define FRACFREQ 8

class SERVO
{
public:
	SERVO();
	void Attach(uint8_t pin);
	void Detach();
	bool SetPos(uint8_t pos);
	uint8_t GetPos();
	bool IsAttached();
	static bool IsSetPosAvailable();
	
private:
	uint8_t attached_pin;
	uint8_t pos;
	uint8_t _attached_pin_group;
	uint8_t _attached_pin_number_shifted;
	uint32_t _pulse_width;
	uint8_t _setpos_progress_max;
	
	//static uint8_t nServo;
	static bool is_setpos_available;
	static uint8_t setpos_progress;
	static SERVO* setpos_servo;
	static void SendPWMFunc();
};

#endif /* SERVO_H_ */
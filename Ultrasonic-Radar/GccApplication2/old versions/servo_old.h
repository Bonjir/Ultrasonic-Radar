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

#define MINPINS_ULSE_WIDTH      500
#define MAXPINS_ULSE_WIDTH      2500
#define DEFAULT_ULSE_WIDTH		1500
#define REFRESH_INTERVAL		20000 // 20 ms as a cycle for PWM signals

#define DEFAULT_DEG 90
//#define MAXN_SERVO 5 __VARIABLE__
#define MAXN_SETPOSQUEUE 10 __VARIABLE__
#define NTIMES_PWM_SIGNAL 30 __VARIABLE__

class SERVO
{
public:
	SERVO();
	void Attach(uint8_t pin);
	void Detach();
	void SetPos(int pos);
	uint8_t GetPos();
	bool IsAttached();
	
private:
	uint8_t attached_pin;
	uint8_t pos;
	
	static SERVO* setpos_queue[MAXN_SETPOSQUEUE];
	static uint8_t queue_len;
	static void SendPWMFunc();
	static bool queue_top_servo_pwm_sta;
	static uint8_t queue_top_servo_progress;
};

#endif /* SERVO_H_ */
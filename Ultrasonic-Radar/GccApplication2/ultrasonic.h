/*
 * hcsr04.h
 * Description: ultrasonic module (HC_SR04)
 * Created: 2023/4/21 18:47:34
 *  Author: Bonjir
 */ 

#ifndef _ULTRASONIC_H_
#define _ULTRASONIC_H_
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <util/delay.h>
#include "pins.h"
#include "timer.h"

class ULTRASONIC 
{
public:
	ULTRASONIC();
	void Attach(uint8_t pEcho, uint8_t pTrig);
	uint16_t GetDistance();
	
private:
	uint8_t pEcho;
	uint8_t pTrig;
};

#endif /* _ULTRASONIC_H_ */
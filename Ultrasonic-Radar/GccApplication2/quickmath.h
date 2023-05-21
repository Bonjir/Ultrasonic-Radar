/*
 * quickmath.h
 *
 * Created: 2023/4/29 17:10:04
 * Author: Bonjir
 */ 


#ifndef QUICKMATH_H_
#define QUICKMATH_H_

#include <avr/eeprom.h>

#define STORE_IN_EEPROM // __SWITCH__  if closed then store in flash

float quick_sin(int deg);
float quick_cos(int deg);
float quick_tan(int deg);


#endif /* QUICKMATH_H_ */
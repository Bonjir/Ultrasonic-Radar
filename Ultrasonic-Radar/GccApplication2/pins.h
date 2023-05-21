/*
 * pins.h
 *
 * Created: 2023/4/29 15:59:05
 *  Author: Bonjir
 *    Note:	Only for ATMega8A
 */ 


#ifndef PINS_H_
#define PINS_H_

#include <avr/io.h>

typedef uint8_t PINS;

#define PINS_B0 0xB0
#define PINS_B1 0xB1
#define PINS_B2 0xB2
#define PINS_B3 0xB3
#define PINS_B4 0xB4
#define PINS_B5 0xB5
#define PINS_B6 0xB6
#define PINS_B7 0xB7
#define PINS_C0 0xC0
#define PINS_C1 0xC1
#define PINS_C2 0xC2
#define PINS_C3 0xC3
#define PINS_C4 0xC4
#define PINS_C5 0xC5
#define PINS_C6 0xC6
#define PINS_D0 0xD0
#define PINS_D1 0xD1
#define PINS_D2 0xD2
#define PINS_D3 0xD3
#define PINS_D4 0xD4
#define PINS_D5 0xD5
#define PINS_D6 0xD6
#define PINS_D7 0xD7

#define GETPIN_GROUP(pin) ((pin) >> 4)
#define GETPIN_NUMBERSHIFTED(pin) (1 << ((pin) & 0xf))

#define PINS_NULL	0xC7
#define PINS_ERR	0xFF

#define OUTPUT 1
#define INPUT 0

void SetPinMode(PINS pin, bool bMode);
void DigitalWrite(PINS pin, bool val);
bool DigitalRead(PINS pin);
bool IsPinValid(PINS pin);
bool DigitalRead_Fast(uint8_t pin_group, uint8_t pin_number_shifted);
void DigitalWrite_Fast(uint8_t pin_group, uint8_t pin_number_shifted, bool val);

#ifndef __CONST__
#define __CONST__
#endif
#ifndef __VARIABLE__
#define __VARIABLE__
#endif
#ifndef __SWITCH__
#define __SWITCH__
#endif
#ifndef NULL
#define NULL 0
#endif
#ifndef min
#define min(a, b) ((a) > (b) ? (b) : (a))
#endif
#ifndef max
#define max(a, b) ((a) < (b) ? (b) : (a))
#endif
#ifndef abs
#define abs(x) ((x) > 0 ? (x) : -(x))
#endif

#endif /* PINS_H_ */
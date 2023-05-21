/*
 * pin.cpp
 *
 * Created: 2023/4/29 16:20:36
 *  Author: Bonjir
 */ 

#include "pins.h"

void SetPinMode(PINS pin, bool bMode)
{
	if(!IsPinValid(pin))
		return;
		
	switch(pin >> 4)
	{
		case 0xB:
		{
			if(bMode) DDRB |= (1 << (pin & 0xf));
			else DDRB &= ~(1 << (pin & 0xf));
			break;
		}
		case 0xC:
		{
			if(bMode) DDRC |= (1 << (pin & 0xf));
			else DDRC &= ~(1 << (pin & 0xf));
			break;
		}
		case 0xD:
		{
			if(bMode) DDRD |= (1 << (pin & 0xf));
			else DDRD &= ~(1 << (pin & 0xf));
			break;
		}
	}
}

void DigitalWrite(PINS pin, bool val)
{
	if(!IsPinValid(pin))
		return;
	
	switch(pin >> 4)
	{
		case 0xB:
		{
			if(val) PORTB |= (1 << (pin & 0xf));
			else PORTB &= ~(1 << (pin & 0xf));
			break;
		}
		case 0xC:
		{
			if(val) PORTC |= (1 << (pin & 0xf));
			else PORTC &= ~(1 << (pin & 0xf));
			break;
		}
		case 0xD:
		{
			if(val) PORTD |= (1 << (pin & 0xf));
			else PORTD &= ~(1 << (pin & 0xf));
			break;
		}
	}
}

bool DigitalRead(PINS pin)
{
	if(!IsPinValid(pin))
		return 0;
	
	switch(pin >> 4)
	{
		case 0xB:
		{
			return (PINB & (1 << (pin & 0xf) ));
		}
		case 0xC:
		{
			return (PINC & (1 << (pin & 0xf) ));
		}
		case 0xD:
		{
			return (PIND & (1 << (pin & 0xf) ));
		}
		default: 
		{
			return 0;
		}
	}
}

bool IsPinValid(PINS pin)
{
	if( (pin >= PINS_B0 && pin <= PINS_B7) || (pin >= PINS_C0 && pin <= PINS_C6) || (pin >= PINS_D0 && pin <= PINS_D7) || pin == PINS_NULL)
		return true;
	return false;
}

void DigitalWrite_Fast(uint8_t pin_group, uint8_t pin_number_shifted, bool val)
{
	switch(pin_group)
	{
		case 0xB:
		{
			if(val) PORTB |= pin_number_shifted;
			else PORTB &= ~pin_number_shifted;
			break;
		}
		case 0xC:
		{
			if(val) PORTC |= pin_number_shifted;
			else PORTC &= ~pin_number_shifted;
			break;
		}
		case 0xD:
		{
			if(val) PORTD |= pin_number_shifted;
			else PORTD &= ~pin_number_shifted;
			break;
		}
	}
}

bool DigitalRead_Fast(uint8_t pin_group, uint8_t pin_number_shifted)
{
	switch(pin_group)
	{
		case 0xB:
		{
			return (PINB & pin_number_shifted);
		}
		case 0xC:
		{
			return (PINC & pin_number_shifted);
		}
		case 0xD:
		{
			return (PIND & pin_number_shifted);
		}
		default: return 0;
	}
}

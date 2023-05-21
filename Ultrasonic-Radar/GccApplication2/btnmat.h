/*
 * btnmat.h
 *
 * Created: 2023/5/16 20:16:01
 *  Author: Bonjir
 */ 


#ifndef BTNMAT_H_
#define BTNMAT_H_

#include "pins.h"

typedef class BUTTONMATRIX
{
public:
	void Attach(PINS pins[8]);
	uint8_t GetKeyDown();
	//bool IsKeyDown();
private:
	PINS pins[8];
} BTNMAT;


#endif /* BTNMAT_H_ */
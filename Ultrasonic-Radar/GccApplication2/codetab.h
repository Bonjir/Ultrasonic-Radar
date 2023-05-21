/*
 * codetab.h
 *
 * Created: 2023/4/29 20:16:30
 *  Author: Bonjir
 * Version: 3.0
 * 
 */ 


#ifndef CODETAB_H_
#define CODETAB_H_

#include <avr/eeprom.h>

#define USE_CODETAB
#define USE_SMALLCODETAB

#define P6X8CHAR 1
unsigned char GetP6x8ChDat(char ch, unsigned char col);


#endif /* CODETAB_H_ */

#include <avr/eeprom.h>
#include <stdlib.h>
#include <string.h>
#include "OLED_Paint.h"

//eeprom variable
uint8_t EEMEM NonVolatileChar = 1;
uint16_t EEMEM NonVolatileInt = 2;
uint8_t EEMEM NonVolatileString[10];

int main(void) {
	
	OP_Init();
	char buff[20] = { 0 };
	uint8_t SRAMchar;
	
	memset(buff, 0, 20);
	itoa(NonVolatileChar, buff, 10);
	OP_TextOut(0, 0, buff, 10, 1);
	memset(buff, 0, 20);
	
	// read byte
	SRAMchar = eeprom_read_byte( &NonVolatileChar );
	itoa(SRAMchar, buff, 10);
	OP_TextOut(0, 8, buff, 10, 1);
	memset(buff, 0, 20);
	
	// write byte
	SRAMchar = 10;
	eeprom_write_byte ( &NonVolatileChar, SRAMchar);
	
	// read byte
	SRAMchar = eeprom_read_byte( &NonVolatileChar );
	itoa(SRAMchar, buff, 10);
	OP_TextOut(0, 16, buff, 10, 1);
	memset(buff, 0, 20);
	/*
	// read str
	char StringOfData[10];
	eeprom_read_block((void*)&StringOfData, (const void*)12, 10);
	//itoa(SRAMchar, buff, 10);
	OP_TextOut(0, 8, StringOfData, 10, 1);
	
	// write byte
	SRAMchar = 10;
	eeprom_write_byte (&NonVolatileChar, SRAMchar);
	
	// write str
	uint8_t SRAMchars[11] = "HelloWorld";
	eeprom_write_block ((const void *)&NonVolatileString,(void *)12, 11);
	*/
	OP_Update();
	
	while(1);
}

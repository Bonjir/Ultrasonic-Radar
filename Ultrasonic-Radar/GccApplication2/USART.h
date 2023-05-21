/*
 * USART.h
 *
 * Created: 2023/5/13 19:13:49
 *  Author: Bonjir
 */ 


#ifndef USART_H_
#define USART_H_

#include <avr/io.h>

#define CBR_9600	9600
#define CBR_115200	115200

void USART_Init(uint16_t baudrate, uint32_t fosc);
void USART_WriteChar(char c);
void USART_WriteString(const char str[], int len);
bool USART_HaveDataInRAM();
char USART_ReadChar();

#endif /* USART_H_ */
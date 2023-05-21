/*
 * USART.cpp
 *
 * Created: 2023/5/13 19:14:15
 *  Author: Bonjir
 */ 

#define F_CPU 16000000UL

#include "USART.h"
#include <util/delay.h>


void USART_Init(uint16_t baudrate, uint32_t fosc)
{
	UCSRC = 0x80; //清空UCSRC
	UBRRL = 0;//
	UBRRL = fosc / 16 / baudrate - 1; // 103 : 9600bps ; Fosc = 16 MHz, BaudRate = 9600, UBBR = Fosc / 16 / BaudRate - 1 = 103
	UCSRB = (1 << RXEN) | (1 << TXEN);//开启USART接收和发送
	UCSRC = (1 << URSEL) | (3 << UCSZ0);//异步,无校验,8位数据， 1位停止位,...
}

void USART_WriteChar(char c)
{
	while (!(UCSRA & (1 << UDRE)));
	UDR = c;
}

void USART_WriteString(const char str[], int len)
{
	for(int i = 0; i < len; i ++)
	USART_WriteChar(str[i]);
}

bool USART_HaveDataInRAM()
{
	return (UCSRA & (1 << RXC));
}

char USART_ReadChar()
{
	char ch = UDR;
	_delay_ms(1);
	return ch;
}

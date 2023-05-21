/*
 * _test_for_usart2.cpp
 *
 * Created: 2023/5/13 13:18:02
 *  Author: Bonjir
 */ 
/*
 * serial_led_gui_upper_computer.cpp
 *
 * Created: 2023/5/12 12:59:40
 *  Author: Bonjir
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include "twi_lcd.h"
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>
#include "timer.h"

#define CBR_9600	9600
#define CBR_115200	115200

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

void TimerFunc()
{
	USART_WriteString("{[HBT]}", 7);
}

int main(void)
{
	uint8_t cmd_cnt = 0;
	char cmd_data[16]; //接收命令和数据
	USART_Init(CBR_9600, F_CPU);
	
	SetTimer(1000000, TimerFunc, 1024);
	AutoSetTimerInterval(1);
	
	DDRC |= (1 << DDRC0);
	
	TWI_Init();
	LCD_Init(); 
	
	while (1)
	{
		cmd_cnt = 0;
		memset(cmd_data, 0, sizeof(cmd_data));
		while (USART_HaveDataInRAM()) // have data in buffer
		{
			cmd_data[cmd_cnt] = USART_ReadChar(); 
			cmd_cnt ++;
			_delay_ms(1);
		}
		
		if (cmd_cnt)
		{
			LCD_Write_String(0, 0, "GetMsg");
			LCD_Write_String(1, 0, "          ");
			LCD_Write_String(1, 0, cmd_data);
			
			// deal with received message
			if(strcmp(cmd_data, "ON") == 0)
			{
				PORTC |= (1 << PORTC0);
			}
			if(strcmp(cmd_data, "OFF") == 0)
			{
				PORTC &= ~(1 << PORTC0);
			}
			
			// ACK
			USART_WriteString("ACK", 3);
		}
		
		_delay_ms(1);
	} 
} 

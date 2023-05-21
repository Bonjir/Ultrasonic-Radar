/*
 * GccApplication2.cpp
 *
 * Created: 2023/5/11 18:57:07
 * Author : Bonjir
 */
#define F_CPU 16000000UL

#include <avr/io.h>
#include "twi_lcd.h"
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
	uint8_t cmd_cnt = 0;
	char cmd_data[16]; //�������������
	UCSRC = 0x80; //���UCSRC
	UBRRL = 0;//
	UBRRL = 103; // 9600bps ; Fosc = 16 MHz, BaudRate = 9600, UBBR = Fosc / 16 / BaudRate - 1 = 103
	UCSRB = (1 << RXEN) | (1 << TXEN);//����USART���պͷ���
	UCSRC = (1 << URSEL) | (3 << UCSZ0);//�첽,��У��,8λ���ݣ� 1λֹͣλ,...
	
	DDRC |= (1 << DDRC0);
	
	TWI_Init();
	LCD_Init(); 
	
	PORTC ^= (1 << PORTC0);
	

	while (1)
	{
		cmd_cnt = 0;
		memset(cmd_data, 0, sizeof(cmd_data));
		while (UCSRA & (1 << RXC))//���յ�����
		{
			cmd_data[cmd_cnt] = UDR;//������
			cmd_cnt ++;
			_delay_ms(1);
		}
		
		if (cmd_cnt)
		{
			PORTC ^= (1 << PORTC0);
			
			LCD_Write_String(0, 0, "GetMsg");
			LCD_Write_String(1, 0, "          ");
			LCD_Write_String(1, 0, cmd_data);
			/*
			LCD_Write_Char(0, 12, cmd_data[1]);
			LCD_Write_Char(0, 13, cmd_data[2]);
			
			LCD_Write_String(1, 0, "   ");
			LCD_Write_String(1, 6, "   ");
			char szData[4];
			itoa(cmd_data[1], szData, 10);
			LCD_Write_String(1, 0, szData);
			itoa(cmd_data[2], szData, 10);
			LCD_Write_String(1, 6, szData);*/
			//LCD_Write_String(1, 0, (char *)cmd_data);
			
			/*switch (cmd_data[2])
			{//case 0xc0://LCDָ��ֱ�Ӳ���
			case 0x5c://proteus:\+����
				LCD_8Bit_Write(cmd_data[1], 0);
				break;
			case 0xc1://��æ��־
				break;
				//case 0xc2://��RAMд����
			case 0x5d://proteus ]+��ʾ�ַ�
				LCD_8Bit_Write(cmd_data[1], 1);
				break;
			case 0xc3://��RAM������
				break;
			}*/
			while (!(UCSRA & (1 << UDRE)));//�ȴ�����
				UDR = 'O'; //�����ַ�
			while (!(UCSRA & (1 << UDRE)));//�ȴ�����
				UDR = 'K'; //�����ַ�
		}
		_delay_ms(1);
	} //while����
} //main����

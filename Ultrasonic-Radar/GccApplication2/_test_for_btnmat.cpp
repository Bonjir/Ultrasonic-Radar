/*
 * _test_for_btnmat.cpp
 *
 * Created: 2023/5/16 11:22:02
 *  Author: Bonjir
 */ 

#define F_CPU 16000000UL
#include <util/delay.h>
#include <stdlib.h>
#include "pins.h"
#include "OLED_Paint.h"

unsigned char seg7_hex[17]={0xfc,0x60,0xda,0xf2,0x66,0xb6,0xbe,0xe0,
	0xfe,0xf6,0xee,0x3e,0x9c,0x7a,0x9e,0x8e,0x02};//4-7����
unsigned char seg7_com[4]={0xe,0xd,0x0b,0x07};//ɨ����

void Setup()
{
	OP_Init();
	
	SetPinMode(PINS_D5, OUTPUT);
	SetPinMode(PINS_D6, OUTPUT);
	SetPinMode(PINS_D7, OUTPUT);
	SetPinMode(PINS_B0, OUTPUT);
	SetPinMode(PINS_B1, INPUT);
	SetPinMode(PINS_B2, INPUT);
	SetPinMode(PINS_B3, INPUT);
	SetPinMode(PINS_B4, INPUT);
}

bool key[16];

int main(void)
{
	Setup();
	
	while (1)
	{
		_delay_us(100);
		OP_Clear();
		char num[6];
		uint16_t key_mask= 0;
		
		//ɨ���1��
		key_mask = 0;
		DigitalWrite(PINS_D5, 0);
		DigitalWrite(PINS_D6, 1);
		DigitalWrite(PINS_D7, 1);
		DigitalWrite(PINS_B0, 1);
		_delay_us(100);
		for(int i = PINS_B1; i <= PINS_B4; i ++)
		{
			if(DigitalRead(i) == 0)
			{
				key[0 * 4 + (i - PINS_B1)] = 1;
				key_mask |= (1 << ((i - PINS_B1)) << 0);
			}
		}
		
		memset(num, 0, sizeof(num));
		itoa(key_mask, num, 10);
		OP_TextOut(0, 0, num, strlen(num), 1);
		
		//ɨ���2��
		key_mask = 0;
		DigitalWrite(PINS_D5, 1);
		DigitalWrite(PINS_D6, 0);
		DigitalWrite(PINS_D7, 1);
		DigitalWrite(PINS_B0, 1);
		_delay_us(100);
		for(int i = PINS_B1; i <= PINS_B4; i ++)
		{
			if(DigitalRead(i) == 0)
			{
				key[1 * 4 + (i - PINS_B1)] = 1;
				key_mask |= (1 << (i - PINS_B1) << (1 * 4));
			}
		}
		
		memset(num, 0, sizeof(num));
		itoa(key_mask, num, 10);
		OP_TextOut(0, 8, num, strlen(num), 1);
		
		//ɨ���3��
		key_mask = 0;
		DigitalWrite(PINS_D5, 1);
		DigitalWrite(PINS_D6, 1);
		DigitalWrite(PINS_D7, 0);
		DigitalWrite(PINS_B0, 1);
		_delay_us(100);
		for(int i = PINS_B1; i <= PINS_B4; i ++)
		{
			if(DigitalRead(i) == 0)
			{
				key[2 * 4 + (i - PINS_B1)] = 1;
				key_mask |= (1 << (i - PINS_B1) << (2 * 4));
			}
		}
		
		memset(num, 0, sizeof(num));
		itoa(key_mask, num, 10);
		OP_TextOut(0, 16, num, strlen(num), 1);
		
		//ɨ���4��
		key_mask = 0;
		DigitalWrite(PINS_D5, 1);
		DigitalWrite(PINS_D6, 1);
		DigitalWrite(PINS_D7, 1);
		DigitalWrite(PINS_B0, 0);
		_delay_us(100);
		for(int i = PINS_B1; i <= PINS_B4; i ++)
		{
			if(DigitalRead(i) == 0)
			{
				key[3 * 4 + (i - PINS_B1)] = 1;
				key_mask |= (1 << (i - PINS_B1) << (3 * 4));
			}
		}
		
		memset(num, 0, sizeof(num));
		itoa(key_mask, num, 10);
		OP_TextOut(0, 24, num, strlen(num), 1);
		
		OP_Update();
		/*
		uint16_t key_mask = 0;
		for(int i = 0; i < 16; i ++)
		{
			key_mask |= (key[i] << i);
		}
		
		char num[6] = { 0, 0, 0, 0, 0, 0 };
		itoa(key_mask, num, 10);
		OP_Clear();
		OP_TextOut(0, 0, num, 6, 1);
		OP_TextOut(0, 8, "65535", 5, 1);
		OP_Update();
		*/
		/*
		//4.ɨ���4��
		PORTD = ~(1<<PORTD3);//PORTD3Ϊ�͵�ƽ��ɨ���4��
		_delay_us(2);
		getkey = (PIND & 0xf0)>>4;//��ȡ��״̬,���ƶ�����4λ
		switch(getkey)
		{
		case 0x0e:keyno =12;break;//4��1��ΪS12
		case 0x0d:keyno =13;break;//4��2��ΪS13
		case 0x0b:keyno =14;break;//4��3��ΪS14
		case 0x07:keyno =15;break;//4��4��ΪS15
		}
		
		if(keyno<16) / *ɨ��һ�ֲ������* /
		{
			seg7_no[0]=keyno; //�µİ�������
			bUpdated = 0;
		}
		if(keyno == 16 && bUpdated == 0)
		{
			seg7_no[4]=seg7_no[3]; //�ƶ���������
			seg7_no[3]=seg7_no[2];
			seg7_no[2]=seg7_no[1];
			seg7_no[1]=seg7_no[0];
			bUpdated = 1;
		}*/
	} //while ����
} //main��������
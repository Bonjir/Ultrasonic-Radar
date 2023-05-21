/*
 * _test_for_btnmap2.cpp
 *
 * Created: 2023/5/16 12:55:42
 *  Author: Bonjir
 */ 
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/io.h>
#include "OLED_Paint.h"
#include "pins.h"

int main(void)
{
	OP_Init();
	
	unsigned char seg7_no[5]={16,16,16,16, 16};//Ҫ��ʾ��4λ����
	unsigned char getkey,keyno;//ȡɨ�����룬�԰��°����ı���
	//DDRD = (0x0f);//��PD3~0���ɨ���뵽��(S_R3~0)������(S_C3~0)���뵽PD7~4
	for(int i = PINS_D0; i <= PINS_D3; i ++)
		SetPinMode(i, OUTPUT);
	for(int i = PINS_D4; i <= PINS_D7; i ++)
		SetPinMode(i, INPUT);
	for(int i = PINS_D0; i <= PINS_D7; i ++)
		DigitalWrite(i, 1);
	//PORTD = (0xff);//PD7~4���ڲ�������������,��PD3~0����ߵ�ƽ
	unsigned char bUpdated = 0;
	uint32_t key_mask = 0;
	
	while (1)
	{ 
		/*��������ɨ�裬�жϣ�����*/
		key_mask = 0;
		keyno =16;//Ĭ���ް�������
		
		//1.ɨ���1��
		for(int p = PINS_D0; p <= PINS_D7; p ++)
			DigitalWrite(p, 1);
		#define ReLU(x) (max(0, (x)))
		for(int row = 0; row < 4; row ++)
		{
			DigitalWrite(PINS_D0 + ReLU(row - 1), 1);
			DigitalWrite(PINS_D0 + row, 0);
			_delay_us(2);
			for(int i = 0; i < 4; i ++)
			{
				if(DigitalRead(PINS_D4 + i) == 0)
				{
					keyno = row * 4 + i;
					key_mask |= (1 << (row * 4 + i));
				}
			}
		}
		/*getkey = (PIND & 0xf0)>>4;//��ȡ��״̬,���ƶ�����4λ
		switch(getkey)
		{ 
			case 0x0e:keyno = 0;key_mask |= (1 << 0);break;//1��1��ΪS0
			case 0x0d:keyno = 1;key_mask |= (1 << 1);break;//1��2��ΪS1
			case 0x0b:keyno = 2;key_mask |= (1 << 2);break;//1��3��ΪS2
			case 0x07:keyno = 3;key_mask |= (1 << 3);break;//1��4��ΪS3
		}*/		/*
		//2.ɨ���2��

		PORTD = ~(1<<PORTD1);//PORTD1Ϊ�͵�ƽ��ɨ���2��

		_delay_us(2);

		getkey = (PIND & 0xf0)>>4;//��ȡ��״̬,���ƶ�����4λ

		switch(getkey)

		{ 

			case 0x0e:keyno = 4;key_mask |= (1 << 4);break;			//2��1��ΪS4

			case 0x0d:keyno = 5;key_mask |= (1 << 5);break;			//2��2��ΪS5

			case 0x0b:keyno = 6;key_mask |= (1 << 6);break;			//2��3��ΪS6

			case 0x07:keyno = 7;key_mask |= (1 << 7);break;			//2��4��ΪS7

		}

		

		//3.ɨ���3��

		PORTD = ~(1<<PORTD2);//ɨ���3��

		_delay_us(2);

		getkey = (PIND & 0xf0)>>4;//��ȡ��״̬

		switch(getkey)

		{ 

			case 0x0e:keyno = 8;key_mask |= (1 << 8);break;//3��1��ΪS8

			case 0x0d:keyno = 9;key_mask |= (1 << 9);break;//3��2��ΪS9

			case 0x0b:keyno = 10;key_mask |= (1 << 10);break;//��ΪS10

			case 0x07:keyno = 11;key_mask |= (1 << 11);break;//��ΪS11

		}

		

		//4.ɨ���4��

		PORTD = ~(1<<PORTD3);//PORTD3Ϊ�͵�ƽ��ɨ���4��

		_delay_us(2);

		getkey = (PIND & 0xf0)>>4;//��ȡ��״̬,���ƶ�����4λ

		switch(getkey)

		{ 

			case 0x0e:keyno =12;key_mask |= (1 << 12);break;//4��1��ΪS12

			case 0x0d:keyno =13;key_mask |= (1 << 13);break;//4��2��ΪS13

			case 0x0b:keyno =14;key_mask |= (1 << 14);break;//4��3��ΪS14

			case 0x07:keyno =15;key_mask |= (1 << 15);break;//4��4��ΪS15

		}

		*/
		if(keyno<16) /*ɨ��һ�ֲ������*/
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
		}
		
		OP_Clear();
		
		char num[4][6];
		memset(num, 0, sizeof(num));
		itoa(( key_mask >> 0 * 4 ) & 0xf, num[0], 2);
		itoa(( key_mask >> 1 * 4 ) & 0xf, num[1], 2);
		itoa(( key_mask >> 2 * 4 ) & 0xf, num[2], 2);
		itoa(( key_mask >> 3 * 4 ) & 0xf, num[3], 2);
		OP_TextOut(0, 0, num[0], strlen(num[0]), 1);
		OP_TextOut(0, 8, num[1], strlen(num[1]), 1);
		OP_TextOut(0, 16, num[2], strlen(num[2]), 1);
		OP_TextOut(0, 24, num[3], strlen(num[3]), 1);
		
// 		char num[4][6];
// 		memset(num, 0, sizeof(num));
// 		itoa(seg7_no[0], num[0], 10);
// 		itoa(seg7_no[1], num[1], 10);
// 		itoa(seg7_no[2], num[2], 10);
// 		itoa(seg7_no[3], num[3], 10);
// 		OP_TextOut(0, 0, num[0], strlen(num[0]), 1);
// 		OP_TextOut(0, 8, num[1], strlen(num[1]), 1);
// 		OP_TextOut(0, 16, num[2], strlen(num[2]), 1);
// 		OP_TextOut(0, 24, num[3], strlen(num[3]), 1);
		
		OP_Update();
		
	} //while ����
} //main��������
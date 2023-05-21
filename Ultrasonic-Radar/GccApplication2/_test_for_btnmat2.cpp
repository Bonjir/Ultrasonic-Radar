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
	
	unsigned char seg7_no[5]={16,16,16,16, 16};//要显示的4位数字
	unsigned char getkey,keyno;//取扫描输入，对按下按键的编码
	//DDRD = (0x0f);//经PD3~0输出扫描码到行(S_R3~0)，从列(S_C3~0)输入到PD7~4
	for(int i = PINS_D0; i <= PINS_D3; i ++)
		SetPinMode(i, OUTPUT);
	for(int i = PINS_D4; i <= PINS_D7; i ++)
		SetPinMode(i, INPUT);
	for(int i = PINS_D0; i <= PINS_D7; i ++)
		DigitalWrite(i, 1);
	//PORTD = (0xff);//PD7~4的内部上拉电阻启用,并PD3~0输出高电平
	unsigned char bUpdated = 0;
	uint32_t key_mask = 0;
	
	while (1)
	{ 
		/*按键阵列扫描，判断，编码*/
		key_mask = 0;
		keyno =16;//默认无按键按下
		
		//1.扫描第1行
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
		/*getkey = (PIND & 0xf0)>>4;//获取列状态,并移动到低4位
		switch(getkey)
		{ 
			case 0x0e:keyno = 0;key_mask |= (1 << 0);break;//1行1列为S0
			case 0x0d:keyno = 1;key_mask |= (1 << 1);break;//1行2列为S1
			case 0x0b:keyno = 2;key_mask |= (1 << 2);break;//1行3列为S2
			case 0x07:keyno = 3;key_mask |= (1 << 3);break;//1行4列为S3
		}*/		/*
		//2.扫描第2行

		PORTD = ~(1<<PORTD1);//PORTD1为低电平，扫描第2行

		_delay_us(2);

		getkey = (PIND & 0xf0)>>4;//获取列状态,并移动到低4位

		switch(getkey)

		{ 

			case 0x0e:keyno = 4;key_mask |= (1 << 4);break;			//2行1列为S4

			case 0x0d:keyno = 5;key_mask |= (1 << 5);break;			//2行2列为S5

			case 0x0b:keyno = 6;key_mask |= (1 << 6);break;			//2行3列为S6

			case 0x07:keyno = 7;key_mask |= (1 << 7);break;			//2行4列为S7

		}

		

		//3.扫描第3行

		PORTD = ~(1<<PORTD2);//扫描第3行

		_delay_us(2);

		getkey = (PIND & 0xf0)>>4;//获取列状态

		switch(getkey)

		{ 

			case 0x0e:keyno = 8;key_mask |= (1 << 8);break;//3行1列为S8

			case 0x0d:keyno = 9;key_mask |= (1 << 9);break;//3行2列为S9

			case 0x0b:keyno = 10;key_mask |= (1 << 10);break;//…为S10

			case 0x07:keyno = 11;key_mask |= (1 << 11);break;//…为S11

		}

		

		//4.扫描第4行

		PORTD = ~(1<<PORTD3);//PORTD3为低电平，扫描第4行

		_delay_us(2);

		getkey = (PIND & 0xf0)>>4;//获取列状态,并移动到低4位

		switch(getkey)

		{ 

			case 0x0e:keyno =12;key_mask |= (1 << 12);break;//4行1列为S12

			case 0x0d:keyno =13;key_mask |= (1 << 13);break;//4行2列为S13

			case 0x0b:keyno =14;key_mask |= (1 << 14);break;//4行3列为S14

			case 0x07:keyno =15;key_mask |= (1 << 15);break;//4行4列为S15

		}

		*/
		if(keyno<16) /*扫描一轮并编码后*/
		{
			seg7_no[0]=keyno; //新的按键数据
			bUpdated = 0;
		}
		if(keyno == 16 && bUpdated == 0)
		{
			seg7_no[4]=seg7_no[3]; //移动按键数据
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
		
	} //while 结束
} //main函数结束
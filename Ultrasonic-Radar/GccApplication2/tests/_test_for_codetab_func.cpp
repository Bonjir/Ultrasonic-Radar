/*
 * test_for_codetab_func.c
 *
 * Created: 2023/4/23 20:55:38
 *  Author: 86198
 */

#include <avr/io.h>
#include "OLED12864.h"

void OLED_P6x8Str(unsigned char col, unsigned char page,char ch[])
{
	unsigned char i=0,j=0;
	while (ch[j]!='\0')
	{
		if(col>126)
		{
			col=0;
			page++;
		}
		OLED_Set_Pos(col,page);
		for(i=0;i<6;i++)
		OLED_WrDat(GetP6x8ChDat(ch[j], i));
		col+=6;
		j++;
	}
}

int main(void)
{
	OLED_Init(); //OLED��ʼ��
	while(1)
	{
		OLED_Fill(0xff);
		/*_delay_ms(1000);*/
		//OLED_Fill(0x00); //��ȫ��
		//_delay_ms(200);
		
		/*OLED_P16x16Ch(24,0,1);
		OLED_P16x16Ch(40,0,2);
		OLED_P16x16Ch(57,0,3);
		OLED_P16x16Ch(74,0,4);
		OLED_P16x16Ch(91,0,5);
		for(i=0; i<8; i++)//ͨ��������ʾ���� -- i��ʾ�ֱ������λ��
		{
			//			OLED_P16x16Ch(i*16,0,i);
			OLED_P16x16Ch(i*16,2,i+8);
			OLED_P16x16Ch(i*16,4,i+16);
			OLED_P16x16Ch(i*16,6,i+24);
		}
		_delay_ms(4000);
		OLED_Clear();//����
*/

		OLED_P6x8Str(0,0,"WELCOME");//��һ�� -- 8x16����ʾ��Ԫ��ʾASCII��
		OLED_P6x8Str(1,2,"WELCOME");
		OLED_P6x8Str(2,4,"WELCOME");
		OLED_P6x8Str(3,6,"HELLO WORLD!");
		/*OLED_P8x16Str(0,0,"WELCOME");//��һ�� -- 8x16����ʾ��Ԫ��ʾASCII��
		OLED_P8x16Str(1,2,"WELCOME");
		OLED_P8x16Str(2,4,"WELCOME");
		OLED_P8x16Str(3,6,"HELLO WORLD!");*/
		/*OLED_P6x8Str(0,6,"cn.heltec@gmail.com");
		OLED_P6x8Str(0,7,"heltec.taobao.com");*/
		_delay_ms(2000);
		OLED_Clear();
		
		OLED_Set_Pos(0, 1);
		for(unsigned char i = 0; i < 8; i ++)
		{
			OLED_WrDat(1 << i);
		}
		OLED_Set_Pos(0, 0);
		
		for(unsigned char i = 0; i < 8; i ++)
		{
			OLED_WrDat(1 << (7 - i));
		}
		OLED_Set_Pos(0, 2);
		for(unsigned char i = 0; i < 8; i ++)
		{
			OLED_WrDat((1 << (i + 1)) - 1);
		}
		_delay_ms(4000);
		OLED_Clear();
		/*OLED_Draw_BMP(0,0,128,8,BMP1);  //ͼƬ��ʾ(ͼƬ��ʾ���ã����ɵ��ֱ�ϴ󣬻�ռ�ý϶�ռ䣬FLASH�ռ�8K��������)
		_delay_ms(8000);
		OLED_Draw_BMP(0,0,128,8,BMP2);
		_delay_ms(8000);*/
	}
}
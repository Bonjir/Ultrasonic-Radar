/************************************************************************************
*
* File name: OLED12864.h
* Project  : OLED
* Processor: Atmega8A
* Compiler : Atmel Studio
*
* Author : Bonjir
* Version: 1.00
* Date   : 2023.4.21
*
* Description: 128*64点整OLED模块驱动文件
*
* Others: none;
*
* Function List:
*
* 1. void OLED_Init(void) -- OLED屏初始化程序，此函数应在操作屏幕之前最先调用
* 2. void OLED_WrDat(unsigned char IIC_Data) -- 向OLED屏写数据
* 3. void OLED_WrCmd(unsigned char cmd) -- 向OLED屏写命令
* 4. void OLED_Set_Pos(unsigned char x, unsigned char y) -- 设置显示坐标
* 5. void OLED_Fill(unsigned char bmp_dat) -- 全屏显示(显示BMP图片时才会用到此功能)
* 6. void OLED_CLS(void) -- 复位/清屏
* 7. void OLED_P6x8Str(unsigned char x, y,unsigned char ch[]) -- 6x8点整，用于显示ASCII码的最小阵列，不太清晰
* 8. void OLED_P8x16Str(unsigned char x, y,unsigned char ch[]) -- 8x16点整，用于显示ASCII码，非常清晰
* 9. void OLED_P16x16Ch(unsigned char x, y, N) -- 16x16点整，用于显示汉字的最小阵列，可设置各种字体、加粗、倾斜、下划线等
* 10. void OLED_Draw_BMP(unsigned char x0, y0,x1, y1,unsigned char BMP[]) -- 将128x64像素的BMP位图在取字软件中算出字表，然后复制到codetab中，此函数调用即可
*
*
* History: none;
*
*************************************************************************************/

#define F_CPU 1000000UL

#include <util/delay.h>
#include "twi_fun.h"
#include "codetab.h"

#define	Brightness	0xCF
#define X_WIDTH 	128
#define Y_WIDTH 	64

/*********************OLED写数据************************************/
void OLED_WrDat(unsigned char IIC_Data)
{
	TWI_Start();
	TWI_Write(0x78);
	TWI_Write(0x40);			//write data
	TWI_Write(IIC_Data);
	TWI_Stop();
}

/*********************OLED写命令************************************/
void OLED_WrCmd(unsigned char IIC_Command)
{
	TWI_Start();
	TWI_Write(0x78);            //Slave address,SA0=0
	TWI_Write(0x00);			//write command
	TWI_Write(IIC_Command);
	TWI_Stop();
}

/*********************OLED 设置坐标************************************/
void OLED_Set_Pos(unsigned char x, unsigned char y)
{
	OLED_WrCmd(0xb0+y);
	OLED_WrCmd(((x&0xf0)>>4)|0x10);
	OLED_WrCmd((x&0x0f)|0x01);
}

/*********************OLED全屏************************************/
void OLED_Fill(unsigned char bmp_dat)
{
	unsigned char y,x;
	for(y=0;y<8;y++)
	{
		OLED_WrCmd(0xb0+y);
		OLED_WrCmd(0x01);
		OLED_WrCmd(0x10);
		for(x=0;x<X_WIDTH;x++)
			OLED_WrDat(bmp_dat);
	}
}

/*********************OLED复位************************************/
void OLED_CLS(void)
{
	unsigned char y,x;
	for(y=0;y<8;y++)
	{
		OLED_WrCmd(0xb0+y);
		OLED_WrCmd(0x01);
		OLED_WrCmd(0x10);
		for(x=0;x<X_WIDTH;x++)
			OLED_WrDat(0);
	}
}

/*********************OLED初始化************************************/
void OLED_Init(void)
{
	_delay_ms(500);//初始化之前的延时很重要！
	OLED_WrCmd(0xae);//--turn off oled panel
	OLED_WrCmd(0x00);//---set low column address
	OLED_WrCmd(0x10);//---set high column address
	OLED_WrCmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WrCmd(0x81);//--set contrast control register
	OLED_WrCmd(Brightness); // Set SEG Output Current Brightness
	OLED_WrCmd(0xa1);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	//OLED_WrCmd(0xc8);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	OLED_WrCmd(0xa6);//--set normal display
	OLED_WrCmd(0xa8);//--set multiplex ratio(1 to 64)
	OLED_WrCmd(0x3f);//--1/64 duty
	OLED_WrCmd(0xd3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WrCmd(0x00);//-not offset
	OLED_WrCmd(0xd5);//--set display clock divide ratio/oscillator frequency
	OLED_WrCmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WrCmd(0xd9);//--set pre-charge period
	OLED_WrCmd(0xf1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WrCmd(0xda);//--set com pins hardware configuration
	OLED_WrCmd(0x12);
	OLED_WrCmd(0xdb);//--set vcomh
	OLED_WrCmd(0x40);//Set VCOM Deselect Level
	//OLED_WrCmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
	//OLED_WrCmd(0x02);//
	OLED_WrCmd(0x8d);//--set Charge Pump enable/disable
	OLED_WrCmd(0x14);//--set(0x10) disable
	//OLED_WrCmd(0xa4);// Disable Entire Display On (0xa4/0xa5)
	//OLED_WrCmd(0xa6);// Disable Inverse Display On (0xa6/a7)
	OLED_WrCmd(0xaf);//--turn on oled panel
	OLED_Fill(0x00); //初始清屏
	OLED_Set_Pos(0,0);
}
/*
/ ***************功能描述：显示6*8一组标准ASCII字符串	显示的坐标（x,y），y为页范围0～7**************** /
void OLED_P6x8Str(unsigned char x, unsigned char y,char ch[])
{
	unsigned char c=0,i=0,j=0;
	while (ch[j]!='\0')
	{
		c =ch[j]-32;
		if(x>126)
		{
			x=0;
			y++;
		}
		OLED_Set_Pos(x,y);
		for(i=0;i<6;i++)
			OLED_WrDat(F6x8[c][i]);
		x+=6;
		j++;
	}
}*/

/*******************功能描述：显示8*16一组标准ASCII字符串	 显示的坐标（x,y），y为页范围0～7****************/
void OLED_P8x16Str(unsigned char x, unsigned char y,char ch[])
{
	unsigned char c=0,i=0,j=0;
	while (ch[j]!='\0')
	{
		c =ch[j]-32;
		if(x>120)
		{
			x=0;
			y++;
		}
		OLED_Set_Pos(x, y);
		for(i=0;i<8;i++)
			OLED_WrDat(F8X16[c*16+i]);
		OLED_Set_Pos(x, y+1);
		for(i=0;i<8;i++)
			OLED_WrDat(F8X16[c*16+i+8]);
		x+=8;
		j++;
	}
}
/*

/ *****************功能描述：显示16*16点阵  显示的坐标（x,y），y为页范围0～7**************************** /
void OLED_P16x16Ch(unsigned char x, unsigned char y, unsigned char N)
{
	unsigned char wm=0;
	unsigned int adder=32*N;
	OLED_Set_Pos(x, y);
	for(wm = 0;wm < 16;wm++)
	{
		OLED_WrDat(F16x16[adder]);
		adder += 1;
	}
	OLED_Set_Pos(x,y + 1);
	for(wm = 0;wm < 16;wm++)
	{
		OLED_WrDat(F16x16[adder]);
		adder += 1;
	}
}*/
/*

/ ***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7***************** /
void OLED_Draw_BMP(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1,unsigned char BMP[])
{
	unsigned int j=0;
	unsigned char x,y;

	if(y1%8==0) 
		y=y1/8;
	else 
		y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
		for(x=x0;x<x1;x++)
		{
			OLED_WrDat(BMP[j++]);
		}
	}
}*/
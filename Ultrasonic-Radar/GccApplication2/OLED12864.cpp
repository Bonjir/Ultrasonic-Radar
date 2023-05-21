/*
 * OLED12864.c
 *
 * Created: 2023/4/22 21:41:43
 *  Author: Bonjir
 */ 

#include "OLED12864.h"

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
void OLED_Set_Pos(unsigned char col, unsigned char page)
{
	page = page % 8;
	col = col % 128;
	OLED_WrCmd(0xb0 + page); //设置起始页地址
	//OLED_WrCmd(((col & 0xf0) >> 4) | 0x10); //设置起始列高半字节
	OLED_WrCmd(((col & 0xf0) >> 4) | 0x10); //设置起始列高半字节
	OLED_WrCmd((col & 0x0f) | 0x00);	//设置起始列低半字节 // VERY ESSENTIAL 非常重要，不要 |0x01， 否则col会以2个像素为单位
}

/*********************OLED全屏************************************/
void OLED_Fill(unsigned char bmp_dat)
{
	unsigned char page, col;
	for (page = 0; page < 8; page++)
	{
		OLED_WrCmd(0xb0 + page); //设置页地址（0~7）
		OLED_WrCmd(0x00); //设置显示位置—列低地址
		OLED_WrCmd(0x10); //设置显示位置—列高地址
		for (col = 0; col < X_WIDTH; col++)
			OLED_WrDat(bmp_dat); // TODO draw pixel
	}
}

/*********************OLED复位************************************/
void OLED_Clear(void)
{
	unsigned char page, col;
	for (page = 0; page < 8; page++)
	{
		OLED_WrCmd(0xb0 + page);
		OLED_WrCmd(0x00);
		OLED_WrCmd(0x10);
		for (col = 0; col <= X_WIDTH; col++)
			OLED_WrDat(0);
	}
}

void OLED_DeepClear(void)
{
	unsigned char page, col;
	OLED_Clear();
	for (page = 0; page < 8; page++)
	{
		OLED_WrCmd(0xb0 + page);
		OLED_WrCmd(0x00);
		OLED_WrCmd(0x11);
		for (col = 0; col <= X_WIDTH; col++)
			OLED_WrDat(0);
	}
}

/*********************OLED初始化************************************/
void OLED_Init(void)
{
	TWI_Init();
	_delay_ms(500);//初始化之前的延时很重要！
	OLED_WrCmd(0xae);//--turn off oled panel
	OLED_WrCmd(0x00);//---set low column address 
	OLED_WrCmd(0x10);//---set high column address
	OLED_WrCmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WrCmd(0x81);//--set contrast control register
	OLED_WrCmd(Brightness); // Set SEG Output Current Brightness
	OLED_WrCmd(0xa1);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	OLED_WrCmd(0xc8);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
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
	OLED_WrCmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WrCmd(0x02);// 0x00 Page Address 0x01 Horizontal Address 0x02 Vertical address
	/*
	OLED_WrCmd(0x21);//-Set Column Start Address and Column End Address
	OLED_WrCmd(0x02);// DEBUG
	OLED_WrCmd(0x12);//
	*/
	OLED_WrCmd(0x8d);//--set Charge Pump enable/disable
	OLED_WrCmd(0x14);//--set(0x10) disable
	//OLED_WrCmd(0xa4);// Disable Entire Display On (0xa4/0xa5)
	//OLED_WrCmd(0xa6);// Disable Inverse Display On (0xa6/a7)
	OLED_WrCmd(0xaf);//--turn on oled panel
	
	OLED_DeepClear();
	OLED_Set_Pos(0, 0);
}

#if P6X8CHAR

/***************功能描述：显示6*8一组标准ASCII字符串	显示的坐标（col,page），page为页范围0～7****************/
void OLED_P6x8Ch(unsigned char col, unsigned char page,char ch)
{
	unsigned char i=0,j=0;
	if(col>126)
	{
		col=0;
		page++;
	}
	OLED_Set_Pos(col,page);
	for(i=0;i<6;i++)
		OLED_WrDat(GetP6x8ChDat(ch, i));
	col+=6;
	j++;
}

/***************功能描述：显示6*8一组标准ASCII字符串	显示的坐标（col,page），page为页范围0～7****************/
void OLED_P6x8Str(unsigned char col, unsigned char page,char ch[])
{
	unsigned char i=0,j=0;
	OLED_Set_Pos(col,page);
	
	while (ch[j]!='\0')
	{
		if(col>126)
		{
			col=0;
			page++;
			OLED_Set_Pos(col,page);
		}
		for(i=0;i<6;i++)
			OLED_WrDat(GetP6x8ChDat(ch[j], i));
		col+=6;
		j++;
	}
}

#endif 

#if P8X16CHAR

/*******************功能描述：显示8*16一组标准ASCII字符串	 显示的坐标（col,page），page为页范围0～7****************/
void OLED_P8x16Ch(unsigned char col, unsigned char page, char ch)
{
	unsigned char i = 0, j = 0;
	ch -= 32;
	if (col > 120)
	{
		col = 0;
		page += 2;
	}
	OLED_Set_Pos(col, page);
	for (i = 0; i < 8; i++)
		OLED_WrDat(F8X16[ch * 16 + i]);
	OLED_Set_Pos(col, page + 1);
	for (i = 0; i < 8; i++)
		OLED_WrDat(F8X16[ch * 16 + i + 8]);
	col += 8;
	j++;
}


/*******************功能描述：显示8*16一组标准ASCII字符串	 显示的坐标（col,page），page为页范围0～7****************/
void OLED_P8x16Str(unsigned char col, unsigned char page, char ch[])
{
	unsigned char c = 0, i = 0, j = 0;
	while (ch[j] != '\0')
	{
		c = ch[j] - 32;
		if (col > 120)
		{
			col = 0;
			page += 2;
		}
		OLED_Set_Pos(col, page);
		for (i = 0; i < 8; i++)
		OLED_WrDat(F8X16[c * 16 + i]);
		OLED_Set_Pos(col, page + 1);
		for (i = 0; i < 8; i++)
		OLED_WrDat(F8X16[c * 16 + i + 8]);
		col += 8;
		j++;
	}
}
 
#endif 

/*

/ *****************功能描述：显示16*16点阵  显示的坐标（col,page），page为页范围0～7**************************** /
void OLED_P16x16Ch(unsigned char col, unsigned char page, unsigned char N)
{
	unsigned char wm=0;
	unsigned int adder=32*N;
	OLED_Set_Pos(col, page);
	for(wm = 0;wm < 16;wm++)
	{
		OLED_WrDat(F16x16[adder]);
		adder += 1;
	}
	OLED_Set_Pos(col,page + 1);
	for(wm = 0;wm < 16;wm++)
	{
		OLED_WrDat(F16x16[adder]);
		adder += 1;
	}
}*/
/*

/ ***********功能描述：显示显示BMP图片128×64起始点坐标(col,page),col的范围0～127，page为页的范围0～7***************** /
void OLED_Draw_BMP(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1,unsigned char BMP[])
{
	unsigned int j=0;
	unsigned char col,page;

	if(y1%8==0)
		page=y1/8;
	else
		page=y1/8+1;
	for(page=y0;page<y1;page++)
	{
		OLED_Set_Pos(x0,page);
		for(col=x0;col<x1;col++)
		{
			OLED_WrDat(BMP[j++]);
		}
	}
}*/

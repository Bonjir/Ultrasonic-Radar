/*
 * OLED12864.c
 *
 * Created: 2023/4/22 21:41:43
 *  Author: Bonjir
 */ 

#include "OLED12864.h"

/*********************OLEDд����************************************/
void OLED_WrDat(unsigned char IIC_Data)
{
	TWI_Start();
	TWI_Write(0x78);
	TWI_Write(0x40);			//write data
	TWI_Write(IIC_Data);
	TWI_Stop();
}

/*********************OLEDд����************************************/
void OLED_WrCmd(unsigned char IIC_Command)
{
	TWI_Start();
	TWI_Write(0x78);            //Slave address,SA0=0
	TWI_Write(0x00);			//write command
	TWI_Write(IIC_Command);
	TWI_Stop();
}

/*********************OLED ��������************************************/
void OLED_Set_Pos(unsigned char col, unsigned char page)
{
	page = page % 8;
	col = col % 128;
	OLED_WrCmd(0xb0 + page); //������ʼҳ��ַ
	//OLED_WrCmd(((col & 0xf0) >> 4) | 0x10); //������ʼ�и߰��ֽ�
	OLED_WrCmd(((col & 0xf0) >> 4) | 0x10); //������ʼ�и߰��ֽ�
	OLED_WrCmd((col & 0x0f) | 0x00);	//������ʼ�еͰ��ֽ� // VERY ESSENTIAL �ǳ���Ҫ����Ҫ |0x01�� ����col����2������Ϊ��λ
}

/*********************OLEDȫ��************************************/
void OLED_Fill(unsigned char bmp_dat)
{
	unsigned char page, col;
	for (page = 0; page < 8; page++)
	{
		OLED_WrCmd(0xb0 + page); //����ҳ��ַ��0~7��
		OLED_WrCmd(0x00); //������ʾλ�á��е͵�ַ
		OLED_WrCmd(0x10); //������ʾλ�á��иߵ�ַ
		for (col = 0; col < X_WIDTH; col++)
			OLED_WrDat(bmp_dat); // TODO draw pixel
	}
}

/*********************OLED��λ************************************/
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

/*********************OLED��ʼ��************************************/
void OLED_Init(void)
{
	TWI_Init();
	_delay_ms(500);//��ʼ��֮ǰ����ʱ����Ҫ��
	OLED_WrCmd(0xae);//--turn off oled panel
	OLED_WrCmd(0x00);//---set low column address 
	OLED_WrCmd(0x10);//---set high column address
	OLED_WrCmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WrCmd(0x81);//--set contrast control register
	OLED_WrCmd(Brightness); // Set SEG Output Current Brightness
	OLED_WrCmd(0xa1);//--Set SEG/Column Mapping     0xa0���ҷ��� 0xa1����
	OLED_WrCmd(0xc8);//Set COM/Row Scan Direction   0xc0���·��� 0xc8����
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

/***************������������ʾ6*8һ���׼ASCII�ַ���	��ʾ�����꣨col,page����pageΪҳ��Χ0��7****************/
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

/***************������������ʾ6*8һ���׼ASCII�ַ���	��ʾ�����꣨col,page����pageΪҳ��Χ0��7****************/
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

/*******************������������ʾ8*16һ���׼ASCII�ַ���	 ��ʾ�����꣨col,page����pageΪҳ��Χ0��7****************/
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


/*******************������������ʾ8*16һ���׼ASCII�ַ���	 ��ʾ�����꣨col,page����pageΪҳ��Χ0��7****************/
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

/ *****************������������ʾ16*16����  ��ʾ�����꣨col,page����pageΪҳ��Χ0��7**************************** /
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

/ ***********������������ʾ��ʾBMPͼƬ128��64��ʼ������(col,page),col�ķ�Χ0��127��pageΪҳ�ķ�Χ0��7***************** /
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

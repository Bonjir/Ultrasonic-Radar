/*
 * twi_lcd.cpp
 *
 * Created: 2023/5/6 22:36:37
 *  Author: Bonjir
 */ 
#include "twi_lcd.h"

unsigned char TWI_Write_LCD(unsigned char uc_data)
{
	TWI_Start();//发送START信号
	if(TWI_Get_State_Info()!=TW_START) return 0;//不成功
	TWI_Write(LCD_SLAVE_ADDRESS<<1|TW_WRITE); //发送SLA+W
	if(TWI_Get_State_Info()!=TW_MT_SLA_ACK)return 0;//不成功
	TWI_Write(uc_data|LCD_BACKLIGHTON);//发送数据+背光常开
	if(TWI_Get_State_Info()!=TW_MT_DATA_ACK)return 0;//不成功
	TWI_Stop();
	return 1;//成功
}

void LCD_4Bit_Write(unsigned char uc_data)//4位方式写PCF8574
{
	TWI_Write_LCD(uc_data);//数据送出，E=0
	_delay_us(1);//保持
	TWI_Write_LCD(uc_data|LCD_E);//数据送出，E=1
	_delay_us(1);//保持
	TWI_Write_LCD(uc_data & (~LCD_E));//数据送出，E=0
	_delay_us(50);//等待数据传输结束
}
void LCD_8Bit_Write(unsigned char uc_data,unsigned char uc_mode)
//2次4位数据传输方式写PCF9574,uc_mode:0-命令,1-数据
{
	unsigned char high4bit = uc_data & 0xf0;
	unsigned char low4bit = (uc_data<<4)&0xf0;
	LCD_4Bit_Write(high4bit|uc_mode);//先发送高4位
	LCD_4Bit_Write(low4bit|uc_mode);//再发送低4位
}

void LCD_Init()//初始化LCD1602
{
	TWI_Init();
	_delay_ms(50);//上电后至少再等40ms
	LCD_4Bit_Write(0x30); //在默认8位接口，试着进入4位接口模式
	_delay_us(4500);//等待至少4.5ms
	LCD_4Bit_Write(0x30);
	_delay_us(4500);//等待至少4.5ms
	LCD_4Bit_Write(0x30);
	_delay_us(150);//等待至少150us
	LCD_4Bit_Write(0x20);//进入4位接口模式
	//设置模式，显示，点数等
	LCD_8Bit_Write(LCD_FUNCTIONSET|LCD_4BITMODE|LCD_2LINE|LCD_5X8DOTS,
	0);
	LCD_8Bit_Write(LCD_DISPLAYCONTROL|LCD_DISPLAYON,0);//显示
	LCD_8Bit_Write(LCD_CLEARDISPLAY,0);
	_delay_us(2000);//等待
	LCD_8Bit_Write(LCD_ENTRYMODESET|LCD_ENTRYINC,0);//显示左移(递增)
	LCD_8Bit_Write(LCD_RETURNHOME,0);//返回原点
	_delay_us(2000);//等待
}

void LCD_Set_Cursor_Location(unsigned char row,unsigned char col)
//设置光标位置,row:0~1,col:0~39
{
	unsigned char offset[]={0x0,0x40};
	LCD_8Bit_Write(LCD_SETDDRAMADDR|(col+offset[row]),0);
}
void LCD_Write_NewChar(char c_data)//在当前位置显示
{
	LCD_8Bit_Write(c_data,1);
}
void LCD_Write_Char(unsigned char row,unsigned char col,char c_data)
//在指定位置显示
{
	LCD_Set_Cursor_Location(row,col);
	LCD_8Bit_Write(c_data,1);
}

void LCD_Write_String(unsigned char row,unsigned char col,const char *pStr)
//在指定位置显示串
{
	LCD_Set_Cursor_Location(row,col);
	while((*pStr) != '\0')
	{
		LCD_8Bit_Write(*pStr,1);
		pStr ++;
	}
}
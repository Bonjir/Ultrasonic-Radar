/*
 * twi_lcd.cpp
 *
 * Created: 2023/5/6 22:36:37
 *  Author: Bonjir
 */ 
#include "twi_lcd.h"

unsigned char TWI_Write_LCD(unsigned char uc_data)
{
	TWI_Start();//����START�ź�
	if(TWI_Get_State_Info()!=TW_START) return 0;//���ɹ�
	TWI_Write(LCD_SLAVE_ADDRESS<<1|TW_WRITE); //����SLA+W
	if(TWI_Get_State_Info()!=TW_MT_SLA_ACK)return 0;//���ɹ�
	TWI_Write(uc_data|LCD_BACKLIGHTON);//��������+���ⳣ��
	if(TWI_Get_State_Info()!=TW_MT_DATA_ACK)return 0;//���ɹ�
	TWI_Stop();
	return 1;//�ɹ�
}

void LCD_4Bit_Write(unsigned char uc_data)//4λ��ʽдPCF8574
{
	TWI_Write_LCD(uc_data);//�����ͳ���E=0
	_delay_us(1);//����
	TWI_Write_LCD(uc_data|LCD_E);//�����ͳ���E=1
	_delay_us(1);//����
	TWI_Write_LCD(uc_data & (~LCD_E));//�����ͳ���E=0
	_delay_us(50);//�ȴ����ݴ������
}
void LCD_8Bit_Write(unsigned char uc_data,unsigned char uc_mode)
//2��4λ���ݴ��䷽ʽдPCF9574,uc_mode:0-����,1-����
{
	unsigned char high4bit = uc_data & 0xf0;
	unsigned char low4bit = (uc_data<<4)&0xf0;
	LCD_4Bit_Write(high4bit|uc_mode);//�ȷ��͸�4λ
	LCD_4Bit_Write(low4bit|uc_mode);//�ٷ��͵�4λ
}

void LCD_Init()//��ʼ��LCD1602
{
	TWI_Init();
	_delay_ms(50);//�ϵ�������ٵ�40ms
	LCD_4Bit_Write(0x30); //��Ĭ��8λ�ӿڣ����Ž���4λ�ӿ�ģʽ
	_delay_us(4500);//�ȴ�����4.5ms
	LCD_4Bit_Write(0x30);
	_delay_us(4500);//�ȴ�����4.5ms
	LCD_4Bit_Write(0x30);
	_delay_us(150);//�ȴ�����150us
	LCD_4Bit_Write(0x20);//����4λ�ӿ�ģʽ
	//����ģʽ����ʾ��������
	LCD_8Bit_Write(LCD_FUNCTIONSET|LCD_4BITMODE|LCD_2LINE|LCD_5X8DOTS,
	0);
	LCD_8Bit_Write(LCD_DISPLAYCONTROL|LCD_DISPLAYON,0);//��ʾ
	LCD_8Bit_Write(LCD_CLEARDISPLAY,0);
	_delay_us(2000);//�ȴ�
	LCD_8Bit_Write(LCD_ENTRYMODESET|LCD_ENTRYINC,0);//��ʾ����(����)
	LCD_8Bit_Write(LCD_RETURNHOME,0);//����ԭ��
	_delay_us(2000);//�ȴ�
}

void LCD_Set_Cursor_Location(unsigned char row,unsigned char col)
//���ù��λ��,row:0~1,col:0~39
{
	unsigned char offset[]={0x0,0x40};
	LCD_8Bit_Write(LCD_SETDDRAMADDR|(col+offset[row]),0);
}
void LCD_Write_NewChar(char c_data)//�ڵ�ǰλ����ʾ
{
	LCD_8Bit_Write(c_data,1);
}
void LCD_Write_Char(unsigned char row,unsigned char col,char c_data)
//��ָ��λ����ʾ
{
	LCD_Set_Cursor_Location(row,col);
	LCD_8Bit_Write(c_data,1);
}

void LCD_Write_String(unsigned char row,unsigned char col,const char *pStr)
//��ָ��λ����ʾ��
{
	LCD_Set_Cursor_Location(row,col);
	while((*pStr) != '\0')
	{
		LCD_8Bit_Write(*pStr,1);
		pStr ++;
	}
}
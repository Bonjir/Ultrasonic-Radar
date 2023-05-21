/*
 * twi_lcd.h
 *
 *   Note:	LCD 1602
 */ 

#ifndef _TWI_LCD_H_
#define _TWI_LCD_H_

#ifndef F_CPU
#define F_CPU 16000000UL //延时用
#endif
#include "twi_fun.h"
#include <util/delay.h>

//LCD1602 控制和显示指令
#define LCD_CLEARDISPLAY 0x01//清屏，设置AC为DDRAM地址0
#define LCD_RETURNHOME 0x02//设置AC为DDRAM地址0，光标回原点
#define LCD_ENTRYMODESET 0x04//与I/D和S位定义光标移动方向和显示移位
#define LCD_DISPLAYCONTROL 0x08//与D/C/B 设置显示开关，光标开关/闪烁
#define LCD_CURSORSHIFT 0x10//与S/C和R/L 一起设置光标移动或显示移位
#define LCD_FUNCTIONSET 0x20//与DL、N和F一起设置LCD功能:8/4位数据;1/2行显示;5*8/10点阵字符
#define LCD_SETCGRAMADDR 0x40//设置CGRAM地址到地址计数器(AC）
#define LCD_SETDDRAMADDR 0x80//设置DDRAM地址到地址计数器(AC）

// LCD进入模式设置位(LCD_ENTRYMODESET=0x04)
#define LCD_ENTRYSHIFT 0x01// S 位=1，显示移位，=0不移位
#define LCD_ENTRYINC 0x02//I/ D 位=1，显示左移(递增)
//LCD显示开关控制位 (LCD_DISPLAYCONTROL=0x08)
#define LCD_BLINKON 0x01//B=1，闪烁
#define LCD_CURSORON 0x02//C=1，光标
#define LCD_DISPLAYON 0x04//D=1，显示开
//LCD光标和显示移位控制位(LCD_CURSORSHIFT=0x10)
#define LCD_CURSOR2LEFT 0x00//S/C=0,R/L=0:光标往左移
#define LCD_CURSOR2RIGHT 0x04//S/C=0,R/L=1:光标往右移
#define LCD_DC2LEFT 0x08//S/C=1,R/L=0:显示向左移，光标跟着移
#define LCD_DC2RIGHT 0x0C//S/C=1,R/L=1:显示向右移,光标跟着移

//LCD功能设置位(LCD_FUNCTIONSET=0x20)
#define LCD_4BITMODE 0x00 //DL=0:4位(DB7-4)数据，需2次传输
#define LCD_8BITMODE 0x10 //DL=1：8位(DB7-0)数据传输
#define LCD_1LINE 0x00 //N=0，1行显示
#define LCD_2LINE 0x08 //N=1，2行显示
#define LCD_5X8DOTS 0x00 //F=0：5X8 dots 字符
#define LCD_5XADOTS 0x04 //F=1：5X10 dots字符，只能1行显示
//LCD 1602 控制管脚：I2C数据的低4位（PCF8574-P0~3)
#define LCD_RS 0x01 //PCF8574-P0控制LCD1602的RS管脚
#define LCD_RW 0x02 //PCF8574-P1）控制LCD1602的RW管脚
#define LCD_E 0x04 //PCF8574-P2）控制LCD1602的E管脚
#define LCD_BACKLIGHTON 0x08 //PCF8574-P3控制LCD1602的K管脚
#define LCD_SLAVE_ADDRESS 0x27 //从机地址PCF8574(A2-0:111)

unsigned char TWI_Write_LCD(unsigned char uc_data);
void LCD_4Bit_Write(unsigned char uc_data); //4位方式写PCF8574
void LCD_8Bit_Write(unsigned char uc_data,unsigned char uc_mode);
void LCD_Init(); //初始化LCD1602
void LCD_Set_Cursor_Location(unsigned char row,unsigned char col);
void LCD_Write_NewChar(char c_data); //在当前位置显示
void LCD_Write_Char(unsigned char row,unsigned char col,char c_data);
void LCD_Write_String(unsigned char row,unsigned char col,const char *pStr);

#endif
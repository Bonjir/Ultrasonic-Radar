/************************************************************************************
* Description:
* 1. 128*64点整OLED模块功能演示程序的字表，仅适用heltec.taobao.com所售产品;
* 2. 字表由打包资料中的“取字软件”计算得出;
* 3. 取字方式 -- 共阴、列行式、逆向输出
*
* Others: none;
*
* Function List:
*
* 1. void delay(unsigned int z) -- 主函数中用于调整显示效果的延时函数,STC89C52 12MHZ z=1时大约延时1ms,其他频率需要自己计算
* 2. void main(void) -- 主函数
*
* History: none;
*
*************************************************************************************/
#ifndef P6X8CHAR
#define P6X8CHAR 0
#endif
#ifndef P8X16CHAR
#define P8X16CHAR 0
#endif


/***************************16*16的点阵字体取模方式：共阴——列行式——逆向输出*********/
/*
extern unsigned char F16x16[];
*/

#if P6X8CHAR

/************************************6*8的点阵************************************/
extern const unsigned char F6x8[][6];

#endif

#if P8X16CHAR

/****************************************8*16的点阵************************************/
extern const unsigned char F8X16[];

#endif 

/*

extern unsigned char BMP1[];

extern unsigned char BMP2[];
*/

/************************************************************************************
* Description:
* 1. 128*64����OLEDģ�鹦����ʾ������ֱ�������heltec.taobao.com���۲�Ʒ;
* 2. �ֱ��ɴ�������еġ�ȡ�����������ó�;
* 3. ȡ�ַ�ʽ -- ����������ʽ���������
*
* Others: none;
*
* Function List:
*
* 1. void delay(unsigned int z) -- �����������ڵ�����ʾЧ������ʱ����,STC89C52 12MHZ z=1ʱ��Լ��ʱ1ms,����Ƶ����Ҫ�Լ�����
* 2. void main(void) -- ������
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


/***************************16*16�ĵ�������ȡģ��ʽ��������������ʽ�����������*********/
/*
extern unsigned char F16x16[];
*/

#if P6X8CHAR

/************************************6*8�ĵ���************************************/
extern const unsigned char F6x8[][6];

#endif

#if P8X16CHAR

/****************************************8*16�ĵ���************************************/
extern const unsigned char F8X16[];

#endif 

/*

extern unsigned char BMP1[];

extern unsigned char BMP2[];
*/

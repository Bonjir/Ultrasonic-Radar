/*
 * 3d_radar.cpp
 *
 * Created: 2023/4/28 22:54:29
 * Author : Bonjir
 *   Note :
 *	
 *   Todo :
 *			* 对scr数组进行放缩以节省distances的空间				(x)
 *			* 数据规则化处理 // 去掉大误差数据						(v)
 *			* 小键盘同时按下多个按键，控制舵机角度、显示在oled		
 *			* 上位机通信，把三维扫描结果传给电脑，用opengl显示出来，可漫游 (v)
 */ 
#define F_CPU 16000000UL
#include <stdlib.h>
#include <stdio.h>
#include "pins.h"
#include "servo.h"
#include "ultrasonic.h"
#include "twi_lcd.h"
#include "OLED_Paint.h"
#include "quickmath.h"
#include "USART.h"

SERVO servo_z, servo_r; // servo_z rotates around the z axis; servo_r rotates around the r vector
ULTRASONIC ultrasonic;

uint16_t GetDenoisedAverage(uint16_t data[], uint8_t arr_len, uint8_t *denoised_arr_len);

void Setup()
{
	servo_z.Attach(PINS_D4);
	servo_r.Attach(PINS_D3);
	
	ultrasonic.Attach(PINS_C2, PINS_C1);
	
	/*LCD_Init();*/
	OP_Init();
	USART_Init(CBR_9600, F_CPU);
}

#define DEGREE_STEP 3
#define NTIMES_ULTRASONIC_MEASURE 10

int main(void)
{
	Setup();
	
	servo_z.SetPos(0);
	servo_r.SetPos(0);
	
	uint16_t distance=0;
	uint16_t distances_at_same_pos[20];
	
	OP_PutPixel(SCR_WX / 2, 0, 1);
	OP_Update();	
	
	for(int deg_z = 0; deg_z <= 180; deg_z += DEGREE_STEP)
	{
		servo_z.SetPos(deg_z);
		
		for(int deg_r_fake = 0, deg_r; deg_r_fake <= 180; deg_r_fake += DEGREE_STEP)
		{
			deg_r = ((deg_z / DEGREE_STEP & 1) ? 180 - deg_r_fake : deg_r_fake);
			servo_r.SetPos(deg_r);
			
			distance = 0;
			for(int i = 0; i < NTIMES_ULTRASONIC_MEASURE; i ++)
			{
				distances_at_same_pos[i] = ultrasonic.GetDistance();
			}
			distance = GetDenoisedAverage(distances_at_same_pos, NTIMES_ULTRASONIC_MEASURE, NULL);
			
			char szText[40];
			sprintf(szText, "{[DAT](dst)degz:%d;degr:%d;dist:%d}", deg_z, deg_r, distance);
			int len = strlen(szText);
			USART_WriteString(szText, len);
			
			_delay_ms(10);
		}
	}
	
    while (1) 
    {
    }
}

/*
void Swap(uint16_t *a, uint16_t *b)
{
	*a ^= *b ^= *a ^= *b;
}

void BubbleSort(uint16_t arr[], uint8_t arr_len)
{
	for(int i = 0; i < arr_len; i ++)
	{
		for(int j = 0; j < i; j ++)
		{
			if(arr[j] > arr[j + 1])
				Swap(arr + j, arr + j + 1);
		}
	}
}
*/

uint16_t GetDenoisedAverage(uint16_t data[], uint8_t arr_len, uint8_t *denoised_arr_len)
{
	uint32_t sum = 0;
	for(int8_t i = 0; i < arr_len; i ++)
		sum += data[i];
	uint16_t avg = sum / arr_len;
	uint8_t _denoised_arr_len = 0;
	sum = 0;
	
	for(int8_t i = 0; i < arr_len; i ++)
	{
		uint16_t err = abs(data[i] - avg);
		if(err * 10 < avg * 4) // data with error <= 40% are reserved
		{
			data[_denoised_arr_len] = data[i];
			_denoised_arr_len ++;
			sum += data[i];
		}
	}
	if(denoised_arr_len != NULL)
		(*denoised_arr_len) = _denoised_arr_len;
	if(_denoised_arr_len)
		return sum / _denoised_arr_len;
	else
		return 0;
}

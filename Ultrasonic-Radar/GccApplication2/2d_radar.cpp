/*
 * 2d_radar.cpp
 *
 * Created: 2023/4/28 22:54:29
 * Author : Bonjir
 *   Note :
 *	
 *   Todo :
 */ 
#define F_CPU 16000000UL
#include <stdlib.h>
#include "pins.h"
#include "servo.h"
#include "ultrasonic.h"
#include "twi_lcd.h"
#include "OLED_Paint.h"
#include "quickmath.h"

SERVO servo_z, servo_r; // servo_z rotates around the z axis; servo_r rotates around the r vector
ULTRASONIC ultrasonic;

uint16_t GetDenoisedAverage(uint16_t data[], uint8_t arr_len, uint8_t *denoised_arr_len);

void Setup()
{
	servo_z.Attach(PINS_D4);
	servo_r.Attach(PINS_D3);
	
	ultrasonic.Attach(PINS_C2, PINS_C1);
/*

	LCD_Init();*/
	OP_Init();
}

uint16_t max_distance =  0, distances[91] = { 0 };
double real_scr_ratio = 100, prv_real_scr_ratio = 0;

int main(void)
{
	Setup();
	
	servo_z.SetPos(0);
	servo_r.SetPos(0);
	
	uint16_t distance=0;
	uint16_t distances_at_same_pos[20];
	//char sDistance[10]; // LCD
	
	OP_PutPixel(SCR_WX / 2, 0, 1);
	OP_Update();	
	
	for(int deg = 0; deg <= 180; deg += 2)
	{
		servo_z.SetPos(deg);
		
	REMEASURE:
		distance = 0;
		int n_getdistance = 20;
		for(int i = 0; i < n_getdistance; i ++)
		{
			distances_at_same_pos[i] = ultrasonic.GetDistance();
		}
		distance = GetDenoisedAverage(distances_at_same_pos, n_getdistance, NULL);
		if(distance == 0)
			goto REMEASURE;
		
		distances[deg / 2] = distance;
		max_distance = max(max_distance, distance);
		real_scr_ratio = min(real_scr_ratio, ((SCR_WY - 1) * 1.0 / max_distance));
		/*
		itoa(distance, sDistance, 10); // LCD
		LCD_Write_String(1, 0, "     ");
		LCD_Write_String(1, 0, sDistance);
		
		itoa(max_distance, sDistance, 10); // LCD
		LCD_Write_String(1, 8, "     ");
		LCD_Write_String(1, 8, sDistance);*/
		/*
		itoa(SCR_WX / 2 - quick_cos(deg) * distances[deg] * real_scr_ratio, sDistance, 10); // LCD
		LCD_Write_String(0, 0, "     ");
		LCD_Write_String(0, 0, sDistance);
		
		itoa(quick_sin(deg) * distances[deg] * real_scr_ratio, sDistance, 10); // LCD
		LCD_Write_String(0, 6, "     ");
		LCD_Write_String(0, 6, sDistance);
		*/
		
		if(prv_real_scr_ratio != real_scr_ratio)
		{
			OP_Clear();
			OP_PutPixel(SCR_WX / 2, 0, 1);
			for(int i = 0; i <= deg; i += 2)
			{
				OP_PutPixel(SCR_WX / 2 - quick_cos(i) * distances[i / 2] * real_scr_ratio, quick_sin(i) * distances[i / 2] * real_scr_ratio, 1);
			}
		}
		else
		{
			OP_PutPixel(SCR_WX / 2 - quick_cos(deg) * distances[deg / 2] * real_scr_ratio, quick_sin(deg) * distances[deg / 2] * real_scr_ratio, 1);
		}
		OP_Update();
		prv_real_scr_ratio = real_scr_ratio;
		
		_delay_ms(10);
	}
	
    while (1) 
    {
    }
}

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

uint16_t GetDenoisedAverage(uint16_t data[], uint8_t arr_len, uint8_t *denoised_arr_len)
{
/*
	BubbleSort(data, arr_len);

	int midval = data[arr_len / 2];
	uint8_t _denoised_arr_len = 0;
	uint32_t sum = 0;
	
	for(int8_t i = 0; i < arr_len; i ++)
	{
		uint16_t err = abs(data[i] - midval);
		if(err < midval * 0.1) // data with error <= 40% are reserved
		{
			data[_denoised_arr_len] = data[i];
			_denoised_arr_len ++;
			sum += data[i];
		}
	}
	
	if(denoised_arr_len != NULL)
	(*denoised_arr_len) = _denoised_arr_len;
	return sum / _denoised_arr_len;
	*/

	uint32_t sum = 0;
	for(int8_t i = 0; i < arr_len; i ++)
		sum += data[i];
	uint16_t avg = sum / arr_len;
	uint8_t _denoised_arr_len = 0;
	sum = 0;
	
	for(int8_t i = 0; i < arr_len; i ++)
	{
		uint16_t err = abs(data[i] - avg);
		if(err < avg * 0.4) // data with error <= 40% are reserved
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

/*
 * _main_2d_3d_radar.cpp
 *
 * Created: 2023/5/16 21:06:25
 *  Author: Bonjir
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
#include "btnmat.h"

SERVO servo_z, servo_r; // servo_z rotates around the z axis; servo_r rotates around the r vector
ULTRASONIC ultrasonic;
BTNMAT btnmat;

void ShowMenu();
void Radar_2D();
void Radar_3D();
//uint16_t GetDenoisedAverage(uint16_t data[], uint8_t arr_len, uint8_t *denoised_arr_len);

void Setup()
{
	servo_z.Attach(PINS_D4);
	servo_r.Attach(PINS_D3);
	
	ultrasonic.Attach(PINS_C2, PINS_C1);
	
	PINS btnmat_pins[8] = {PINS_B4, PINS_B3, PINS_B2, PINS_B1, PINS_D5, PINS_D6, PINS_D7, PINS_B0};
	btnmat.Attach(btnmat_pins);
	
	OP_Init();
	USART_Init(CBR_9600, F_CPU);
}

int main(void)
{
	Setup();
	
	while(1)
	{
		ShowMenu();
		
		servo_z.SetPos(0);
		servo_r.SetPos(0);
		
		int key;
		while(!(key = btnmat.GetKeyDown()));
		if(key == 1)
		{
			Radar_2D();
			while(!btnmat.GetKeyDown());
		}
		else if(key == 2)
		{
			Radar_3D();
		}
	}
}

/************************************************************************/
/* radar main                                                           */
/************************************************************************/

#define DEGREE_STEP_3D 3
#define NTIMES_ULTRASONIC_MEASURE_3D 10
#define DEGREE_STEP_2D 2
#define NTIMES_ULTRASONIC_MEASURE_2D 20

void Radar_3D()
{
	OP_Clear();
	OP_Update();
	
	uint16_t distance=0;
	uint16_t distances_at_same_pos[20];
	
	for(int deg_z = 0; deg_z <= 180; deg_z += DEGREE_STEP_3D)
	{
		servo_z.SetPos(deg_z);
		
		for(int deg_r_fake = 0, deg_r; deg_r_fake <= 180; deg_r_fake += DEGREE_STEP_3D)
		{
			deg_r = ((deg_z / DEGREE_STEP_3D & 1) ? 180 - deg_r_fake : deg_r_fake);
			servo_r.SetPos(deg_r);
			
			distance = 0;
			for(int i = 0; i < NTIMES_ULTRASONIC_MEASURE_3D; i ++)
			{
				distances_at_same_pos[i] = ultrasonic.GetDistance();
				distance += distances_at_same_pos[i];
			}
			//distance = GetDenoisedAverage(distances_at_same_pos, NTIMES_ULTRASONIC_MEASURE, NULL);
			distance /= NTIMES_ULTRASONIC_MEASURE_3D;
			
			char szText[40];
			sprintf(szText, "{[DAT](dst)degz:%d;degr:%d;dist:%d}", deg_z, deg_r, distance);
			int len = strlen(szText);
			USART_WriteString(szText, len);
			
			_delay_ms(10);
		}
	}
}

uint16_t max_distance =  0, distances[91] = { 0 };
double real_scr_ratio = 100, prv_real_scr_ratio = 0;

void Radar_2D()
{
	uint16_t distance=0;
	uint16_t distances_at_same_pos[20];
	
	OP_Clear();
	OP_PutPixel(SCR_WX / 2, 0, 1);
	OP_Update();	
	
	for(int deg = 0; deg <= 180; deg += 2)
	{
		servo_z.SetPos(deg);
		
	/*REMEASURE:
		distance = 0;
		int n_getdistance = 20;
		for(int i = 0; i < n_getdistance; i ++)
		{
			distances_at_same_pos[i] = ultrasonic.GetDistance();
		}
		distance = GetDenoisedAverage(distances_at_same_pos, n_getdistance, NULL);
		if(distance == 0)
			goto REMEASURE;
		*/ // deleted because of the limit of resource
		
		distance = 0;
		//int n_getdistance = 20;
		for(int i = 0; i < NTIMES_ULTRASONIC_MEASURE_2D; i ++)
		{
			distances_at_same_pos[i] = ultrasonic.GetDistance();
			distance += distances_at_same_pos[i];
		}
		//distance = GetDenoisedAverage(distances_at_same_pos, n_getdistance, NULL);
		distance /= NTIMES_ULTRASONIC_MEASURE_2D;
		
		distances[deg / DEGREE_STEP_2D] = distance;
		max_distance = max(max_distance, distance);
		real_scr_ratio = min(real_scr_ratio, ((SCR_WY - 1) * 1.0 / max_distance));
		
		if(prv_real_scr_ratio != real_scr_ratio)
		{
			OP_Clear();
			OP_PutPixel(SCR_WX / 2, 0, 1);
			for(int i = 0; i <= deg; i += 2)
			{
				OP_PutPixel(
					SCR_WX / 2 - quick_cos(i) * distances[i / DEGREE_STEP_2D] * real_scr_ratio, 
					quick_sin(i) * distances[i / DEGREE_STEP_2D] * real_scr_ratio, 
					1);
			}
		}
		else
		{
			OP_PutPixel(
				SCR_WX / 2 - quick_cos(deg) * distances[deg / DEGREE_STEP_2D] * real_scr_ratio,
				quick_sin(deg) * distances[deg / DEGREE_STEP_2D] * real_scr_ratio,
				1);
		}
		OP_Update();
		prv_real_scr_ratio = real_scr_ratio;
		
		_delay_ms(10);
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

void ShowMenu()
{
	OP_Clear();
	OP_TextOut(4, 4, "1 2Dradar", 9, 1);
	OP_TextOut(4, 16, "2 3Dradar", 9, 1);
	OP_Update();
}
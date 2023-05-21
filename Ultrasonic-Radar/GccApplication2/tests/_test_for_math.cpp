/*
 * test_for_math.c
 *
 * Created: 2023/4/24 19:35:10
 *  Author: 86198
 */

#include <avr/io.h>
#include "OLED_Paint.h"
#include "quickmath.h"

int main(void)
{
	OP_Init();
	while(1)
	{
		for(int deg = 0; deg < 90; deg ++)
		{
			float sindeg = quick_sin(deg);
			float cosdeg = quick_cos(deg);
			
			char str_deg[6] = { 'd', 'e', 'g', '=', (int)deg / 10 + '0', (int)deg % 10 + '0'};
			char str_sin[5] = { (int)sindeg + '0', '.', (int)(sindeg * 1e1) % 10 + '0', (int)(sindeg * 1e2) % 10 + '0', (int)(sindeg * 1e3) % 10 + '0'};
			char str_cos[5] = { (int)cosdeg + '0', '.', (int)(cosdeg * 1e1) % 10 + '0', (int)(cosdeg * 1e2) % 10 + '0', (int)(cosdeg * 1e3) % 10 + '0'};
			
			OP_TextOut(0, 0, str_deg, 6, 1);
			OP_TextOut(0, 8, str_sin, 5, 1);
			OP_TextOut(0, 16, str_cos, 5, 1);
			
			OP_Update();
			OP_Clear();
			_delay_ms(1000);
		}
	}
}
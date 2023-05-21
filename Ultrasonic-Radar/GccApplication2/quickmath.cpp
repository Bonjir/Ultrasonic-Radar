/*
 * quickmath.cpp
 *
 * Created: 2023/4/29 17:15:16
 *  Author: Bonjir
 */ 

#include <string.h>
#include "quickmath.h"

#ifdef STORE_IN_EEPROM

float EEMEM _quick_sin_dat[91] = 
{
	0.00, 0.02, 0.03, 0.05, 0.07, 0.09, 0.10, 0.12, 0.14, 0.16, 0.17, 0.19, 0.21, 
	0.22, 0.24, 0.26, 0.28, 0.29, 0.31, 0.33, 0.34, 0.36, 0.37, 0.39, 0.41, 0.42, 
	0.44, 0.45, 0.47, 0.48, 0.50, 0.52, 0.53, 0.54, 0.56, 0.57, 0.59, 0.60, 0.62, 
	0.63, 0.64, 0.66, 0.67, 0.68, 0.69, 0.71, 0.72, 0.73, 0.74, 0.75, 0.77, 0.78, 
	0.79, 0.80, 0.81, 0.82, 0.83, 0.84, 0.85, 0.86, 0.87, 0.87, 0.88, 0.89, 0.90, 
	0.91, 0.91, 0.92, 0.93, 0.93, 0.94, 0.95, 0.95, 0.96, 0.96, 0.97, 0.97, 0.97, 
	0.98, 0.98, 0.98, 0.99, 0.99, 0.99, 0.99, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00
};

float quick_sin(int deg)
{
	if(deg >= 0 && deg <= 90)
	{
		//return _quick_sin_dat[deg];/**/
		char float_dat[4];
		for(int i = 0; i  < 4; i ++)
			float_dat[i] = eeprom_read_byte((uint8_t *)_quick_sin_dat + deg * 4 + i);
		float ans;
		memcpy(&ans, float_dat, 4);
		return ans;
		//return eeprom_read_byte((int *)(&_quick_sin_dat[deg]));
	}

	if(deg < 0)
		return -quick_sin(-deg);
	if(deg >= 180)
		return -quick_sin(deg - 180);
	if(deg > 90 && deg < 180)
		return quick_sin(180 - deg);
	// assert(false);
	return 0;
}

float quick_cos(int deg)
{	
	return quick_sin(deg + 90);
}

#else

float quick_sin(int deg)
{
	switch (deg)
	{
		case 0: { return 0.00; }
		case 1: { return 0.02; }
		case 2: { return 0.03; }
		case 3: { return 0.05; }
		case 4: { return 0.07; }
		case 5: { return 0.09; }
		case 6: { return 0.10; }
		case 7: { return 0.12; }
		case 8: { return 0.14; }
		case 9: { return 0.16; }
		case 10: { return 0.17; }
		case 11: { return 0.19; }
		case 12: { return 0.21; }
		case 13: { return 0.22; }
		case 14: { return 0.24; }
		case 15: { return 0.26; }
		case 16: { return 0.28; }
		case 17: { return 0.29; }
		case 18: { return 0.31; }
		case 19: { return 0.33; }
		case 20: { return 0.34; }
		case 21: { return 0.36; }
		case 22: { return 0.37; }
		case 23: { return 0.39; }
		case 24: { return 0.41; }
		case 25: { return 0.42; }
		case 26: { return 0.44; }
		case 27: { return 0.45; }
		case 28: { return 0.47; }
		case 29: { return 0.48; }
		case 30: { return 0.50; }
		case 31: { return 0.52; }
		case 32: { return 0.53; }
		case 33: { return 0.54; }
		case 34: { return 0.56; }
		case 35: { return 0.57; }
		case 36: { return 0.59; }
		case 37: { return 0.60; }
		case 38: { return 0.62; }
		case 39: { return 0.63; }
		case 40: { return 0.64; }
		case 41: { return 0.66; }
		case 42: { return 0.67; }
		case 43: { return 0.68; }
		case 44: { return 0.69; }
		case 45: { return 0.71; }
		case 46: { return 0.72; }
		case 47: { return 0.73; }
		case 48: { return 0.74; }
		case 49: { return 0.75; }
		case 50: { return 0.77; }
		case 51: { return 0.78; }
		case 52: { return 0.79; }
		case 53: { return 0.80; }
		case 54: { return 0.81; }
		case 55: { return 0.82; }
		case 56: { return 0.83; }
		case 57: { return 0.84; }
		case 58: { return 0.85; }
		case 59: { return 0.86; }
		case 60:
		case 61: { return 0.87; }
		case 62: { return 0.88; }
		case 63: { return 0.89; }
		case 64: { return 0.90; }
		case 65:
		case 66: { return 0.91; }
		case 67: { return 0.92; }
		case 68:
		case 69: { return 0.93; }
		case 70: { return 0.94; }
		case 71:
		case 72: { return 0.95; }
		case 73:
		case 74: { return 0.96; }
		default:
		{
			if(deg <= 77) { return 0.97; }
			if(deg <= 80) { return 0.98; }
			if(deg <= 84) { return 0.99; }
			if(deg <= 90) { return 1.00; }
				
			
			if(deg < 0)
				return -quick_sin(-deg);
			if(deg >= 180)
				return -quick_sin(deg - 180);
			if(deg > 90 && deg < 180)
				return quick_cos(deg - 90);
			// assert(false);
			return 0;
		}
	}
}

float quick_cos(int deg)
{
	return quick_sin(deg + 90);
	/*switch (deg)
	{
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5: { return 1.00; }
		case 6:
		case 7:
		case 8:
		case 9: { return 0.99; }
		case 10:
		case 11:
		case 12: { return 0.98; }
		case 13:
		case 14:
		case 15: { return 0.97; }
		case 16:
		case 17: { return 0.96; }
		case 18:
		case 19: { return 0.95; }
		case 20: { return 0.94; }
		case 21:
		case 22: { return 0.93; }
		case 23: { return 0.92; }
		case 24:
		case 25: { return 0.91; }
		case 26: { return 0.90; }
		case 27: { return 0.89; }
		case 28: { return 0.88; }
		case 29:
		case 30: { return 0.87; }
		case 31: { return 0.86; }
		case 32: { return 0.85; }
		case 33: { return 0.84; }
		case 34: { return 0.83; }
		case 35: { return 0.82; }
		case 36: { return 0.81; }
		case 37: { return 0.80; }
		case 38: { return 0.79; }
		case 39: { return 0.78; }
		case 40: { return 0.77; }
		case 41: { return 0.75; }
		case 42: { return 0.74; }
		case 43: { return 0.73; }
		case 44: { return 0.72; }
		case 45: { return 0.71; }
		case 46: { return 0.69; }
		case 47: { return 0.68; }
		case 48: { return 0.67; }
		case 49: { return 0.66; }
		case 50: { return 0.64; }
		case 51: { return 0.63; }
		case 52: { return 0.62; }
		case 53: { return 0.60; }
		case 54: { return 0.59; }
		case 55: { return 0.57; }
		case 56: { return 0.56; }
		case 57: { return 0.54; }
		case 58: { return 0.53; }
		case 59: { return 0.52; }
		case 60: { return 0.50; }
		case 61: { return 0.48; }
		case 62: { return 0.47; }
		case 63: { return 0.45; }
		case 64: { return 0.44; }
		case 65: { return 0.42; }
		case 66: { return 0.41; }
		case 67: { return 0.39; }
		case 68: { return 0.37; }
		case 69: { return 0.36; }
		case 70: { return 0.34; }
		case 71: { return 0.33; }
		case 72: { return 0.31; }
		case 73: { return 0.29; }
		case 74: { return 0.28; }
		case 75: { return 0.26; }
		case 76: { return 0.24; }
		case 77: { return 0.22; }
		case 78: { return 0.21; }
		case 79: { return 0.19; }
		case 80: { return 0.17; }
		case 81: { return 0.16; }
		case 82: { return 0.14; }
		case 83: { return 0.12; }
		case 84: { return 0.10; }
		case 85: { return 0.09; }
		case 86: { return 0.07; }
		case 87: { return 0.05; }
		case 88: { return 0.03; }
		case 89: { return 0.02; }
		case 90: { return 0.00; }
		default: 
		{
			if(deg < 0)
				return quick_cos(-deg);
			if(deg >= 180)
				return -quick_cos(deg - 180);
			if(deg > 90 && deg < 180)
				return -quick_sin(deg - 90);
			// assert(false);
			return 0;
		}
	}*/
}

#endif
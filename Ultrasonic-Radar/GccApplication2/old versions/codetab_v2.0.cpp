/*
 * codetab.c
 *
 * Created: 2023/4/23 20:46:17
 *  Author: 86198
 */ 

#include "codetab_v2.0.h"

unsigned char GetP6x8ChDat_V2(char ch, unsigned char col)
{
	switch(ch)
	{
		case ' ': { switch (col) { case 0: return 0x00; case 1: return 0x00; case 2: return 0x00; case 3: return 0x00; case 4: return 0x00; case 5: return 0x00;  } }
		case '!': { switch (col) { case 0: return 0x00; case 1: return 0x00; case 2: return 0x00; case 3: return 0x2f; case 4: return 0x00; case 5: return 0x00;  } }
		//case '\"':{ switch (col) { case 0: return 0x00; case 1: return 0x00; case 2: return 0x07; case 3: return 0x00; case 4: return 0x07; case 5: return 0x00;  } }
		//case '#': { switch (col) { case 0: return 0x00; case 1: return 0x14; case 2: return 0x7f; case 3: return 0x14; case 4: return 0x7f; case 5: return 0x14;  } }
		//case '$': { switch (col) { case 0: return 0x00; case 1: return 0x24; case 2: return 0x2a; case 3: return 0x7f; case 4: return 0x2a; case 5: return 0x12;  } }
		//case '%': { switch (col) { case 0: return 0x00; case 1: return 0x62; case 2: return 0x64; case 3: return 0x08; case 4: return 0x13; case 5: return 0x23;  } }
		//case '&': { switch (col) { case 0: return 0x00; case 1: return 0x36; case 2: return 0x49; case 3: return 0x55; case 4: return 0x22; case 5: return 0x50;  } }
		//case '\'':{ switch (col) { case 0: return 0x00; case 1: return 0x00; case 2: return 0x05; case 3: return 0x03; case 4: return 0x00; case 5: return 0x00;  } }
		/*case '(': { switch (col) { case 0: return 0x00; case 1: return 0x00; case 2: return 0x1c; case 3: return 0x22; case 4: return 0x41; case 5: return 0x00;  } }
		case ')': { switch (col) { case 0: return 0x00; case 1: return 0x00; case 2: return 0x41; case 3: return 0x22; case 4: return 0x1c; case 5: return 0x00;  } }
		case '*': { switch (col) { case 0: return 0x00; case 1: return 0x14; case 2: return 0x08; case 3: return 0x3E; case 4: return 0x08; case 5: return 0x14;  } }
		case '+': { switch (col) { case 0: return 0x00; case 1: return 0x08; case 2: return 0x08; case 3: return 0x3E; case 4: return 0x08; case 5: return 0x08;  } }*/
		case ',': { switch (col) { case 0: return 0x00; case 1: return 0x00; case 2: return 0x00; case 3: return 0xA0; case 4: return 0x60; case 5: return 0x00;  } }
		//case '-': { switch (col) { case 0: return 0x00; case 1: return 0x08; case 2: return 0x08; case 3: return 0x08; case 4: return 0x08; case 5: return 0x08;  } }
		case '.': { switch (col) { case 0: return 0x00; case 1: return 0x00; case 2: return 0x60; case 3: return 0x60; case 4: return 0x00; case 5: return 0x00;  } }
		//case '/': { switch (col) { case 0: return 0x00; case 1: return 0x20; case 2: return 0x10; case 3: return 0x08; case 4: return 0x04; case 5: return 0x02;  } }
		case '0': { switch (col) { case 0: return 0x00; case 1: return 0x3E; case 2: return 0x51; case 3: return 0x49; case 4: return 0x45; case 5: return 0x3E;  } }
		case '1': { switch (col) { case 0: return 0x00; case 1: return 0x00; case 2: return 0x42; case 3: return 0x7F; case 4: return 0x40; case 5: return 0x00;  } }
		case '2': { switch (col) { case 0: return 0x00; case 1: return 0x42; case 2: return 0x61; case 3: return 0x51; case 4: return 0x49; case 5: return 0x46;  } }
		case '3': { switch (col) { case 0: return 0x00; case 1: return 0x21; case 2: return 0x41; case 3: return 0x45; case 4: return 0x4B; case 5: return 0x31;  } }
		case '4': { switch (col) { case 0: return 0x00; case 1: return 0x18; case 2: return 0x14; case 3: return 0x12; case 4: return 0x7F; case 5: return 0x10;  } }
		case '5': { switch (col) { case 0: return 0x00; case 1: return 0x27; case 2: return 0x45; case 3: return 0x45; case 4: return 0x45; case 5: return 0x39;  } }
		case '6': { switch (col) { case 0: return 0x00; case 1: return 0x3C; case 2: return 0x4A; case 3: return 0x49; case 4: return 0x49; case 5: return 0x30;  } }
		case '7': { switch (col) { case 0: return 0x00; case 1: return 0x01; case 2: return 0x71; case 3: return 0x09; case 4: return 0x05; case 5: return 0x03;  } }
		case '8': { switch (col) { case 0: return 0x00; case 1: return 0x36; case 2: return 0x49; case 3: return 0x49; case 4: return 0x49; case 5: return 0x36;  } }
		case '9': { switch (col) { case 0: return 0x00; case 1: return 0x06; case 2: return 0x49; case 3: return 0x49; case 4: return 0x29; case 5: return 0x1E;  } }
		case ':': { switch (col) { case 0: return 0x00; case 1: return 0x00; case 2: return 0x36; case 3: return 0x36; case 4: return 0x00; case 5: return 0x00;  } }
		//case ';': { switch (col) { case 0: return 0x00; case 1: return 0x00; case 2: return 0x56; case 3: return 0x36; case 4: return 0x00; case 5: return 0x00;  } }
		//case '<': { switch (col) { case 0: return 0x00; case 1: return 0x08; case 2: return 0x14; case 3: return 0x22; case 4: return 0x41; case 5: return 0x00;  } }
		case '=': { switch (col) { case 0: return 0x00; case 1: return 0x14; case 2: return 0x14; case 3: return 0x14; case 4: return 0x14; case 5: return 0x14;  } }
		//case '>': { switch (col) { case 0: return 0x00; case 1: return 0x00; case 2: return 0x41; case 3: return 0x22; case 4: return 0x14; case 5: return 0x08;  } }
		//case '?': { switch (col) { case 0: return 0x00; case 1: return 0x02; case 2: return 0x01; case 3: return 0x51; case 4: return 0x09; case 5: return 0x06;  } }
		//case '@': { switch (col) { case 0: return 0x00; case 1: return 0x32; case 2: return 0x49; case 3: return 0x59; case 4: return 0x51; case 5: return 0x3E;  } }
		case 'A': { switch (col) { case 0: return 0x00; case 1: return 0x7C; case 2: return 0x12; case 3: return 0x11; case 4: return 0x12; case 5: return 0x7C;  } }
		case 'B': { switch (col) { case 0: return 0x00; case 1: return 0x7F; case 2: return 0x49; case 3: return 0x49; case 4: return 0x49; case 5: return 0x36;  } }
		case 'C': { switch (col) { case 0: return 0x00; case 1: return 0x3E; case 2: return 0x41; case 3: return 0x41; case 4: return 0x41; case 5: return 0x22;  } }
		case 'D': { switch (col) { case 0: return 0x00; case 1: return 0x7F; case 2: return 0x41; case 3: return 0x41; case 4: return 0x22; case 5: return 0x1C;  } }
		case 'E': { switch (col) { case 0: return 0x00; case 1: return 0x7F; case 2: return 0x49; case 3: return 0x49; case 4: return 0x49; case 5: return 0x41;  } }
		case 'F': { switch (col) { case 0: return 0x00; case 1: return 0x7F; case 2: return 0x09; case 3: return 0x09; case 4: return 0x09; case 5: return 0x01;  } }
		case 'G': { switch (col) { case 0: return 0x00; case 1: return 0x3E; case 2: return 0x41; case 3: return 0x49; case 4: return 0x49; case 5: return 0x7A;  } }
		case 'H': { switch (col) { case 0: return 0x00; case 1: return 0x7F; case 2: return 0x08; case 3: return 0x08; case 4: return 0x08; case 5: return 0x7F;  } }
		case 'I': { switch (col) { case 0: return 0x00; case 1: return 0x00; case 2: return 0x41; case 3: return 0x7F; case 4: return 0x41; case 5: return 0x00;  } }
		case 'J': { switch (col) { case 0: return 0x00; case 1: return 0x20; case 2: return 0x40; case 3: return 0x41; case 4: return 0x3F; case 5: return 0x01;  } }
		case 'K': { switch (col) { case 0: return 0x00; case 1: return 0x7F; case 2: return 0x08; case 3: return 0x14; case 4: return 0x22; case 5: return 0x41;  } }
		case 'L': { switch (col) { case 0: return 0x00; case 1: return 0x7F; case 2: return 0x40; case 3: return 0x40; case 4: return 0x40; case 5: return 0x40;  } }
		case 'M': { switch (col) { case 0: return 0x00; case 1: return 0x7F; case 2: return 0x02; case 3: return 0x0C; case 4: return 0x02; case 5: return 0x7F;  } }
		case 'N': { switch (col) { case 0: return 0x00; case 1: return 0x7F; case 2: return 0x04; case 3: return 0x08; case 4: return 0x10; case 5: return 0x7F;  } }
		case 'O': { switch (col) { case 0: return 0x00; case 1: return 0x3E; case 2: return 0x41; case 3: return 0x41; case 4: return 0x41; case 5: return 0x3E;  } }
		case 'P': { switch (col) { case 0: return 0x00; case 1: return 0x7F; case 2: return 0x09; case 3: return 0x09; case 4: return 0x09; case 5: return 0x06;  } }
		case 'Q': { switch (col) { case 0: return 0x00; case 1: return 0x3E; case 2: return 0x41; case 3: return 0x51; case 4: return 0x21; case 5: return 0x5E;  } }
		case 'R': { switch (col) { case 0: return 0x00; case 1: return 0x7F; case 2: return 0x09; case 3: return 0x19; case 4: return 0x29; case 5: return 0x46;  } }
		case 'S': { switch (col) { case 0: return 0x00; case 1: return 0x46; case 2: return 0x49; case 3: return 0x49; case 4: return 0x49; case 5: return 0x31;  } }
		case 'T': { switch (col) { case 0: return 0x00; case 1: return 0x01; case 2: return 0x01; case 3: return 0x7F; case 4: return 0x01; case 5: return 0x01;  } }
		case 'U': { switch (col) { case 0: return 0x00; case 1: return 0x3F; case 2: return 0x40; case 3: return 0x40; case 4: return 0x40; case 5: return 0x3F;  } }
		case 'V': { switch (col) { case 0: return 0x00; case 1: return 0x1F; case 2: return 0x20; case 3: return 0x40; case 4: return 0x20; case 5: return 0x1F;  } }
		case 'W': { switch (col) { case 0: return 0x00; case 1: return 0x3F; case 2: return 0x40; case 3: return 0x38; case 4: return 0x40; case 5: return 0x3F;  } }
		case 'X': { switch (col) { case 0: return 0x00; case 1: return 0x63; case 2: return 0x14; case 3: return 0x08; case 4: return 0x14; case 5: return 0x63;  } }
		case 'Y': { switch (col) { case 0: return 0x00; case 1: return 0x07; case 2: return 0x08; case 3: return 0x70; case 4: return 0x08; case 5: return 0x07;  } }
		case 'Z': { switch (col) { case 0: return 0x00; case 1: return 0x61; case 2: return 0x51; case 3: return 0x49; case 4: return 0x45; case 5: return 0x43;  } }
		//case '[': { switch (col) { case 0: return 0x00; case 1: return 0x00; case 2: return 0x7F; case 3: return 0x41; case 4: return 0x41; case 5: return 0x00;  } }
		//case '\\':{ switch (col) { case 0: return 0x00; case 1: return 0x55; case 2: return 0x2A; case 3: return 0x55; case 4: return 0x2A; case 5: return 0x55;  } }
		//case ']': { switch (col) { case 0: return 0x00; case 1: return 0x00; case 2: return 0x41; case 3: return 0x41; case 4: return 0x7F; case 5: return 0x00;  } }
		//case '^': { switch (col) { case 0: return 0x00; case 1: return 0x04; case 2: return 0x02; case 3: return 0x01; case 4: return 0x02; case 5: return 0x04;  } }
		//case '_': { switch (col) { case 0: return 0x00; case 1: return 0x40; case 2: return 0x40; case 3: return 0x40; case 4: return 0x40; case 5: return 0x40;  } }
		//case '`': { switch (col) { case 0: return 0x00; case 1: return 0x00; case 2: return 0x01; case 3: return 0x02; case 4: return 0x04; case 5: return 0x00;  } }
		case 'a': { switch (col) { case 0: return 0x00; case 1: return 0x20; case 2: return 0x54; case 3: return 0x54; case 4: return 0x54; case 5: return 0x78;  } }
		case 'b': { switch (col) { case 0: return 0x00; case 1: return 0x7F; case 2: return 0x48; case 3: return 0x44; case 4: return 0x44; case 5: return 0x38;  } }
		case 'c': { switch (col) { case 0: return 0x00; case 1: return 0x38; case 2: return 0x44; case 3: return 0x44; case 4: return 0x44; case 5: return 0x20;  } }
		case 'd': { switch (col) { case 0: return 0x00; case 1: return 0x38; case 2: return 0x44; case 3: return 0x44; case 4: return 0x48; case 5: return 0x7F;  } }
		case 'e': { switch (col) { case 0: return 0x00; case 1: return 0x38; case 2: return 0x54; case 3: return 0x54; case 4: return 0x54; case 5: return 0x18;  } }
		case 'f': { switch (col) { case 0: return 0x00; case 1: return 0x08; case 2: return 0x7E; case 3: return 0x09; case 4: return 0x01; case 5: return 0x02;  } }
		case 'g': { switch (col) { case 0: return 0x00; case 1: return 0x18; case 2: return 0xA4; case 3: return 0xA4; case 4: return 0xA4; case 5: return 0x7C;  } }
		case 'h': { switch (col) { case 0: return 0x00; case 1: return 0x7F; case 2: return 0x08; case 3: return 0x04; case 4: return 0x04; case 5: return 0x78;  } }
		case 'i': { switch (col) { case 0: return 0x00; case 1: return 0x00; case 2: return 0x44; case 3: return 0x7D; case 4: return 0x40; case 5: return 0x00;  } }
		case 'j': { switch (col) { case 0: return 0x00; case 1: return 0x40; case 2: return 0x80; case 3: return 0x84; case 4: return 0x7D; case 5: return 0x00;  } }
		case 'k': { switch (col) { case 0: return 0x00; case 1: return 0x7F; case 2: return 0x10; case 3: return 0x28; case 4: return 0x44; case 5: return 0x00;  } }
		case 'l': { switch (col) { case 0: return 0x00; case 1: return 0x00; case 2: return 0x41; case 3: return 0x7F; case 4: return 0x40; case 5: return 0x00;  } }
		case 'm': { switch (col) { case 0: return 0x00; case 1: return 0x7C; case 2: return 0x04; case 3: return 0x18; case 4: return 0x04; case 5: return 0x78;  } }
		case 'n': { switch (col) { case 0: return 0x00; case 1: return 0x7C; case 2: return 0x08; case 3: return 0x04; case 4: return 0x04; case 5: return 0x78;  } }
		case 'o': { switch (col) { case 0: return 0x00; case 1: return 0x38; case 2: return 0x44; case 3: return 0x44; case 4: return 0x44; case 5: return 0x38;  } }
		case 'p': { switch (col) { case 0: return 0x00; case 1: return 0xFC; case 2: return 0x24; case 3: return 0x24; case 4: return 0x24; case 5: return 0x18;  } }
		case 'q': { switch (col) { case 0: return 0x00; case 1: return 0x18; case 2: return 0x24; case 3: return 0x24; case 4: return 0x18; case 5: return 0xFC;  } }
		case 'r': { switch (col) { case 0: return 0x00; case 1: return 0x7C; case 2: return 0x08; case 3: return 0x04; case 4: return 0x04; case 5: return 0x08;  } }
		case 's': { switch (col) { case 0: return 0x00; case 1: return 0x48; case 2: return 0x54; case 3: return 0x54; case 4: return 0x54; case 5: return 0x20;  } }
		case 't': { switch (col) { case 0: return 0x00; case 1: return 0x04; case 2: return 0x3F; case 3: return 0x44; case 4: return 0x40; case 5: return 0x20;  } }
		case 'u': { switch (col) { case 0: return 0x00; case 1: return 0x3C; case 2: return 0x40; case 3: return 0x40; case 4: return 0x20; case 5: return 0x7C;  } }
		case 'v': { switch (col) { case 0: return 0x00; case 1: return 0x1C; case 2: return 0x20; case 3: return 0x40; case 4: return 0x20; case 5: return 0x1C;  } }
		case 'w': { switch (col) { case 0: return 0x00; case 1: return 0x3C; case 2: return 0x40; case 3: return 0x30; case 4: return 0x40; case 5: return 0x3C;  } }
		case 'x': { switch (col) { case 0: return 0x00; case 1: return 0x44; case 2: return 0x28; case 3: return 0x10; case 4: return 0x28; case 5: return 0x44;  } }
		case 'y': { switch (col) { case 0: return 0x00; case 1: return 0x1C; case 2: return 0xA0; case 3: return 0xA0; case 4: return 0xA0; case 5: return 0x7C;  } }
		case 'z': { switch (col) { case 0: return 0x00; case 1: return 0x44; case 2: return 0x64; case 3: return 0x54; case 4: return 0x4C; case 5: return 0x44;  } }
		//case '|': { switch (col) { case 0: return 0x14; case 1: return 0x14; case 2: return 0x14; case 3: return 0x14; case 4: return 0x14; case 5: return 0x14;  } }
		default : { switch (col) { case 0: return 0x7f; case 1: return 0x41; case 2: return 0x41; case 3: return 0x41; case 4: return 0x7f; default:return 0x00;  } }
	}
	//unsigned char chs[] = { 0xff, 0x81, 0x81, 0x81, 0xff, 0x00}; return chs[col];
}
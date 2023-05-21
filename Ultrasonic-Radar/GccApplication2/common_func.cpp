/*
 * common_func.cpp
 *
 * Created: 2023/5/16 21:39:39
 *  Author: Bonjir
 */ 
#include <string.h>

int GetStrLen(const char str[])
{
	for(int i = 0; ; i ++)
	if(str[i] == '\0')
	return i;
}

void MemorySet(void *arr, char val, int size)
{
	for(int i = 0; i < size; i ++)
		*((char *)arr + i) = val;
}

void MemoryCpy(void *dst, const void *rsc, int size)
{
	for(int i = 0; i < size; i ++)
		*((char *)dst + i) = *((char *)rsc + i);
}
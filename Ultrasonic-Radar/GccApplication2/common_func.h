/*
 * common_func.h
 *
 * Created: 2023/5/16 11:24:18
 *  Author: Bonjir
 */ 
// if the flash goes full again, then implement the common functions by self instead of including std headers

#ifndef COMMON_FUNC_H_
#define COMMON_FUNC_H_

int GetStrLen(const char str[]);
void MemorySet(void *arr, char val, int size);
void MemoryCpy(void *dst, const void *rsc, int size);

#endif /* COMMON_FUNC_H_ */
/*
 * conversion.h
 * Scooter final project's header file for conversion functions
 * Authors: Ryan ZumBrunnen, Guanxiong Fu and Arash Yousefdezah
 * Created on: October 26, 2017
 *
 */


#ifndef CONVERSION_H_
#define CONVERSION_H_
#include <stdint.h>

void reverse(char s[], int slength);

int itoa(uint32_t n, char s[]);

void ftoa(float n, char s[], int afterpoint);

#endif /* CONVERSION_H_ */

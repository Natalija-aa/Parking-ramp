#ifndef TYPES_H
#define TYPES_H

#include <at89c51RC2.h>

typedef unsigned char uint8_t;
typedef signed char int8_t;
typedef unsigned int uint16_t;
typedef signed int int16_t;

typedef struct{
	uint8_t *buff;
	uint8_t index;	
} sBuffer_t;


#endif
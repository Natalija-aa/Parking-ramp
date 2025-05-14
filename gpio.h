#ifndef GPIO_H
#define GPIO_H

#include "types.h"

typedef struct{
	uint8_t state;
	uint8_t last_state;
} sPin_t;

void initGPIO(void);
void initPin(sPin_t* pin, uint8_t value);
void updateLastState(sPin_t* pin);
void updateState(sPin_t* pin, uint8_t value);
uint8_t risingEdge(sPin_t* pin);
//uint8_t fallingEdge(sPin_t* pin);

#endif
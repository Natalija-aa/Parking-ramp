#include "gpio.h"

void initGPIO(void)
{
 	P0 = 0xFF;
	P2 = 0x00;
}

void initPin(sPin_t* pin, uint8_t value)
{
	pin->state = value;
	pin->last_state = value;
}

void updateLastState(sPin_t* pin)
{
   	pin->last_state = pin->state;
}

void updateState(sPin_t* pin, uint8_t value)
{
	pin->state = value;
}


// return 1 on rising edge
uint8_t risingEdge(sPin_t* pin)
{
	return pin->state == 1 && pin->last_state == 0;
}
/*
// return 1 on falling edge
uint8_t fallingEdge(sPin_t* pin)
{
	return pin->state == 0 && pin->last_state == 1;
}
*/


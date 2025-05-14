#ifndef SERIAL_H
#define SERIAL_H

#include "types.h"
#include "protocol.h"
#include "gpio.h"
#include "display.h"

#include <math.h>

#define F_PER 11059200/2
#define BUFF_SIZE 20

void initUART(uint8_t smod1, uint8_t spd, uint8_t baud);
void enableGlobal_IT(uint8_t enable);
void enableUART_IT(uint8_t enable);

extern sBuffer_t r_buff;
extern sBuffer_t t_buff;

#endif
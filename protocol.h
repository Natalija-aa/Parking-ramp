#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "types.h"
#include "serial.h"
#include "gpio.h"
#include "display.h"

void send(uint8_t *msg);
void parse();
//void decode(uint8_t *msg);
void encode(uint8_t *msg, uint8_t len);

#endif
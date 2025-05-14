#ifndef TIMERS_H
#define TIMERS_H

#include "types.h"

extern volatile uint8_t tim1_int;
extern volatile uint8_t tim1_int_debounce;
extern uint8_t final_int;
extern uint8_t flag;

// inicijalizacija timera 0
void initTIM1(uint8_t mode, uint8_t TH0_reg, uint8_t TL0_reg, uint16_t max);
void resetTIM1(uint8_t TH0_reg, uint8_t TL0_reg, uint16_t max);
void setTIM1_reg(uint8_t TH0_reg, uint8_t TL0_reg, uint16_t max);
void startTIM1(void);
void stopTIM1(void);

void enableGlobalIT(uint8_t on);

#endif
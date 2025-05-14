#include "timers.h"
#include "types.h"

uint16_t tim1_cnt = 0;
uint16_t tim1_cnt_max;

uint8_t tim1_int = 0;
uint8_t tim1_int_debounce = 0;

uint8_t TH1_val = 0;
uint8_t TL1_val = 0;

void initTIM1(uint8_t mode, uint8_t TH1_reg, uint8_t TL1_reg, uint16_t max)
{
	setTIM1_reg(TH1_reg, TL1_reg, max);

	TMOD |= mode;
	TH1 = TH1_reg;
	TL1 = TL1_reg;
	ET1 = 1;
}

void setTIM1_reg(uint8_t TH1_reg, uint8_t TL1_reg, uint16_t max)
{
   	TH1_val = TH1_reg;
	TL1_val = TL1_reg;
	tim1_cnt_max = max;

	if(tim1_cnt_max == 0x64 && TH1_reg == 0x4C && TL1_reg == 0xA6){
		flag = 0x01;
	}
}

void resetTIM1(uint8_t TH1_reg, uint8_t TL1_reg, uint16_t max)
{
	tim1_cnt_max = max;
 	TH1 = TH1_reg;
	TL1 = TL1_reg;
	/*
	if(tim1_cnt_max == 0x64 && TH1_reg == 0x4C && TL1_reg == 0xA6){
		flag = 0x01;
	}
	*/
	tim1_cnt = 0;
}

void startTIM1(void)
{
	TR1 = 1;
}

void stopTIM1(void)
{
 	TR1 = 0;
}

void enableGlobalIT(uint8_t on)
{
 	EA = (on & 0x01);
}

void TIM1_IT(void) interrupt 3
{
	stopTIM1();
	TH1 = TH1_val;
	TL1 = TL1_val;
 	
	++tim1_cnt;

	if(flag == 0x01){
		if(tim1_cnt == tim1_cnt_max){
			tim1_cnt = 0;
			tim1_int = 1;
			final_int = 1;
		}	
	}else{
		if(tim1_cnt == tim1_cnt_max){
			tim1_cnt = 0;
			tim1_int = 1;
			final_int = 0;
	}
	}

	// svaki prekid
	tim1_int_debounce = 1;

	startTIM1();
}
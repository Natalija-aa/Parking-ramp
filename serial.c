#include "serial.h"

uint8_t buffer[BUFF_SIZE];
sBuffer_t r_buff = {buffer, 0};
sBuffer_t t_buff;
uint8_t receive;
uint8_t duz = 0;
uint8_t kraj = 0;

void initUART(uint8_t smod1, uint8_t spd, uint8_t baud)
{
	// Iskljucivanje X2 moda 
	CKCON0 &= ~(0x01);

	// Konfiguracija baudrate-a i dozvola konfiguracije modova u SCON
	PCON &= ~(0xC0);
	PCON |= ((smod1 & 0x01) << 7);

	// Podesavanje 1. moda rada - 8-bitni UART
	SCON &= ~(0x30);
	SCON |= (0x01 << 6);

	// Dozvola UART primanja
	SCON |= (0x01 << 4);

	// Podesavanje baudrate-a
	BDRCON |= ((spd & 0x01) << 1 | 0x01 << 2 | 0x01 << 3 | 0x01 << 4);
	BRL = baud;
}

void enableUART_IT(uint8_t enable)
{
	ES = (enable & 0x01);
}

void enableGlobal_IT(uint8_t enable)
{
	EA = (enable & 0x01);
}

void UART_IT(void) interrupt 4
{	
	if(TI){
		TI = 0;

		// slanje bajt po bajt
		++t_buff.buff;
		++duz;
		if(*t_buff.buff != 0x00 && !kraj){
			SBUF = *t_buff.buff;		
		}else if(!kraj && *t_buff.buff == 0x00){
			SBUF = *t_buff.buff;
			kraj= 1;
		}
	} 
	
	if (RI){
		RI = 0;

		// prijem bajt po bajt
		receive = SBUF;
	//	send(&receive);

		// upis u buffer i update indeksa buffera
		r_buff.buff[r_buff.index] = receive;

		// overflow zastita
		r_buff.index = (r_buff.index + 1) % BUFF_SIZE;
		
		if(receive == 0x00){
			parse();
			r_buff.buff[r_buff.index] = '\0';
			r_buff.index = 0;
			kraj = 0;		
		}
	}
}

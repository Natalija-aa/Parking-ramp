#include "protocol.h"
#include "gpio.h"
#include "display.h"

uint8_t previosCar_cnt = 0x00;

uint8_t decodedMsg[2];
extern uint8_t encodedMsg[3];
uint8_t next0Index = 0;

uint8_t ack[2];
uint8_t error[2];
uint8_t len;
uint8_t index;
uint8_t i;

void parse()
{
	ack[0] = 0xAA;
	ack[1] = 0xAA;
	len = 2;

	error[0] = 0xFB;
	error[1] = 0xBF;
  
}

void send(uint8_t *msg){

	t_buff.buff = msg;

	// Inicijalno slanje
	SBUF = *t_buff.buff;
		
}

//mozes da vratis vrijednost ako treba u vise fajlova
//moze isto i za encode da se isto rai pa da se zove send u parse, a ne u encode
/*
void decode(uint8_t *msg){
	uint8_t i = 0;
	uint8_t len = 0;
	next0Index = msg[0];

	while(msg[i] != 0x00){
	 	len++;
		i++;
	}
	len++;

	for(i = 0; i < len; i++){
		if(i == next0Index){
			decodedMsg[i-1] = 0x00;
			next0Index = next0Index + msg[i];
		}else{
		 	decodedMsg[i-1] = msg[i];
		}
	}
}
*/
void encode(uint8_t *msg, uint8_t len){
	uint8_t i = 0;
	uint8_t previos0Index = 0;
	int8_t current0Index = 0;

	for(i = 0; i < len; i++){
		if(msg[i] != 0x00){
			encodedMsg[i+1] = msg[i];
		}else{
		 	current0Index = i+1;
			encodedMsg[previos0Index] = current0Index - previos0Index;
			previos0Index = current0Index;
		}
	}

   	current0Index = i + 1;
	encodedMsg[previos0Index] = current0Index - previos0Index;
	encodedMsg[i+1] = 0x00;
	send(encodedMsg);
}
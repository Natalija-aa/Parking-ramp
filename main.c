#include "serial.h"
#include "gpio.h"
#include "timers.h"
#include "display.h"

//globalne promjenljive
//simulacija senzora
sPin_t pinP0_6;	//down
sPin_t pinP0_7;	//up

//debounce
uint8_t debounce6 = 0;
uint8_t debounce6_cnt = 0;
uint8_t debounce7 = 0;
uint8_t debounce7_cnt = 0;

//prekidaci podignuto spusteno
uint8_t tmp_up = 0x00;
uint8_t tmp_down = 0x00;
uint8_t up;				
uint8_t down = 0xFF;		

uint8_t final_int = 0x00;
uint8_t tmp = 0x00;	

extern uint8_t car_cnt = 0x00;		//brojac auta na parkingu

uint8_t flag = 0x00;
uint8_t strBr[3];		//dva karaktera plus terminator
uint8_t br = 0x00;
uint8_t encodedMsg[3];
uint8_t str[3];


void Podigni(void){
	P2 = 0x80;
	down = 0x00;
}

void Spusti(void){
 	P2 = 0x01;
	up = 0x00;
}

void Podignuta(void){
	P2 = 0xFF;
}

void Spustena(void){
	P2 = 0x00;
}

//hex u string
void HexToStr(uint8_t hex, char* str) {
	uint8_t index = 0;
	uint8_t tmpStr[3];
	uint8_t ostatak;
	uint8_t duzina;
	uint8_t i = 0;

	do{
		ostatak = hex % 10;
		
		if(ostatak < 10){
			tmpStr[index++] = '0' + ostatak;
		}	

		hex/=10;

	}while(hex != 0);

	tmpStr[index] = '\0';
   	
	duzina = index;	

	for(i = 0; i < duzina; i++){
	 	str[i] = tmpStr[duzina - i - 1];
	}

	str[duzina] = '\0';
}

//UP i DOWN su u stvarnoj situaciji senzori koji reaguju na kontakt 
//tako da je 'nemoguce' da se oba aktiviraju istovremeno

void main(void){

	initGPIO();
	initPin(&pinP0_6, P0_6);
	initPin(&pinP0_7, P0_7);
	if(flag != 0x01){
	initTIM1(0x10, 0xEE, 0x00, 1);
	}
	enableGlobalIT(1);

	initUART(1, 0, 253);
	enableUART_IT(1);
	enableGlobal_IT(1);

	initDisplay();
	clearDisplay();

	startTIM1();

//	encode(&car_cnt, 1);
	writeLine("SLOBODAN ULAZ");
	newLine();

//PROBLEM KADA POZOVEM ENCODE U WHILE ISPISUJE SE SAMO PRVA CIFRA A DOBRO SE ENCODUJE PORUKA
	while(1){

		//Simulacija senzora up i down kada dodje do krajnjih pozicija
		//prekidac te koristimo debounce

		updateState(&pinP0_6, P0_6);

		if(risingEdge(&pinP0_6) && !debounce6){
			debounce6 = 1;
		   	tmp_down = ~tmp_down;
		}

		if(tim1_int_debounce && debounce6){
			tim1_int_debounce = 0;

			if(pinP0_6.last_state == pinP0_6.state){
				debounce6_cnt++;
			}else{
			 	debounce6_cnt = 0;
			}

			if(debounce6_cnt > 5 - 1){
				debounce6 = 0;
				debounce6_cnt = 0;
				down = tmp_down; 
				tmp_down = ~tmp_down;  //jer ne ide upali ugasi vec samo upali

				resetTIM1(0xEE, 0x00, 2);
			}
		 }
		
		updateLastState(&pinP0_6);


		updateState(&pinP0_7, P0_7);

		if(risingEdge(&pinP0_7) && !debounce7){
			debounce7 = 1;
		   	tmp_up = ~tmp_up;
		}

		if(tim1_int_debounce && debounce7){
			tim1_int_debounce = 0;

			if(pinP0_7.last_state == pinP0_7.state){
				debounce7_cnt++;
			}else{
			 	debounce7_cnt = 0;
			}

			if(debounce7_cnt > 5 - 1){
				debounce7 = 0;
				debounce7_cnt = 0;
				up = tmp_up; 
				tmp_up = ~tmp_up;  	//jer ne ide upali ugasi vec samo upali

				resetTIM1(0xEE, 0x00, 2);
			}
		 }
		
		updateLastState(&pinP0_7);


		if(tim1_int){

			//ZA IN
			
			//neko je na ulazu i rampa je spustena, ima mjesta na parkingu za jos auta
			if(P0_0 == 0 && down == 0xFF && up == 0x00 && car_cnt < 0x0F){ 	
				Podigni();
			//neko je na ulazu, rampa je u procesu dizanja(P2) i pritisnut je taster up, ima mjesta na parkingu za jos auta
			}else if(P0_0 == 0 && P2 == 0x80 && down == 0x00 && up == 0xFF && car_cnt < 0x0F){ 	
				Podignuta();
				tmp = 0x01;	 					//za ulaz
				initTIM1(0x10, 0x4C, 0xA6, 100); //5s
			//proslo je 5s u podigntom polozaju, up je aktiviran, ima mjesta na parkingu
			}else if(P2 == 0xFF && up ==0xFF && down == 0x00 && flag == 0x01 && final_int == 0x01 && car_cnt < 0x0F && tmp == 0x01){
				if(P0_0 == 1){ 		//ako je uslo auto povecaj broj auta na rarkingu
					car_cnt++;
					Spusti();
				}else{
					Spusti();
				}

				flag = 0x00;	  //za 5 s vrati na 0
			//rampa se spusta P2 = 0x01, pritisnut je prekidac down i zadovoljen je uslov za broj auta
			}else if(P2 == 0x01 && up == 0x00 && down == 0xFF && (car_cnt == 0x0F || car_cnt < 0x0F)){		 
			   Spustena();
			  // encode(&car_cnt, 1);	//ovo ne ispisuje citavu poruku
					//ISPISI PORUUKE o zauzetosti
					if(car_cnt == 0x0F){
						br = 15 - car_cnt;

						clearDisplay();
						HexToStr(br, str);
						writeLine(str);
						newLine();
					}else{
						encode(&car_cnt, 1);	//samo prvi put se ispise a kasnije samo po djelic poruke a ne cijela
						
						br = 15 - car_cnt;
							
						clearDisplay();
						HexToStr(br, str);
						writeLine(str);
						newLine();
					}
		
			}

			
			//ZA OUT
			//neko je na izlazu i rampa je spustena, ima auta na parkingu
			if(P0_1 == 0 && down == 0xFF && up == 0x00 && car_cnt > 0x00){ 	
				Podigni();
			//neko je na izlazu, rampa se podize i pritisnut je aster up, ima auta na parkingu
			}else if(P0_1 == 0 && P2 == 0x80 && down == 0x00 && up == 0xFF && car_cnt > 0x00){ 	
				Podignuta();
				tmp = 0x02;
				initTIM1(0x10, 0x4C, 0xA6, 100);   //5s
			//proslo je 5s u podigntom polozaju, ima auta na parkingu
			}else if(P2 == 0xFF && up ==0xFF && down == 0x00 && final_int == 0x01 && car_cnt > 0x00 && tmp == 0x02){	
				if(P0_1 == 1){ 		//ato je auto izaslo smanji broj auta
					car_cnt--;
					Spusti();
				}else{
					Spusti();
				}
				flag = 0x00;
			//rampa se spustala i dosla je do kraja(pritisnuto je down)
			}else if(P2 == 0x01 && up == 0x00 && down == 0xFF && (car_cnt == 0x00 || car_cnt > 0x00)){	
				Spustena();
				encode(&car_cnt, 1);

				br = 15 - car_cnt;	//broj slobodnih mjesta na parkingu
				//encode(&br, 1);

				//ISPIS PORUKA
				clearDisplay();
				HexToStr(br, str);
				writeLine(str);
				newLine();

			}

			if(car_cnt == 0x0F){
				clearDisplay();
				writeLine(15);
				newLine();
			}
		}

	}

}
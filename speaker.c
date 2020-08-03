#include <avr/io.h>
#include "speaker.h"
#define speaker_set DDRB|=0x80;
#define speaker_in PORTB
#define off speaker_in&=0x7F;
#define on speaker_in|=0x80;


void speaker(){


	for(int i=0;i<800;i++){
		if(i<400){
			on;
		}
		else off;
	}
}


void speaker_init(){
	speaker_set;
}

